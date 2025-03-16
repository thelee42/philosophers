/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thealee <thealee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:00:07 by thealee           #+#    #+#             */
/*   Updated: 2025/03/16 11:38:23 by thealee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void philo_fork(t_philo *philo)
{
    if (philo->name % 2) // impair
    {
        pthread_mutex_lock(philo->fork_r);
        write_lock(philo, "has taken a fork");
        pthread_mutex_lock(philo->fork_l);
        write_lock(philo, "has taken a fork");
    }
    else
    {
        pthread_mutex_lock(philo->fork_l);
        write_lock(philo, "has taken a fork");
        pthread_mutex_lock(philo->fork_r);
        write_lock(philo, "has taken a fork");
    }
}

void philo_eat(t_philo *philo)
{
    pthread_mutex_lock(philo->lock_t);
    philo->last_eat = cur_time();
    pthread_mutex_unlock(philo->lock_t);
    write_lock(philo, "is eating");
    pthread_mutex_lock(&philo->shared->lock_m);
    philo->meals++;
    pthread_mutex_unlock(&philo->shared->lock_m);
    precise_usleep(philo->shared->time_eat * 1000);
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
    write_lock(philo, "is sleeping");
    precise_usleep(philo->shared->time_sleep * 1000);
}
void philo_think(t_philo *philo)
{
    long long rest;
    
    write_lock(philo, "is thinking");
    if(philo->shared->num_philos % 2)
    {
        pthread_mutex_lock(philo->lock_t);
        rest = cur_time() - philo->last_eat;
        pthread_mutex_unlock(philo->lock_t);
        // if (philo->shared->time_eat * 2 < rest && rest < philo->shared->time_eat * 3)
        //     usleep(philo->shared->time_eat);

        // 너무 오래 기다린 철학자는 대기 없이 바로 턴을 얻도록 함
        if (rest > philo->shared->time_die / 2)
            return;
        // 일반적인 경우: 두 턴을 기다리는 철학자가 지나치게 오래 기다리지 않도록 함
        if (rest >= philo->shared->time_eat * 2)
            usleep(philo->shared->time_eat / 2);
        }
}


void *philosophers(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    if (philo->fork_r == philo->fork_l)
        return NULL;
    if (!philo->name % 2)
        usleep(1000);
    while (!stop_philo(philo))
    {
        philo_fork(philo);
        philo_eat(philo);
        philo_sleep(philo);
        philo_think(philo);
    }
    return NULL;
}


