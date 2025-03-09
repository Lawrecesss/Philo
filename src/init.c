/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshein <lshein@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:06:15 by lshein            #+#    #+#             */
/*   Updated: 2025/03/08 09:37:14 by lshein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	parse_args(int argc, char **argv, t_table *table)
{
	if (argc != 5 && argc != 6)
		return (0);
	table->num_philos = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		table->num_meals = ft_atoi(argv[5]);
	else
		table->num_meals = -1;
	if (table->num_philos <= 0 || table->time_to_die <= 0
		|| table->time_to_eat <= 0 || table->time_to_sleep <= 0 || (argc == 6
			&& table->num_meals <= 0))
		return (0);
	return (1);
}

void	init_table(t_table *table)
{
	table->someone_died = false;
	table->all_ate = false;
	table->start_time = get_time();
	pthread_mutex_init(&table->write_mutex, NULL);
	pthread_mutex_init(&table->dead_mutex, NULL);
	pthread_mutex_init(&table->meal_mutex, NULL);
}

int	init_forks(t_fork **forks, int num_forks)
{
	int	i;

	*forks = malloc(sizeof(t_fork) * num_forks);
	if (!(*forks))
		return (0);
	i = 0;
	while (i < num_forks)
	{
		if (pthread_mutex_init(&(*forks)[i].mutex, NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&(*forks)[i].mutex);
			free(*forks);
			return (0);
		}
		(*forks)[i].id = i + 1;
		i++;
	}
	return (1);
}

int	init_philosophers(t_philo **philos, t_table *table, t_fork *forks)
{
	int	i;

	*philos = malloc(sizeof(t_philo) * table->num_philos);
	if (!(*philos))
		return (0);
	i = 0;
	while (i < table->num_philos)
	{
		(*philos)[i].id = i + 1;
		(*philos)[i].meals_eaten = 0;
		(*philos)[i].last_meal_time = get_time();
		(*philos)[i].table = table;
		(*philos)[i].left_fork = &forks[i];
		(*philos)[i].right_fork = &forks[(i + 1) % table->num_philos];
		if (pthread_mutex_init(&(*philos)[i].meal_mutex, NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&(*philos)[i].meal_mutex);
			free(*philos);
			return (0);
		}
		i++;
	}
	return (1);
}

int	start_simulation(t_philo *philos, t_table *table)
{
	int			i;
	pthread_t	monitor;

	i = 0;
	while (i < table->num_philos)
	{
		if (pthread_create(&philos[i].thread, NULL, philo_routine,
				&philos[i]) != 0)
		{
			while (--i >= 0)
				pthread_detach(philos[i].thread);
			return (0);
		}
		i++;
	}
	if (pthread_create(&monitor, NULL, monitor_routine, philos) != 0)
	{
		detach_all_philos(table, philos);
		return (0);
	}
	join_all_philos(table, philos);
	pthread_join(monitor, NULL);
	return (1);
}
