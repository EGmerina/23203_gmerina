
#include <iostream>
#include <vector>
#include <cmath>
#include <pthread.h>
#include <mpi.h>
#include <unistd.h>
#include <random>
#include <algorithm>
#include <numeric>
#include <iomanip>

struct Task
{
    int repeatNum;
    bool completed;
};

struct ProcessStats
{
    int tasksProcessed = 0;
    double idleTime = 0.0;
    double workTime = 0.0;
    int tasksReceived = 0;
    int tasksSent = 0;
};

std::vector<Task> taskList;
pthread_mutex_t taskMutex;
pthread_cond_t tasksAvailable;
double globalRes = 0.0;
bool allTasksDone = false;
int rank, size;
MPI_Datatype MPI_TASK_TYPE, MPI_STATS_TYPE;
ProcessStats stats;

void *workerThread(void *arg);
void *requestThread(void *arg);
void *responseThread(void *arg);
void printFinalStats();

void *workerThread(void *arg)
{
    while (true)
    {
        double idleStart = MPI_Wtime();
        pthread_mutex_lock(&taskMutex);
        while (std::none_of(taskList.begin(), taskList.end(), [](const Task &t)
                            { return !t.completed; }) &&
               !allTasksDone)
        {
            pthread_cond_wait(&tasksAvailable, &taskMutex);
        }
        if (allTasksDone)
        {
            pthread_mutex_unlock(&taskMutex);
            break;
        }

        auto it = std::find_if(taskList.begin(), taskList.end(), [](const Task &t)
                               { return !t.completed; });
        if (it == taskList.end())
        {
            pthread_mutex_unlock(&taskMutex);
            continue;
        }

        Task task = *it;
        it->completed = true;
        pthread_mutex_unlock(&taskMutex);
        stats.idleTime += MPI_Wtime() - idleStart;

        double workStart = MPI_Wtime();
        double result = 0.0;
        for (int i = 0; i < task.repeatNum; ++i)
        {
            volatile double tmp = sqrt(i + 1);
            result += tmp;
            // Дополнительная нагрузка
            for (int j = 0; j < 10; j++)
            {
                volatile double tmp2 = sin(i + j);
                result += tmp2;
            }
        }
        usleep(5000);

        pthread_mutex_lock(&taskMutex);
        globalRes += result;
        stats.tasksProcessed++;
        pthread_mutex_unlock(&taskMutex);
        stats.workTime += MPI_Wtime() - workStart;
    }
    return nullptr;
}

void *requestThread(void *arg)
{

    std::vector<int> processRanks(size);
    std::iota(processRanks.begin(), processRanks.end(), 0);
    std::random_device rd;
    std::mt19937 g(rd());

    while (!allTasksDone)
    {
        pthread_mutex_lock(&taskMutex);
        bool needsTasks = std::none_of(taskList.begin(), taskList.end(),
                                       [](const Task &t)
                                       { return !t.completed; });
        needsTasks = needsTasks && !allTasksDone;
        pthread_mutex_unlock(&taskMutex);

        if (!needsTasks)
        {
            usleep(20000);
            continue;
        }

        std::shuffle(processRanks.begin(), processRanks.end(), g);
        for (auto targetRank : processRanks)
        {
            if (targetRank == rank)
                continue;

            int requestCode = 1;
            MPI_Send(&requestCode, 1, MPI_INT, targetRank, 0, MPI_COMM_WORLD);

            int flag = 0;
            MPI_Status status;
            MPI_Iprobe(targetRank, 1, MPI_COMM_WORLD, &flag, &status);

            if (flag)
            {
                int numTasks;
                MPI_Recv(&numTasks, 1, MPI_INT, targetRank, 1, MPI_COMM_WORLD, &status);
                if (numTasks > 0)
                {
                    std::vector<Task> receivedTasks(numTasks);
                    MPI_Recv(receivedTasks.data(), numTasks, MPI_TASK_TYPE, targetRank, 2,
                             MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                    pthread_mutex_lock(&taskMutex);
                    for (auto &task : receivedTasks)
                    {
                        task.completed = false;
                        taskList.push_back(task);
                    }
                    stats.tasksReceived += numTasks;
                    pthread_cond_broadcast(&tasksAvailable);
                    pthread_mutex_unlock(&taskMutex);
                    break;
                }
            }
        }
    }
    return nullptr;
}

void *responseThread(void *arg)
{

    while (!allTasksDone)
    {
        MPI_Status status;
        int flag;
        MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &flag, &status);

        if (flag)
        {
            int sourceRank = status.MPI_SOURCE;
            int requestCode;
            MPI_Recv(&requestCode, 1, MPI_INT, sourceRank, 0, MPI_COMM_WORLD, &status);
            pthread_mutex_lock(&taskMutex);
            int availableTasks = std::count_if(taskList.begin(), taskList.end(),
                                               [](const Task &t)
                                               { return !t.completed; });

            if (availableTasks > 10)
            {                                                         // Увеличил порог
                int numTasksToSend = std::min(5, availableTasks / 3); // Увеличил количество
                std::vector<Task> tasksToSend;
                for (auto &t : taskList)
                {
                    if (!t.completed && tasksToSend.size() < numTasksToSend)
                    {
                        tasksToSend.push_back(t);
                        t.completed = true;
                    }
                }
                MPI_Send(&numTasksToSend, 1, MPI_INT, sourceRank, 1, MPI_COMM_WORLD);
                MPI_Send(tasksToSend.data(), numTasksToSend, MPI_TASK_TYPE, sourceRank, 2, MPI_COMM_WORLD);
                stats.tasksSent += numTasksToSend;
            }
            else
            {
                int zero = 0;
                MPI_Send(&zero, 1, MPI_INT, sourceRank, 1, MPI_COMM_WORLD);
            }
            pthread_mutex_unlock(&taskMutex);
        }
        usleep(10000);
    }
    return nullptr;
}

int main(int argc, char **argv)
{
    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    if (provided < MPI_THREAD_MULTIPLE)
    {
        std::cerr << "MPI implementation doesn't support MPI_THREAD_MULTIPLE" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Создание MPI типа для Task
    MPI_Datatype types[2] = {MPI_INT, MPI_C_BOOL};
    int blocklengths[2] = {1, 1};
    MPI_Aint offsets[2];
    offsets[0] = offsetof(Task, repeatNum);
    offsets[1] = offsetof(Task, completed);
    MPI_Type_create_struct(2, blocklengths, offsets, types, &MPI_TASK_TYPE);
    MPI_Type_commit(&MPI_TASK_TYPE);

    // Создание MPI типа для ProcessStats
    MPI_Datatype statsTypes[5] = {MPI_INT, MPI_DOUBLE, MPI_DOUBLE, MPI_INT, MPI_INT};
    int statsBlocklengths[5] = {1, 1, 1, 1, 1};
    MPI_Aint statsOffsets[5];
    statsOffsets[0] = offsetof(ProcessStats, tasksProcessed);
    statsOffsets[1] = offsetof(ProcessStats, workTime);
    statsOffsets[2] = offsetof(ProcessStats, idleTime);
    statsOffsets[3] = offsetof(ProcessStats, tasksReceived);
    statsOffsets[4] = offsetof(ProcessStats, tasksSent);
    MPI_Type_create_struct(5, statsBlocklengths, statsOffsets, statsTypes, &MPI_STATS_TYPE);
    MPI_Type_commit(&MPI_STATS_TYPE);

    pthread_mutex_init(&taskMutex, nullptr);
    pthread_cond_init(&tasksAvailable, nullptr);

    pthread_t worker, requester, responder;
    pthread_create(&worker, nullptr, workerThread, nullptr);

    pthread_create(&requester, nullptr, requestThread, nullptr);
    pthread_create(&responder, nullptr, responseThread, nullptr);

    double totalStart = MPI_Wtime();

    for (int iterCounter = 1; iterCounter <= 10; ++iterCounter)
    { 
        double iterStart = MPI_Wtime();

        pthread_mutex_lock(&taskMutex);
        taskList.clear();
        int baseTasks = 200 + (rank % 3) * 100;
        for (int i = 0; i < baseTasks; ++i)
        {
            int weight = 5000 + abs(rank - (iterCounter % size)) * 500;
            taskList.push_back(Task{weight, false});
        }
        pthread_cond_broadcast(&tasksAvailable);
        pthread_mutex_unlock(&taskMutex);

        while (true)
        {
            pthread_mutex_lock(&taskMutex);
            bool allDone = std::all_of(taskList.begin(), taskList.end(),
                                       [](const Task &t)
                                       { return t.completed; });
            pthread_mutex_unlock(&taskMutex);

            int globalDone = allDone ? 1 : 0;
            int totalDone;
            MPI_Allreduce(&globalDone, &totalDone, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
            if (totalDone == size)
                break;

            usleep(50000);
        }

        MPI_Barrier(MPI_COMM_WORLD);
        double iterTime = MPI_Wtime() - iterStart;

        if (rank == 0)
        {
            std::cout << "Итерация " << iterCounter << " завершена за "
                      << std::fixed << std::setprecision(3) << iterTime << " секунд"
                      << std::endl;
        }
    }

    allTasksDone = true;
    pthread_cond_broadcast(&tasksAvailable);
    pthread_join(worker, nullptr);

    pthread_join(requester, nullptr);
    pthread_join(responder, nullptr);

    double totalTime = MPI_Wtime() - totalStart;
    printFinalStats();

    pthread_mutex_destroy(&taskMutex);
    pthread_cond_destroy(&tasksAvailable);
    MPI_Type_free(&MPI_TASK_TYPE);
    MPI_Type_free(&MPI_STATS_TYPE);
    MPI_Finalize();
    return 0;
}

void printFinalStats()
{
    std::vector<ProcessStats> allStats(size);
    MPI_Gather(&stats, 1, MPI_STATS_TYPE,
               allStats.data(), 1, MPI_STATS_TYPE,
               0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        std::cout << "\n=== Итоговая статистика ===" << std::endl;
        std::cout << "Ранг | Заданий | Время работы | Время простоя | Получено | Отправлено" << std::endl;
        std::cout << "---------------------------------------------------------------" << std::endl;
        std::cout << std::fixed << std::setprecision(3);

        for (int i = 0; i < size; ++i)
        {
            std::cout << std::setw(4) << i << " | "
                      << std::setw(7) << allStats[i].tasksProcessed << " | "
                      << std::setw(12) << allStats[i].workTime << " | "
                      << std::setw(12) << allStats[i].idleTime << " | "
                      << std::setw(8) << allStats[i].tasksReceived << " | "
                      << std::setw(8) << allStats[i].tasksSent << std::endl;
        }

        double avgLoad = 0.0;
        for (int i = 0; i < size; ++i)
        {
            avgLoad += allStats[i].tasksProcessed;
        }
        avgLoad /= size;

        double imbalance = 0.0;
        for (int i = 0; i < size; ++i)
        {
            imbalance += fabs(allStats[i].tasksProcessed - avgLoad);
        }

        if (avgLoad > 0.0)
        {
            imbalance = (imbalance / (avgLoad * size)) * 100;
            std::cout << "\nДисбаланс нагрузки: " << std::setprecision(1) << imbalance << "%" << std::endl;
        }

        int totalTransferred = 0;
        for (int i = 0; i < size; ++i)
        {
            totalTransferred += allStats[i].tasksSent;
        }
        std::cout << "Всего перераспределено задач: " << totalTransferred << std::endl;
    }
}
