/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 12:26:14 by mel-asla          #+#    #+#             */
/*   Updated: 2026/04/18 14:17:35 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	start_coder_threads(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->coder_count)
	{
		if (pthread_create(&table->coders[i].thread, NULL, coder_routine,
				&table->coders[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}

static void	join_coder_threads(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->coder_count)
	{
		pthread_join(table->coders[i].thread, NULL);
		i++;
	}
}

int	start_simulation(t_table *table)
{
	if (start_coder_threads(table) != 0)
		return (1);
	if (pthread_create(&table->monitor_thread, NULL,
			monitor_routine, table) != 0)
	{
		set_simulation_end(table, true);
		join_coder_threads(table);
		return (1);
	}
	pthread_join(table->monitor_thread, NULL);
	join_coder_threads(table);
	return (0);
}
