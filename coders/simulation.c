/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 12:11:36 by mel-asla          #+#    #+#             */
/*   Updated: 2026/05/02 17:22:01 by mel-asla         ###   ########.fr       */
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
		{
			set_simulation_end(table, true);
			pthread_mutex_lock(&table->start_mutex);
			table->start_released = true;
			pthread_cond_broadcast(&table->start_cond);
			pthread_mutex_unlock(&table->start_mutex);
			while (--i >= 0)
				pthread_join(table->coders[i].thread, NULL);
			return (1);
		}
		i++;
	}
	if (table->coder_count != i)
		return (1);
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
	pthread_mutex_lock(&table->start_mutex);
	table->start_released = true;
	pthread_cond_broadcast(&table->start_cond);
	pthread_mutex_unlock(&table->start_mutex);
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
