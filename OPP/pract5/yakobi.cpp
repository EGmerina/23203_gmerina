#include <iostream>
#include <vector>
#include <cmath>
#include <mpi.h>

const double a = 1e5;
const double epsilon = 1e-8;

void print_matrix(double *matrix, int N1, int N2, int N3)
{
    for (size_t i = 0; i < N1; i++)
    {
        for (size_t j = 0; j < N2; j++)
        {
            for (size_t k = 0; k < N3; k++)
            {
                std ::cout << matrix[i * N3 * N2 + j * N3 + k] << " ";
            }
        }
        std::cout << std::endl;
    }
}

////////////////////////////////////////////////////////

double rho(double x, double y, double z)
{
    return 6 - a * (x * x + y * y + z * z);
}

double phi_solution(double x, double y, double z)
{
    return x * x + y * y + z * z;
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const double x0 = -1.0, y0 = -1.0, z0 = -1.0;
    const double Dx = 2.0, Dy = 2.0, Dz = 2.0;

    if (argc != 2)
    {
        std::cout << "Usage : <ex. file >  <Nx> " << std::endl;
        MPI_Finalize();
        return 0;
    }

    char *p_end{};
    int Nx = std::strtol(argv[1], &p_end, 10);

    if (Nx % 16 != 0)
    {
        std::cout << "Nx should be divided for 16 " << std::endl;
        MPI_Finalize();
        return 0;
    }

    int Ny = Nx;
    int Nz = Nx;

    int local_nx = Nx / size;

    const double hx = Dx / (Nx - 1);
    const double hy = Dy / (Ny - 1);
    const double hz = Dz / (Nz - 1);

    int start_x = rank * local_nx;
    int end_x = start_x + local_nx - 1;

    int overlap_start = (start_x == 0) ? 0 : start_x - 1;
    int overlap_end = (end_x == Nx - 1) ? Nx - 1 : end_x + 1;
    int overlap_nx = overlap_end - overlap_start + 1;

    std::vector<std::vector<std::vector<double>>> phi(overlap_nx,
                                                      std::vector<std::vector<double>>(Ny, std::vector<double>(Nz, 0.0)));
    std::vector<std::vector<std::vector<double>>> phi_new(overlap_nx,
                                                          std::vector<std::vector<double>>(Ny, std::vector<double>(Nz, 0.0)));
    std::vector<std::vector<std::vector<double>>> rho_values(overlap_nx,
                                                             std::vector<std::vector<double>>(Ny, std::vector<double>(Nz, 0.0)));

    double *phi_part_result = new double[local_nx * Ny * Nz]();
    double *phi_result;
    if (rank == 0)
    {
        phi_result = new double[Nx * Ny * Nz]();
    }

    for (int i = 0; i < overlap_nx; ++i)
    {
        int global_i = overlap_start + i;
        double x = x0 + global_i * hx;

        for (int j = 0; j < Ny; ++j)
        {
            double y = y0 + j * hy;

            for (int k = 0; k < Nz; ++k)
            {
                double z = z0 + k * hz;

                if (global_i == 0 || global_i == Nx - 1 || j == 0 || j == Ny - 1 || k == 0 || k == Nz - 1)
                {
                    phi[i][j][k] = phi_solution(x, y, z);
                    phi_new[i][j][k] = phi[i][j][k];
                }

                rho_values[i][j][k] = rho(x, y, z);
            }
        }
    }

    const double hx2 = hx * hx;
    const double hy2 = hy * hy;
    const double hz2 = hz * hz;
    const double denominator = 2.0 / hx2 + 2.0 / hy2 + 2.0 / hz2 + a;

    MPI_Request requests[4];
    double *send_down = new double[Ny * Nz];
    double *recv_down = new double[Ny * Nz];
    double *send_up = new double[Ny * Nz];
    double *recv_up = new double[Ny * Nz];

    int iter = 0;
    double max_diff = 0.0;
    double global_max_diff = 0.0;

    double start_time = MPI_Wtime();

    while (global_max_diff > epsilon || iter == 0)
    {
        max_diff = 0.0;
        if (overlap_start < start_x)
        {
            int i = 1;
            for (int j = 1; j < Ny - 1; ++j)
            {
                for (int k = 1; k < Nz - 1; ++k)
                {
                    phi_new[i][j][k] = ((phi[i + 1][j][k] + phi[i - 1][j][k]) / hx2 +
                                        (phi[i][j + 1][k] + phi[i][j - 1][k]) / hy2 +
                                        (phi[i][j][k + 1] + phi[i][j][k - 1]) / hz2 -
                                        rho_values[i][j][k]) /
                                       denominator;

                    double diff = fabs(phi_new[i][j][k] - phi[i][j][k]);
                    if (diff > max_diff)
                        max_diff = diff;
                }
            }

            for (int j = 0; j < Ny; ++j)
            {
                for (int k = 0; k < Nz; ++k)
                {
                    send_down[j * Nz + k] = phi_new[i][j][k];
                }
            }
        }

        if (overlap_end > end_x)
        {

            int i = overlap_nx - 2;
            for (int j = 1; j < Ny - 1; ++j)
            {
                for (int k = 1; k < Nz - 1; ++k)
                {
                    phi_new[i][j][k] = ((phi[i + 1][j][k] + phi[i - 1][j][k]) / hx2 +
                                        (phi[i][j + 1][k] + phi[i][j - 1][k]) / hy2 +
                                        (phi[i][j][k + 1] + phi[i][j][k - 1]) / hz2 -
                                        rho_values[i][j][k]) /
                                       denominator;

                    double diff = fabs(phi_new[i][j][k] - phi[i][j][k]);
                    if (diff > max_diff)
                        max_diff = diff;
                }
            }

            for (int j = 0; j < Ny; ++j)
            {
                for (int k = 0; k < Nz; ++k)
                {
                    send_up[j * Nz + k] = phi_new[i][j][k];
                }
            }
        }

      
        if (rank > 0)
        {
            MPI_Isend(send_down, Ny * Nz, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &requests[0]);
            MPI_Irecv(recv_down, Ny * Nz, MPI_DOUBLE, rank - 1, 1, MPI_COMM_WORLD, &requests[1]);
        }

        if (rank < size - 1)
        {
            MPI_Isend(send_up, Ny * Nz, MPI_DOUBLE, rank + 1, 1, MPI_COMM_WORLD, &requests[2]);
            MPI_Irecv(recv_up, Ny * Nz, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &requests[3]);
        }

        //  Вычисление внутренних точек
        int inner_start = (overlap_start < start_x) ? 2 : 1;
        int inner_end = (overlap_end > end_x) ? overlap_nx - 2 : overlap_nx - 1;

        for (int i = inner_start; i < inner_end; ++i)
        {
            for (int j = 1; j < Ny - 1; ++j)
            {
                for (int k = 1; k < Nz - 1; ++k)
                {
                    phi_new[i][j][k] = ((phi[i + 1][j][k] + phi[i - 1][j][k]) / hx2 +
                                        (phi[i][j + 1][k] + phi[i][j - 1][k]) / hy2 +
                                        (phi[i][j][k + 1] + phi[i][j][k - 1]) / hz2 -
                                        rho_values[i][j][k]) /
                                       denominator;

                    double diff = fabs(phi_new[i][j][k] - phi[i][j][k]);
                    if (diff > max_diff)
                        max_diff = diff;
                }
            }
        }

        // Ожидание завершения обменов
        if (rank > 0)
        {
            MPI_Waitall(2, requests, MPI_STATUSES_IGNORE);

            for (int j = 0; j < Ny; ++j)
            {
                for (int k = 0; k < Nz; ++k)
                {
                    phi_new[0][j][k] = recv_down[j * Nz + k];
                }
            }
        }

        if (rank < size - 1)
        {
            MPI_Waitall(2, requests + 2, MPI_STATUSES_IGNORE);

            for (int j = 0; j < Ny; ++j)
            {
                for (int k = 0; k < Nz; ++k)
                {
                    phi_new[overlap_nx - 1][j][k] = recv_up[j * Nz + k];
                }
            }
        }

        // Обновление phi и вычисление максимальной разницы
        for (int i = 0; i < overlap_nx; ++i)
        {
            for (int j = 0; j < Ny; ++j)
            {
                for (int k = 0; k < Nz; ++k)
                {
                    phi[i][j][k] = phi_new[i][j][k];
                }
            }
        }

        // Находим максимальную разницу по всем процессам
        MPI_Allreduce(&max_diff, &global_max_diff, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

        iter++;
    }

    int start_ind = (rank == 0) ? 0 : 1;
    int end_ind = (rank == size - 1) ? overlap_nx - 1 : overlap_nx - 2;
    for (int i = start_ind; i < end_ind; ++i)
    {
        for (int j = 0; j < Ny; ++j)
        {
            for (int k = 0; k < Nz; ++k)
            {
                phi_part_result[i * Ny * Nz + j * Nz + k] = phi[i][j][k];
            }
        }
    }
    MPI_Gather(phi_part_result, local_nx * Nz * Ny, MPI_DOUBLE, phi_result, local_nx * Nz * Ny, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    if (rank == 0)
    {
        std::cout << "Iterations: " << iter << std::endl;
        std::cout << "Time: " << end_time - start_time << " seconds" << std::endl;
        //print_matrix(phi_result, Nx, Ny, Nz);
        delete[] phi_result;
    }

    // Вычисление ошибки
    double local_error = 0.0;
    for (int i = 1; i < overlap_nx - 1; ++i)
    {
        int global_i = overlap_start + i;
        double x = x0 + global_i * hx;

        for (int j = 1; j < Ny - 1; ++j)
        {
            double y = y0 + j * hy;

            for (int k = 1; k < Nz - 1; ++k)
            {
                double z = z0 + k * hz;
                double exact = phi_solution(x, y, z);
                double error = fabs(phi[i][j][k] - exact);
                if (error > local_error)
                    local_error = error;
            }
        }
    }

    double global_error;
    MPI_Reduce(&local_error, &global_error, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        std::cout << "Max error: " << global_error << std::endl;
    }

    delete[] send_down;
    delete[] recv_down;
    delete[] send_up;
    delete[] recv_up;
    delete[] phi_part_result;

    MPI_Finalize();
    return 0;
}