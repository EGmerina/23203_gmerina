#include <stdio.h>
#include <sys/times.h>
#include <unistd.h>

#define PI 3.14159265358979323846

double decreaseToCircle(const double num)
{
    double res = num;
    if (num > 0)
    {
        while (res > PI * 2)
        {
            res -= PI * 2;
        }
    }
    else
    {
        while (res < PI * 2)
        {
            res += PI * 2;
        }
    }
    return res;
}

double Sin(const double x, const unsigned long long N)
{
    double res = x;
    double summand = x;
    for (unsigned long long i = 2; i < N + 1; ++i)
    {
        char sign = (-1) * (i % 2 == 0) + 1 * (i % 2 != 0);
        summand = summand * (x * x) / ((2 * i - 1) * (2 * i - 2));
        res += (sign * summand);
    }
    return res;
}

int main(void)
{
    double x;
    unsigned long long N;
    printf("enter x : ");
    scanf("%lf", &x);
    if (x > PI * 2 || x < PI * (-2))
    {
        x = decreaseToCircle(x);
    }
    printf("enter N : ");
    scanf("%llu", &N);
    struct tms start, end;
    long clocks_per_sec = sysconf(_SC_CLK_TCK);
    long clocks;
    times(&start);
    double res = Sin(x, N);
    times(&end);
    printf("result : %.15lf\n", res);
    clocks = end.tms_utime - start.tms_utime;
    printf("Time taken: %lf sec.\n", (double)clocks / clocks_per_sec);
}
