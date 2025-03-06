/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshein <lshein@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 02:09:24 by lshein            #+#    #+#             */
/*   Updated: 2025/03/06 01:18:12 by lshein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int parse_args(int argc, char **argv, t_data *data)
{
    if (argc < 5 || argc > 6)
        return (0);
    data->num_philos = ft_atoi(argv[1]);
    if (data->num_philos <= 0)
        return (0);
    data->time_to_die = ft_atoi(argv[2]);
    data->time_to_eat = ft_atoi(argv[3]);
    data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->num_must_eat = ft_atoi(argv[5]);
	else
		data->num_must_eat = -1;
	if (data->time_to_die < 0 || data->time_to_eat < 0 || 
        data->time_to_sleep < 0 || (argc == 6 && data->num_must_eat <= 0))
        return (0);

    return (1);
}

int init_data(t_data *data)
{
    if (pthread_mutex_init(&data->write_lock, NULL) != 0 ||
        pthread_mutex_init(&data->sim_lock, NULL) != 0)
        return (0);
    data->philos = malloc(sizeof(t_philo) * data->num_philos);
    data->forks = malloc(sizeof(t_fork) * data->num_philos);
    if (!data->philos || !data->forks)
        return (0);
    data->simulation_stop = 0;
    data->time.start_time = get_current_time();
    if (!init_forks(data) || !init_philosophers(data))
        return (0);
    return (1);
}

int init_forks(t_data *data)
{
    int i;

	i = 0;
	while (i < data->num_philos)
	{
        if (pthread_mutex_init(&data->forks[i].lock, NULL) != 0)
            return (0);
		i++;
	}
	return (1);
}

int init_philosophers(t_data *data)
{
    int i;

	i = 0;
	while (i < data->num_philos)
	{
        data->philos[i].id = i + 1;
        data->philos[i].data = data;
        data->philos[i].meals_eaten = 0;
        data->philos[i].last_meal_time = data->time.start_time;
        data->philos[i].left_fork = &data->forks[i];
        data->philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
        if (pthread_mutex_init(&data->philos[i].meal_lock, NULL) != 0)
            return (0);
        // if (pthread_create(&data->philos[i].thread, NULL, 
        //                    philosopher_routine, &data->philos[i]) != 0)
        //     return (0);
		i++;
	}
	return (1);
}

void destroy_all(t_data *data)
{
    int i;

	i = 0;
	while (i < data->num_philos)
	{
        pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	i = 0;
	while (i < data->num_philos)
    {
        pthread_mutex_destroy(&data->forks[i].lock);
        pthread_mutex_destroy(&data->philos[i].meal_lock);
		i++;
	}
    pthread_mutex_destroy(&data->write_lock);
    pthread_mutex_destroy(&data->sim_lock);
    free(data->philos);
    free(data->forks);
}
