/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thealee <thealee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 22:59:55 by thealee           #+#    #+#             */
/*   Updated: 2025/03/13 10:31:57 by thealee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void    destroy_mutex(t_shared *shared)
{
    int i;

    i = 0;
    while (i < shared->num_philos)
        pthread_mutex_destroy(&shared->forks[i++]);
    pthread_mutex_destroy(&shared->lock_d);
    pthread_mutex_destroy(&shared->lock_t);
    pthread_mutex_destroy(&shared->lock_m);
    pthread_mutex_destroy(&shared->lock_p);
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
    if (!philo->shared->min_meal)
        return 0;
    pthread_mutex_lock(&philo->shared->lock_m);
    if (philo->meals >= philo->shared->min_meal)
    {
        pthread_mutex_unlock(&philo->shared->lock_m);
        return 1;
    }
    pthread_mutex_unlock(&philo->shared->lock_m);
    return 0;
}

