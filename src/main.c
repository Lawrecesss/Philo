/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshein <lshein@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 01:31:04 by lshein            #+#    #+#             */
/*   Updated: 2025/03/06 01:20:20 by lshein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int main(int argc, char **argv)
{
    t_data data;
    pthread_t monitor;
    int i;

    i = 0;
    // Initialize data from command line arguments
    if (!parse_args(argc, argv, &data))
    {
        fprintf(stderr, "Error: %s\n", ERROR_ARG);
        return (1);
    }
    while ( i < data.num_philos)
    {
        if (pthread_create(&data.philos[i].thread, NULL, philosopher_routine, &data.philos[i]) != 0)
        {
            fprintf(stderr, "Error: Failed to create philosopher thread\n");
            destroy_all(&data);
            return (1);
        }
        i++;
    }
    // Initialize simulation data structures
    if (!init_data(&data))
    {
        fprintf(stderr, "Error: %s\n", ERROR_MALLOC);
        return (1);
    }
    // Create monitor thread to check philosopher states
    if (pthread_create(&monitor, NULL, monitor_routine, &data) != 0)
    {
        fprintf(stderr, "Error: Failed to create monitor thread\n");
        destroy_all(&data);
        return (1);
    }
    i = 0;
    while (i < data.num_philos)
    {
        pthread_join(data.philos[i].thread, NULL);
        i++;
    }
    pthread_join(monitor, NULL);
    destroy_all(&data);
    return (0);
}
