/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshein <lshein@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 01:59:01 by lshein            #+#    #+#             */
/*   Updated: 2025/03/05 01:39:44 by lshein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void *monitor_routine(void *arg)
{
    t_data *data = (t_data *)arg;
    int i;
    long current_time;

	i = 0;
	wait_for_all_threads(data);
	while (!is_simulation_stopped(data))
    {
        while (i < data->num_philos)
        {
            current_time = get_current_time() - data->time.start_time;
            // Lock meal data to check last meal time
            pthread_mutex_lock(&data->philos[i].meal_lock);
            // Check if philosopher has starved
            if (current_time - data->philos[i].last_meal_time > data->time_to_die)
            {
                log_state(&data->philos[i], "died");
                set_simulation_stop(data);
                pthread_mutex_unlock(&data->philos[i].meal_lock);
                return (NULL);
            }
            pthread_mutex_unlock(&data->philos[i].meal_lock);
            // Optional: Check if all philosophers have eaten enough
            if (data->num_must_eat > 0 && all_philosophers_ate_enough(data))
            {
                set_simulation_stop(data);
                return (NULL);
            }
			i++;
		}
		philosopher_sleep(1);
    }
    return (NULL);
}

int all_philosophers_ate_enough(t_data *data)
{
    int i;

	i = 0;
	while (i < data->num_philos)
	{
        pthread_mutex_lock(&data->philos[i].meal_lock);
        if (data->philos[i].meals_eaten < data->num_must_eat)
        {
            pthread_mutex_unlock(&data->philos[i].meal_lock);
            return (0);
        }
        pthread_mutex_unlock(&data->philos[i].meal_lock);
		i++;
	}
	return (1);
}
