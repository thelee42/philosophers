/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thealee <thealee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:00:00 by thealee           #+#    #+#             */
/*   Updated: 2025/03/12 23:07:55 by thealee          ###   ########.fr       */
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

int cur_time()
{
    struct timeval start_tv;
    int cur_time;

    gettimeofday(&start_tv, NULL);
    cur_time = (start_tv.tv_sec * 1000) + (start_tv.tv_usec / 1000);
    return (cur_time);
}

int timestamp(int start_tv)
{
    struct timeval cur;
    int cur_sec;

    gettimeofday(&cur, NULL);
    cur_sec = (cur.tv_sec * 1000) + (cur.tv_usec / 1000);
    return (cur_sec - start_tv);
}

void ft_msleep(int sleep)
{
    struct timeval start;
    struct timeval cur;

    gettimeofday(&start, NULL);
    gettimeofday(&cur, NULL);
    while ((cur.tv_sec - start.tv_sec) * 1000 + (cur.tv_usec - start.tv_usec) / 1000 <= sleep)
        gettimeofday(&cur, NULL);
}