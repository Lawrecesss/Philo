/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshein <lshein@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 01:31:20 by lshein            #+#    #+#             */
/*   Updated: 2025/03/08 09:36:55 by lshein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_fork
{
	pthread_mutex_t	mutex;
	int				id;
}					t_fork;

typedef struct s_table
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_meals;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	dead_mutex;
	pthread_mutex_t	meal_mutex;
	bool			someone_died;
	bool			all_ate;
	long			start_time;
}					t_table;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	pthread_t		thread;
	t_fork			*left_fork;
	t_fork			*right_fork;
	t_table			*table;
	pthread_mutex_t	meal_mutex;
}					t_philo;

// init.c
int					parse_args(int argc, char **argv, t_table *table);
void				init_table(t_table *table);
int					init_forks(t_fork **forks, int num_forks);
int					init_philosophers(t_philo **philos, t_table *table,
						t_fork *forks);
int					start_simulation(t_philo *philos, t_table *table);

// utils.c
int					ft_atoi(const char *str);
long				get_time(void);
void				print_status(t_philo *philo, char *status);
void				cleanup(t_philo *philos, t_fork *forks, t_table *table);

// routine.c
void				*philo_routine(void *arg);
void				philo_eat(t_philo *philo);
void				*monitor_routine(void *arg);
void				ft_sleep(long ms);

// philo_utils.c
void				detach_all_philos(t_table *table, t_philo *philos);
void				join_all_philos(t_table *table, t_philo *philos);
int					check_anyone_dead(t_philo *philos, int i);
int					check_all_ate(t_philo *philos, int all_ate_count);
int					check_stop_conditions(t_philo *philo);
#endif
