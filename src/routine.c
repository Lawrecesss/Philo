/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshein <lshein@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 01:57:35 by lshein            #+#    #+#             */
/*   Updated: 2025/03/06 01:02:26 by lshein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void wait_for_all_threads(t_data *data)
{
    // Ensure all threads are ready before starting
    while (!is_simulation_stopped(data)) 
        usleep(10);
}

int philosopher_eat(t_philo *philo)
{
    // Prevent deadlock by having even/odd philosophers pick up forks differently
    if (is_simulation_stopped(philo->data))
        return (0);

    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(&philo->left_fork->lock);
        log_state(philo, "has taken a fork");
        pthread_mutex_lock(&philo->right_fork->lock);
        log_state(philo, "has taken a fork");
    }
    else
    {
        pthread_mutex_lock(&philo->right_fork->lock);
        log_state(philo, "has taken a fork");
        pthread_mutex_lock(&philo->left_fork->lock);
        log_state(philo, "has taken a fork");
    }

    // Eating state
    log_state(philo, "is eating");
    update_last_meal_time(philo);
    philosopher_sleep(philo->data->time_to_eat);

    // Increment meal count
    pthread_mutex_lock(&philo->meal_lock);
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->meal_lock);

    // Release forks
    pthread_mutex_unlock(&philo->left_fork->lock);
    pthread_mutex_unlock(&philo->right_fork->lock);

    return (1);
}

void *philosopher_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    // Wait for all threads to start
    wait_for_all_threads(philo->data);

    // If philosopher ID is odd, wait slightly to prevent deadlock
    if (philo->id % 2 == 0)
		philosopher_sleep(1);

    // Main philosopher loop
    while (!is_simulation_stopped(philo->data))
    {
        // Try to eat
        if (!philosopher_eat(philo))
            break;

        // Sleep state
        log_state(philo, "is sleeping");
        philosopher_sleep(philo->data->time_to_sleep);

        // Think state
        log_state(philo, "is thinking");
    }

    return (NULL);
}
