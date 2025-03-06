/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshein <lshein@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 01:43:10 by lshein            #+#    #+#             */
/*   Updated: 2025/03/06 00:50:29 by lshein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

long get_current_time(void)
{
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void philosopher_sleep(long milliseconds)
{
    long start = get_current_time();
    
    while (get_current_time() - start < milliseconds)
        usleep(100);
}

void log_state(t_philo *philo, char *state)
{
    long timestamp;

    // Don't log if simulation is stopped
    if (is_simulation_stopped(philo->data))
        return;

    timestamp = get_current_time() - philo->data->time.start_time;

    // Use mutex to ensure atomic printing
    pthread_mutex_lock(&philo->data->write_lock);
    if (!is_simulation_stopped(philo->data))
        printf("%ld %d %s\n", timestamp, philo->id, state);
    pthread_mutex_unlock(&philo->data->write_lock);
}

int is_simulation_stopped(t_data *data)
{
    int stop;

    pthread_mutex_lock(&data->sim_lock);
    stop = data->simulation_stop;
    pthread_mutex_unlock(&data->sim_lock);

    return (stop);
}

void set_simulation_stop(t_data *data)
{
    pthread_mutex_lock(&data->sim_lock);
    data->simulation_stop = 1;
    pthread_mutex_unlock(&data->sim_lock);
}

void update_last_meal_time(t_philo *philo)
{
    pthread_mutex_lock(&philo->meal_lock);
    philo->last_meal_time = get_current_time() - philo->data->time.start_time;
    pthread_mutex_unlock(&philo->meal_lock);
}
