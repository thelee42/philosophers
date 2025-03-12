/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thealee <thealee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:03:03 by thealee           #+#    #+#             */
/*   Updated: 2025/03/12 23:17:38 by thealee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>

#define MONITOR 5000

typedef struct s_shared
{
    int num_philos;
    int start_tv;
    int time_die;
    int time_eat;
    int time_sleep;
    int min_meal;
    int is_dead;
    pthread_mutex_t *forks;
    pthread_mutex_t lock_d;
    pthread_mutex_t lock_t;
    pthread_mutex_t lock_m;
} t_shared;

typedef struct s_philo
{
    int name;
    int meals;
    int turn;
    int last_eat;
    pthread_mutex_t *fork_r;
    pthread_mutex_t *fork_l;
    t_shared *shared;
} t_philo;

// main.c
int main(int ac, char **argv);
int parse_arg(int ac, char **argv, t_shared *shared);
int init_mutex(t_shared *shared);
int threads(t_shared *shared, t_philo *philos);
int init_philo(t_shared *shared, t_philo **philos);

// thread.c
void destroy_mutex(t_shared *shared);
void *philosophers(void *arg);
void philo_sleep(t_philo *philo);
void philo_eat(t_philo *philo);
void philo_fork(t_philo *philo);

// monitor.c
void *thread_death_monitor(void *arg);
int are_philos_full(t_philo *philos);
int is_philo_full(t_philo *philos);
int stop_philo(t_philo *philo);

// utils.c
int ft_atoi(const char *str);
int is_all_digit(char **argv);
int cur_time();
int timestamp(int start_tv);
void ft_msleep(int sleep);

#endif