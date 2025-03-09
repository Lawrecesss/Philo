/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshein <lshein@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:05:02 by lshein            #+#    #+#             */
/*   Updated: 2025/03/08 09:27:49 by lshein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (sign * result);
}

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_status(t_philo *philo, char *status)
{
	long	current_time;

	pthread_mutex_lock(&philo->table->write_mutex);
	pthread_mutex_lock(&philo->table->dead_mutex);
	if (philo->table->someone_died == true)
	{
		pthread_mutex_unlock(&philo->table->dead_mutex);
		pthread_mutex_unlock(&philo->table->write_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->table->dead_mutex);
	current_time = get_time() - philo->table->start_time;
	printf("%ld %d %s\n", current_time, philo->id, status);
	pthread_mutex_unlock(&philo->table->write_mutex);
}

void	ft_sleep(long ms)
{
	long	start_time;
	long	current_time;

	start_time = get_time();
	while (1)
	{
		current_time = get_time();
		if (current_time - start_time >= ms)
			break ;
		usleep(500);
	}
}

void	cleanup(t_philo *philos, t_fork *forks, t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_philos)
	{
		pthread_mutex_destroy(&philos[i].meal_mutex);
		i++;
	}
	i = 0;
	while (i < table->num_philos)
	{
		pthread_mutex_destroy(&forks[i].mutex);
		i++;
	}
	pthread_mutex_destroy(&table->write_mutex);
	pthread_mutex_destroy(&table->dead_mutex);
	pthread_mutex_destroy(&table->meal_mutex);
	free(philos);
	free(forks);
}
