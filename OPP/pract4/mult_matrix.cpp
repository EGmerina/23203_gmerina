#include <iostream>
#include <float.h>
#include <mpi.h>

#define SCALING_FACTOR_N 3
#define CICLES_NUM 1

/**
 * для B отправляем новый тип, получаем double[]
 * для собирания матрицы C  отправляем double[] получаем new type
 * для создания коммуникаторов сздаем group из comm2d, бьем группу , и из ее частей создаем новые коммуникаторы
 * лучше использовать send recv
 */

void print_matrix(double *matrix, int N1, int N2)
{
    for (size_t i = 0; i < N1; i++)
    {
        for (size_t j = 0; j < N2; j++)
        {
            std ::cout << matrix[i * N1 + j] << " ";
        }
        std::cout << std::endl;
    }
}

void print_vec(double *vec, int N)
{
    for (size_t i = 0; i < N; i++)
    {
        std ::cout << vec[i] << " ";
    }
    std::cout << std::endl;
}

/////////////////////////////////////////////

int main(int argc, char *argv[])
{
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 3)
    {
        std::cout << "Usage : <ex. file > <p1> (vertical dimension)  <p2> (horizontal dimension) " << std::endl;
        MPI_Finalize();
        return 0;
    }
    char *p_end{};
    int P1 = std::strtol(argv[1], &p_end, 10);
    int P2 = std::strtol(argv[2], &p_end, 10);
    if (P1 * P2 != size)
    {
        std::cout << "the number of processes must match the size. Try -n " << P1 * P2 << std::endl;
        MPI_Finalize();
        return 0;
    }

    int N1 = SCALING_FACTOR_N * P1;
    int N3 = SCALING_FACTOR_N * P2;
    int N2 = (int)((N1 + N3) / 2);

    double *A;
    double *B;
    double *C;

    // create topolgy of processes

    int dims[] = {P1, P2};
    int periods[] = {0, 0};
    int reorder = 1;
    MPI_Comm comm2d;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &comm2d);
    MPI_Comm_rank(comm2d, &rank);

    // MPI_Cart_get(comm2d, 2, dims, periods, coords);
    // int ranky = coords[0];
    // int rankx = coords[1];
    // std::cout << "im rank " << rank << " on coords " << rankx << " " << ranky << std::endl;

    MPI_Comm row_comm, col_comm;
    int remain_dims_cols[2] = {0, 1}; // Я ЗАПУТАЛАСЬ С X И Y!!!!!!
    int remain_dims_rows[2] = {1, 0};
    MPI_Cart_sub(comm2d, remain_dims_rows, &row_comm);
    MPI_Cart_sub(comm2d, remain_dims_cols, &col_comm);

    int coords[2];
    MPI_Cart_coords(comm2d, rank, 2, coords);

    MPI_Comm first_row_comm;
    int color = (coords[0] == 0) ? 1 : MPI_UNDEFINED;
    MPI_Comm_split(comm2d, color, 0, &first_row_comm);

    MPI_Comm first_col_comm;
    color = (coords[1] == 0) ? 1 : MPI_UNDEFINED;
    MPI_Comm_split(comm2d, color, 0, &first_col_comm);

    // int num = 0;
    // if (rank == 0)
    // {
    //     num = 1;
    // }
    // if (rank == 2)
    // {
    //     num = 3;
    // }

    // if (first_row_comm != MPI_COMM_NULL)
    //     MPI_Bcast(&num, 1, MPI_INT, 0, first_row_comm);
    // std::cout << " rank " << rank << " data : " << num << std::endl;

    if (rank == 0)
    {

        A = new double[N1 * N2];
        // std::fill(A, A + N1 * N2, 0);
        // init A
        for (size_t i = 0; i < N1; i++)
        {
            for (size_t j = 0; j < N2; j++)
            {
                A[i * N2 + j] = i * N2 + j;
            }
        }

        B = new double[N2 * N3];
        // std::fill(B, B + N2 * N3, 0);
        // init B
        for (size_t i = 0; i < N2; i++)
        {
            for (size_t j = 0; j < N3; j++)
            {
                B[i * N3 + j] = i * N3 + j + 2;
            }
        }
        // print_matrix(A, N1, N2);
        // std::cout << "//////////////////////" << std::endl;
        // print_matrix(B, N2, N3);
        C = new double[N1 * N3];
    }

    int num_row = N1 / P1;
    int num_col = N3 / P2;
    double *row = new double[num_row * N2];
    double *col = new double[num_col * N2];
    double *res = new double[num_col * num_row];
    std::fill(res, res + num_col * num_row, 0);

    double start, end;
    double total_time = DBL_MAX;

    for (size_t k = 0; k < CICLES_NUM; k++)
    {

        start = MPI_Wtime();

        if (first_col_comm != MPI_COMM_NULL)
            MPI_Scatter(A, num_row * N2, MPI_DOUBLE, row, num_row * N2, MPI_DOUBLE, 0, first_col_comm);

        MPI_Bcast(row, num_row * N2, MPI_DOUBLE, 0, col_comm);
        // std::cout << " rank row" << rank << std::endl;
        // print_vec(row, num_row * N2);

        MPI_Datatype column_type;
        MPI_Type_vector(N2, num_col, N3 - num_col + 1, MPI_DOUBLE, &column_type);
        MPI_Type_commit(&column_type);

        MPI_Datatype continuos_col_type;
        MPI_Aint lb, extent;

        // Получаем текущие границы и размер типа данных для одного столбца
        MPI_Type_get_extent(column_type, &lb, &extent);

        // Создаем новый тип данных для нескольких столбцов
        MPI_Type_create_resized(column_type, lb, extent * (N3 / row_size), &continuos_col_type);
        MPI_Type_commit(&continuos_col_type);

        if (first_row_comm != MPI_COMM_NULL)
            MPI_Scatter(B, 1, continuos_col_type, col, num_col * N2, MPI_DOUBLE, 0, first_row_comm);

        MPI_Bcast(col, num_col * N2, MPI_DOUBLE, 0, row_comm);
        // if (rank == 0)
        // {

        //     for (int i = 1; i < P1; i++)
        //     {
        //         MPI_Send(A + N2 * i * num_row, N2 * num_row, MPI_DOUBLE,
        //                  i, 0, comm2d);
        //     }
        //     for (int i = 0; i < num_row * N2; i++)
        //     {
        //         row[i] = A[i];
        //     }

        //     for (int i = 1; i < 2; i++)
        //     {
        //         MPI_Send(B + i * num_col, 1, column_type,
        //                  P1 * i, 1, comm2d);
        //     }
        //     // Копируем свои столбцы
        //     for (int i = 0; i < N2; i++)
        //     {
        //         for (int j = 0; j < num_col; j++)
        //         {
        //             col[num_col * i + j] = B[i * N3 + j];
        //         }
        //     }
        // }
        // else
        // {

        //     MPI_Recv(row, N2 * num_row, MPI_DOUBLE,
        //              0, 0, comm2d, MPI_STATUS_IGNORE);

        //     MPI_Recv(col, N2 * num_col, MPI_DOUBLE,
        //              0, 1, comm2d, MPI_STATUS_IGNORE);
        // }

        for (size_t i = 0; i < num_row; i++)
        {
            double *c = res + i * num_col;
            for (size_t cnt = 0; cnt < N2; cnt++)
            {
                const double *b = col + cnt * N2;
                double a = row[i * N2 + cnt];
                for (size_t j = 0; j < num_col; j++)
                {
                    c[j] += a * b[j];
                }
            }
        }

        MPI_Datatype part_matrix_type;
        MPI_Type_vector(num_row, num_col, N3, MPI_DOUBLE, &part_matrix_type);
        MPI_Type_commit(&part_matrix_type);

        MPI_Gather(res, num_col * num_row, MPI_DOUBLE, C, 1, part_matrix_type, 0, comm2d);

        MPI_Type_free(&column_type);
        MPI_Type_free(&continuos_col_type);
        MPI_Type_free(&part_matrix_type);
        end = MPI_Wtime();
        total_time = (end - start < total_time) ? end - start : total_time;
    }

    delete[] row;
    delete[] col;
    delete[] res;

    if (rank == 0)
    {
        std::cout << "time : " << total_time << std::endl;
        // std::cout << " rank " << rank << std::endl;
        // print_matrix(C, N1, N3);

        delete[] A;
        delete[] B;
        delete[] C;
    }

    MPI_Finalize();
    return 0;
}