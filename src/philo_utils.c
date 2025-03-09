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
