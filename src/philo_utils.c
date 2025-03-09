/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshein <lshein@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 09:28:40 by lshein            #+#    #+#             */
/*   Updated: 2025/03/08 09:37:44 by lshein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	detach_all_philos(t_table *table, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < table->num_philos)
	{
		pthread_detach(philos[i].thread);
		i++;
	}
}

void	join_all_philos(t_table *table, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < table->num_philos)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
}

int	check_anyone_dead(t_philo *philos, int i)
{
	long	current_time;

	current_time = get_time();
	if (current_time
		- philos[i].last_meal_time > philos->table->time_to_die)
	{
		print_status(&philos[i], "died");
		pthread_mutex_lock(&philos->table->dead_mutex);
		philos->table->someone_died = true;
		pthread_mutex_unlock(&philos->table->dead_mutex);
		pthread_mutex_unlock(&philos[i].meal_mutex);
		return (1);
	}
	return (0);
}

int	check_all_ate(t_philo *philos, int all_ate_count)
{
	if (all_ate_count == philos->table->num_philos
		&& philos->table->num_meals > 0)
	{
		pthread_mutex_lock(&philos->table->meal_mutex);
		philos->table->all_ate = true;
		pthread_mutex_unlock(&philos->table->meal_mutex);
		return (1);
	}
	return (0);
}

int	check_stop_conditions(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->dead_mutex);
	if (philo->table->someone_died)
	{
		pthread_mutex_unlock(&philo->table->dead_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->table->dead_mutex);
	pthread_mutex_lock(&philo->table->meal_mutex);
	if (philo->table->all_ate)
	{
		pthread_mutex_unlock(&philo->table->meal_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->table->meal_mutex);
	if (philo->table->num_philos == 1)
	{
		pthread_mutex_lock(&philo->left_fork->mutex);
		print_status(philo, "has taken a fork");
		ft_sleep(philo->table->time_to_die);
		pthread_mutex_unlock(&philo->left_fork->mutex);
		return (1);
	}
	return (0);
}
