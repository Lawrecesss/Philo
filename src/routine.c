/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshein <lshein@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:09:01 by lshein            #+#    #+#             */
/*   Updated: 2025/03/08 09:38:00 by lshein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	*monitor_routine(void *arg)
{
	t_philo	*philos;
	int		i;
	long	current_time;
	int		all_ate_count;

	philos = (t_philo *)arg;
	while (1)
	{
		i = 0;
		all_ate_count = 0;
		while (i < philos->table->num_philos)
		{
			pthread_mutex_lock(&philos[i].meal_mutex);
			current_time = get_time();
			if (current_time
				- philos[i].last_meal_time > philos->table->time_to_die)
			{
				print_status(&philos[i], "died");
				pthread_mutex_lock(&philos->table->dead_mutex);
				philos->table->someone_died = true;
				pthread_mutex_unlock(&philos->table->dead_mutex);
				pthread_mutex_unlock(&philos[i].meal_mutex);
				return (NULL);
			}
			if (philos[i].meals_eaten >= philos->table->num_meals
				&& philos->table->num_meals > 0)
				all_ate_count++;
			pthread_mutex_unlock(&philos[i].meal_mutex);
			i++;
		}
		if (all_ate_count == philos->table->num_philos
			&& philos->table->num_meals > 0)
		{
			pthread_mutex_lock(&philos->table->meal_mutex);
			philos->table->all_ate = true;
			pthread_mutex_unlock(&philos->table->meal_mutex);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

void	philo_eat(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->right_fork->mutex);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->left_fork->mutex);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->left_fork->mutex);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->right_fork->mutex);
		print_status(philo, "has taken a fork");
	}
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	precise_sleep(philo->table->time_to_eat);
	pthread_mutex_unlock(&philo->left_fork->mutex);
	pthread_mutex_unlock(&philo->right_fork->mutex);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		precise_sleep(philo->table->time_to_eat / 2);
	while (1)
	{
		pthread_mutex_lock(&philo->table->dead_mutex);
		if (philo->table->someone_died)
		{
			pthread_mutex_unlock(&philo->table->dead_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->table->dead_mutex);
		pthread_mutex_lock(&philo->table->meal_mutex);
		if (philo->table->all_ate)
		{
			pthread_mutex_unlock(&philo->table->meal_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->table->meal_mutex);
		if (philo->table->num_philos == 1)
		{
			pthread_mutex_lock(&philo->left_fork->mutex);
			print_status(philo, "has taken a fork");
			precise_sleep(philo->table->time_to_die);
			print_status(philo, "died");
			pthread_mutex_unlock(&philo->left_fork->mutex);
			break ;
		}
		philo_eat(philo);
		print_status(philo, "is sleeping");
		precise_sleep(philo->table->time_to_sleep);
		print_status(philo, "is thinking");
	}
	return (NULL);
}
