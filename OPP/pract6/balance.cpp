#include <iostream>
#include <vector>
#include <cmath>
#include <mpi.h>
#include <pthread.h>
#include <unistd.h>
#include <algorithm>

#define NUM_ITERATIONS 4
#define NUM_TASKS 10
#define L 5000

int rank, size;
std::vector<int> tasks_per_proc_list;
int num_remaining_tasks;
int max_num_remaining_tasks;
pthread_mutex_t task_mutex;
bool all_tasks_done = false;
double global_time = 0;

void init_tasks(std::vector<int> &tasks_per_proc, int iter_counter, int my_rank, int size)
{
    for (size_t i = 0; i < NUM_TASKS; i++)
    {
        int weight = abs(50 - i % NUM_TASKS) * abs(my_rank - (iter_counter % size) + 17) * L;
        tasks_per_proc.push_back(weight);
    }
}

void *check_end_thread(void *arg)
{
    while (!all_tasks_done)
    {

        pthread_mutex_lock(&task_mutex);
        num_remaining_tasks = tasks_per_proc_list.size();
        pthread_mutex_unlock(&task_mutex);

        MPI_Allreduce(&num_remaining_tasks, &max_num_remaining_tasks, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

        if (max_num_remaining_tasks == 0)
        {
            all_tasks_done = true;
            std::cout << "task done" << all_tasks_done << " rank " << rank << std::endl;
            break;
        }
        usleep(10000);
    }
    return nullptr;
}

void *request_thread(void *arg)
{
    MPI_Status status;
    int count;

    pthread_mutex_lock(&task_mutex);
    num_remaining_tasks = tasks_per_proc_list.size();
    pthread_mutex_unlock(&task_mutex);
    int target_rank = 0;

    while (!all_tasks_done)
    {

        pthread_mutex_lock(&task_mutex);
        num_remaining_tasks = tasks_per_proc_list.size();
        pthread_mutex_unlock(&task_mutex);
        if (num_remaining_tasks > 2)
        {
            usleep(20000);
            continue;
        }

        target_rank = (target_rank + 1) % size;
        if (target_rank == rank)
        {
            continue;
        }

        int my_rank_request = rank;

        MPI_Send(&my_rank_request, 1, MPI_INT, target_rank, rank, MPI_COMM_WORLD);

        int flag;
        MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &flag, &status);
        if (!flag)
        {
            continue;
        }

        MPI_Get_count(&status, MPI_INT, &count);
        int *recv_buf = new int[count];
        MPI_Recv(recv_buf, count, MPI_INT, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status);

        if (count == 0)
        {
            delete[] recv_buf;
            continue;
        }

        pthread_mutex_lock(&task_mutex);
        for (size_t i = 0; i < count; i++)
        {
            tasks_per_proc_list.push_back(recv_buf[i]);
        }
        pthread_mutex_unlock(&task_mutex);

        delete[] recv_buf;
    }

    return nullptr;
}

void *response_thread(void *arg)
{
    MPI_Status status;
    int flag;
    while (!all_tasks_done)
    {

        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);

        if (!flag)
        {
            usleep(10000);
            continue;
        }
        int requested_rank;

        MPI_Recv(&requested_rank, 1, MPI_INT, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status);
        int send_count = 0;

        pthread_mutex_lock(&task_mutex);
        num_remaining_tasks = tasks_per_proc_list.size();
        pthread_mutex_unlock(&task_mutex);
        if (num_remaining_tasks > 10)
        {
            send_count = num_remaining_tasks / 3;
        }
        int *send_buf = new int[send_count];

        if (send_count != 0)
        {
            pthread_mutex_lock(&task_mutex);
            for (size_t i = 0; i < send_count; i++)
            {
                send_buf[i] = tasks_per_proc_list.back();
                tasks_per_proc_list.pop_back();
            }
            pthread_mutex_unlock(&task_mutex);
        }

        MPI_Send(send_buf, send_count, MPI_INT, status.MPI_SOURCE, rank, MPI_COMM_WORLD);

        delete[] send_buf;
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
        MPI_Finalize();
        return 0;
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    pthread_mutex_init(&task_mutex, nullptr);

    double global_min_time, global_max_time;
    double imbalance_time;
    double imbalance_share;
    double global_res = 0;
    int tasks_counter = 0;

    pthread_t requester, responder, checker;

    for (int iter_counter = 0; iter_counter < NUM_ITERATIONS; iter_counter++)
    {
        if (rank == 0)
        {
            std::cout << " ITER_COUNTER=" << iter_counter << std::endl;
        }

        pthread_mutex_lock(&task_mutex);
        tasks_per_proc_list.clear();
        init_tasks(tasks_per_proc_list, iter_counter, rank, size);
        pthread_mutex_unlock(&task_mutex);

        tasks_counter = 0;

        pthread_create(&requester, nullptr, request_thread, nullptr);
        pthread_create(&responder, nullptr, response_thread, nullptr);

        pthread_create(&checker, nullptr, check_end_thread, nullptr);

        MPI_Barrier(MPI_COMM_WORLD);

        double start_time_pk = MPI_Wtime();
        while (!all_tasks_done)
        {
            if (tasks_per_proc_list.size() == 0)
            {
                continue;
            }
            pthread_mutex_lock(&task_mutex);
            int new_task_weight = tasks_per_proc_list.back();
            tasks_per_proc_list.pop_back();

            pthread_mutex_unlock(&task_mutex);

            for (size_t j = 0; j < new_task_weight; j++)
            {
                global_res += sin(j);
            }
            ++tasks_counter;
        }
        double end_time_pk = MPI_Wtime();

        pthread_join(requester, nullptr);
        pthread_join(responder, nullptr);

        pthread_join(checker, nullptr);

        double total_time_pk = end_time_pk - start_time_pk;
        std::cout << " iter_counter=" << iter_counter << " rank=" << rank << " time=" << total_time_pk << " tasks_counter=" << tasks_counter << " global_res=" << global_res << std::endl;

        MPI_Reduce(&total_time_pk, &global_max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        MPI_Reduce(&total_time_pk, &global_min_time, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);

        if (rank == 0)
        {
            global_time += total_time_pk;
            imbalance_time = global_max_time - global_min_time;
            imbalance_share = (imbalance_time / global_max_time) * 100;
            std::cout << " iter_counter=" << iter_counter << " imbalance_time=" << imbalance_time << " imbalance_share=" << imbalance_share << std::endl;
        }
        all_tasks_done = false;
    }

    if (rank == 0)
    {
        std::cout << "!!!!!!!!!global_time=" << global_time << std::endl;
    }
    pthread_mutex_destroy(&task_mutex);

    MPI_Finalize();
    return 0;
}