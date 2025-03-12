/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thealee <thealee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:00:11 by thealee           #+#    #+#             */
/*   Updated: 2025/03/12 23:17:10 by thealee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int is_philo_full(t_philo *philos)
{
    if (!philos->shared->min_meal)
        return 0;
    pthread_mutex_lock(&philos->shared->lock_m);
    if (philos->meals >= philos->shared->min_meal)
    {
        pthread_mutex_unlock(&philos->shared->lock_m);
        return 1;
    }
    pthread_mutex_unlock(&philos->shared->lock_m);
    return 0;
}

int are_philos_full(t_philo *philos)
{
    int i;
    int full_philo;

    full_philo = 0;
    i = 0;
    while (i < philos[0].shared->num_philos)
    {
        full_philo += is_philo_full(&philos[i]);
        i++;
    }
    if (full_philo == philos[0].shared->num_philos)
        return 1;
    return 0;
}

void *thread_death_monitor(void *arg)
{
    t_philo *philos;
    int i;

    philos = (t_philo *)arg;
    while (1)
    {
        if (are_philos_full(philos))
            return NULL;
        i = 0;
        while (i < philos[0].shared->num_philos)
        {
            pthread_mutex_lock(&philos[i].shared->lock_t);
            if (timestamp(philos[i].last_eat) > philos[i].shared->time_die)
            {
                pthread_mutex_lock(&philos[i].shared->lock_d);
                printf("%d %d died\n", timestamp(philos[i].shared->start_tv), philos[i].name);
                philos[i].shared->is_dead = 1;
                pthread_mutex_unlock(&philos[i].shared->lock_d);
                pthread_mutex_unlock(&philos[i].shared->lock_t);
                return NULL;
            }
            pthread_mutex_unlock(&philos[i++].shared->lock_t);
            usleep(MONITOR);
        }
    }
}

int stop_philo(t_philo *philo)
{
    pthread_mutex_lock(&philo->shared->lock_d);
    if (philo->shared->is_dead == 1)
    {
        pthread_mutex_unlock(&philo->shared->lock_d);
        return 1;
    }
    pthread_mutex_unlock(&philo->shared->lock_d);
    if (is_philo_full(philo))
        return 1;
    return 0;
}