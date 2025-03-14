/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thealee <thealee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:00:07 by thealee           #+#    #+#             */
/*   Updated: 2025/03/13 10:20:17 by thealee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void philo_fork(t_philo *philo)
{
    if (philo->name % 2) // impair
    {
        pthread_mutex_lock(philo->fork_r);
        pthread_mutex_lock(philo->fork_l);
    }
    else
    {
        pthread_mutex_lock(philo->fork_l);
        pthread_mutex_lock(philo->fork_r);
    }
}

void philo_eat(t_philo *philo)
{
    pthread_mutex_lock(&philo->shared->lock_p);
    printf("%d %d has taken a fork\n", timestamp(philo->shared->start_tv), philo->name);
    pthread_mutex_unlock(&philo->shared->lock_p);
    pthread_mutex_lock(&philo->shared->lock_t);
    philo->last_eat = cur_time();
    pthread_mutex_unlock(&philo->shared->lock_t);
    pthread_mutex_lock(&philo->shared->lock_p);
    printf("%d %d is eating\n", timestamp(philo->shared->start_tv), philo->name);
    pthread_mutex_unlock(&philo->shared->lock_p);
    pthread_mutex_lock(&philo->shared->lock_m);
    philo->meals++;
    pthread_mutex_unlock(&philo->shared->lock_m);
    ft_msleep(philo->shared->time_eat);
    // pthread_mutex_unlock(philo->fork_r);
    // pthread_mutex_unlock(philo->fork_l);
    if (philo->name % 2) // impair
    {
        pthread_mutex_unlock(philo->fork_r);
        pthread_mutex_unlock(philo->fork_l);
    }
    else
    {
        pthread_mutex_unlock(philo->fork_l);
        pthread_mutex_unlock(philo->fork_r);
    }
}

void philo_sleep(t_philo *philo)
{
    pthread_mutex_lock(&philo->shared->lock_p);
    printf("%d %d is sleeping\n", cur_time() - philo->shared->start_tv, philo->name);
    pthread_mutex_unlock(&philo->shared->lock_p);
    ft_msleep(philo->shared->time_sleep);
}
void philo_think(t_philo *philo)
{
    pthread_mutex_lock(&philo->shared->lock_p);
    printf("%d %d is thinking\n", timestamp(philo->shared->start_tv), philo->name);
    pthread_mutex_unlock(&philo->shared->lock_p);
}


// add turn check
void *philosophers(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    while (1)
    {
        if (philo->fork_r == philo->fork_l) // one fork
            return NULL;
        if (stop_philo(philo))
            return NULL;
        philo_fork(philo);
        if (stop_philo(philo))
        {
            pthread_mutex_unlock(philo->fork_r);
            pthread_mutex_unlock(philo->fork_l);
            return NULL;
        }
        philo_eat(philo);
        if (stop_philo(philo))
            return NULL;
        philo_sleep(philo);
        if (stop_philo(philo))
            return NULL;
        philo_think(philo);
    }
}
