/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thealee <thealee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 22:59:55 by thealee           #+#    #+#             */
/*   Updated: 2025/03/12 23:15:33 by thealee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int init_philo(t_shared *shared, t_philo **philos)
{
    int i;

    *philos = malloc(sizeof(t_philo) * shared->num_philos);
    if (!*philos)
        return 1;
    memset(*philos, 0, sizeof(t_philo) * shared->num_philos);
    i = 0;
    while (i < shared->num_philos)
    {
        (*philos)[i].name = i + 1;
        (*philos)[i].meals = 0;
        (*philos)[i].last_eat = cur_time();
        (*philos)[i].fork_r = &shared->forks[(i + 1) % shared->num_philos];
        (*philos)[i].fork_l = &shared->forks[i];
        (*philos)[i].shared = shared;
        i++;
    }
    return 0;
}

int init_mutex(t_shared *shared)
{
    int i;

    shared->forks = malloc(sizeof(pthread_mutex_t) * shared->num_philos);
    if (!shared->forks)
        return 1;
    i = 0;
    while (i < shared->num_philos)
        pthread_mutex_init(&shared->forks[i++], NULL);
    pthread_mutex_init(&shared->lock_d, NULL);
    pthread_mutex_init(&shared->lock_t, NULL);
    pthread_mutex_init(&shared->lock_m, NULL);
    return 0;
}

int parse_arg(int ac, char **argv, t_shared *shared)
{
    if (ac < 5)
        return (printf("Missing some argument(s)\n"));
    if (ac > 6)
        return (printf("Too many arguments\n"));
    if (is_all_digit(argv))
        return (printf("Invalide argument(s)\n"));
    if (!ft_atoi(argv[1]))
        return (printf("No philosopher on the table\n"));
    shared->num_philos = ft_atoi(argv[1]);
    shared->start_tv = cur_time();
    shared->time_die = ft_atoi(argv[2]);
    shared->time_eat = ft_atoi(argv[3]);
    shared->time_sleep = ft_atoi(argv[4]);
    if (ac == 6)
        shared->min_meal = ft_atoi(argv[5]);
    else
        shared->min_meal = 0;
    shared->is_dead = 0;
    return 0;
}

int threads(t_shared *shared, t_philo *philos)
{
    pthread_t *threads;
    pthread_t death_monitor;
    int i;

    threads = malloc(sizeof(pthread_t) * shared->num_philos);
    if (!threads)
        return 1;
    i = 0;
    while (i < shared->num_philos)
    {
        pthread_create(&threads[i], NULL, philosophers, &philos[i]);
        i++;
    }
    pthread_create(&death_monitor, NULL, thread_death_monitor, philos);
    pthread_join(death_monitor, NULL);
    i = 0;
    while (i < shared->num_philos)
        pthread_join(threads[i++], NULL);
    destroy_mutex(shared);
    free(threads);
    free(shared->forks);
    free(philos);
    return 0;
}

int main(int ac, char **argv)
{
    t_shared commun;
    t_philo *philos;

    // parse arguments
    if (parse_arg(ac, argv, &commun))
        return -2;
    // init mutex
    if (init_mutex(&commun))
        return -2;
    // init + pholos
    if (init_philo(&commun, &philos))
        return -2;
    // run threads
    if (threads(&commun, philos))
        return -2;
    return 0;
}