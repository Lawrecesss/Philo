/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshein <lshein@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 10:00:00 by lshein            #+#    #+#             */
/*   Updated: 2025/03/07 16:17:31 by lshein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_table	table;
	t_philo	*philos;
	t_fork	*forks;

	if (parse_args(argc, argv, &table))
	{
		init_table(&table);
		init_forks(&forks, table.num_philos);
		if (!init_philosophers(&philos, &table, forks))
		{
			printf("Error: Failed to initialize philosophers\n");
			free(forks);
			return (1);
		}
		if (!start_simulation(philos, &table))
		{
			printf("Error: Failed to start simulation\n");
			cleanup(philos, forks, &table);
			return (1);
		}
		cleanup(philos, forks, &table);
	}
	return (0);
}
