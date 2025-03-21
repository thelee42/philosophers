/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelee <thelee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:00:11 by thealee           #+#    #+#             */
/*   Updated: 2025/03/16 12:43:31 by thelee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	are_philos_full(t_philo *philos)
{
	int	i;
	int	full_philo;

	if (philos[0].shared->min_meal == 0)
		return (0);
	full_philo = 0;
	i = 0;
	while (i < philos[0].shared->num_philos)
	{
		pthread_mutex_lock(&philos[i].shared->lock_m);
		if (philos[i].meals >= philos[i].shared->min_meal)
			full_philo++;
		pthread_mutex_unlock(&philos[i].shared->lock_m);
		i++;
	}
	if (full_philo == philos[0].shared->num_philos)
		return (1);
	return (0);
}

void	death_mark(t_philo *philo)
{
	write_lock(philo, "died");
	pthread_mutex_lock(&philo->shared->lock_d);
	philo->shared->is_dead = 1;
	pthread_mutex_unlock(&philo->shared->lock_d);
}

void	*thread_death_monitor(void *arg)
{
	t_philo	*philos;
	int		i;

	philos = (t_philo *)arg;
	while (!philos->shared->is_dead)
	{
		if (are_philos_full(philos))
			return (NULL);
		i = 0;
		while (i < philos[0].shared->num_philos)
		{
			pthread_mutex_lock(philos[i].lock_t);
			if (cur_time() - philos[i].last_eat > philos[i].shared->time_die)
			{
				pthread_mutex_unlock(philos[i].lock_t);
				death_mark(&philos[i]);
				return (NULL);
			}
			pthread_mutex_unlock(philos[i].lock_t);
			i++;
		}
	}
	return (NULL);
}
