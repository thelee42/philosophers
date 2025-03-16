/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thealee <thealee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:00:00 by thealee           #+#    #+#             */
/*   Updated: 2025/03/16 11:19:51 by thealee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int ft_atoi(const char *str)
{
    int i;
    int c;
    int m;

    if (!str)
        return 0;
    i = 0;
    c = 0;
    m = 1;
    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v' || str[i] == '\r' || str[i] == '\f')
        i++;
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            m = m * -1;
        i++;
    }
    while (str[i] >= '0' && str[i] <= '9')
    {
        c = (c * 10) + (str[i] - '0');
        i++;
    }
    return (c * m);
}

int is_all_digit(char **argv)
{
    int i;
    int j;

    i = 1;
    while (argv[i])
    {
        j = 0;
        while (argv[i][j])
        {
            if (argv[i][j] < '0' || argv[i][j] > '9')
                return 1;
            j++;
        }
        i++;
    }
    return 0;
}

long long cur_time()
{
    struct timeval start_tv;
    long long cur_time;

    gettimeofday(&start_tv, NULL);
    cur_time = (start_tv.tv_sec * 1000) + (start_tv.tv_usec / 1000);
    return (cur_time);
}

// void ft_msleep(int sleep)
// {
//     struct timeval start;
//     struct timeval cur;

//     gettimeofday(&start, NULL);
//     gettimeofday(&cur, NULL);
//     while ((cur.tv_sec - start.tv_sec) * 1000 + (cur.tv_usec - start.tv_usec) / 1000 <= sleep)
//         gettimeofday(&cur, NULL);
// }


// void precise_usleep(long long usec) 
// {
//     struct timeval start, current;
//     long long elapsed;
//     long long rem;

//     // 시작 시간 기록
//     gettimeofday(&start, NULL);

//     do 
//     {
//         // 현재 시간 기록
//         gettimeofday(&current, NULL);

//         // 경과 시간 계산 (초와 마이크로초 차이 계산)
//         elapsed = (current.tv_sec - start.tv_sec) * 1000000L + (current.tv_usec - start.tv_usec);

//         // 남은 시간 계산
//         rem = usec - elapsed;

//         // 남은 시간이 충분히 크면 대기
//         if (rem > 1000) 
//             usleep(rem / 2);

//     } while (elapsed < usec);
// }


void precise_usleep(long long usec) 
{
    struct timeval start;
    struct timeval current;
    long long elapsed;
    long long rest;

    elapsed = 0;
    gettimeofday(&start, NULL);
    while (elapsed < usec) 
    {
        gettimeofday(&current, NULL);
        elapsed = (current.tv_sec - start.tv_sec) * 1000000L + (current.tv_usec - start.tv_usec);
        rest = usec - elapsed;

        if (rest > 1000)
            usleep(rest / 2);
    }
}
