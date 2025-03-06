/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshein <lshein@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 01:31:20 by lshein            #+#    #+#             */
/*   Updated: 2025/03/05 01:38:52 by lshein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Time handling
#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

# define ERROR_ARG "Invalid arguments"
# define ERROR_MALLOC "Memory allocation failed"
# define ERROR_THREAD "Thread creation failed"

typedef struct s_time {
    long    start_time;
} t_time;

typedef struct s_fork {
    pthread_mutex_t lock;
} t_fork;

typedef struct s_data {
    int             num_philos;
    int             time_to_die;
    int             time_to_eat;
    int             time_to_sleep;
    int             num_must_eat;
    int             simulation_stop;
    t_time          time;
    struct s_philo  *philos;
    t_fork          *forks;
    pthread_mutex_t write_lock;
    pthread_mutex_t sim_lock;
} t_data;

typedef struct s_philo {
    int             id;
    int             meals_eaten;
    long            last_meal_time;
    pthread_t       thread;
    t_fork          *left_fork;
    t_fork          *right_fork;
    t_data          *data;
    pthread_mutex_t meal_lock;
} t_philo;

// Initialization functions
int     parse_args(int argc, char **argv, t_data *data);
int     init_data(t_data *data);
int     init_forks(t_data *data);
int     init_philosophers(t_data *data);
void    destroy_all(t_data *data);

// Routine functions
void    *philosopher_routine(void *arg);
int     philosopher_eat(t_philo *philo);
void    wait_for_all_threads(t_data *data);

// Utility functions
long    get_current_time(void);
void    philosopher_sleep(long milliseconds);
void    log_state(t_philo *philo, char *state);
int     is_simulation_stopped(t_data *data);
void    set_simulation_stop(t_data *data);
void    update_last_meal_time(t_philo *philo);
int     all_philosophers_ate_enough(t_data *data);

// Monitoring functions
void    *monitor_routine(void *arg);

// ft_utils
int ft_atoi(const char *nptr);
#endif
