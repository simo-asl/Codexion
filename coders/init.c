/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 10:17:06 by mel-asla          #+#    #+#             */
/*   Updated: 2026/05/01 17:58:07 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_coders(t_table *table)
{
	int		i;
	t_coder	*coder;

	table->coders = (t_coder *)malloc(sizeof(t_coder) * table->coder_count);
	if (!table->coders)
		return (1);
	i = 0;
	while (i < table->coder_count)
	{
		coder = &table->coders[i];
		coder->id = i + 1;
		coder->table = table;
		coder->left_dongle = NULL;
		coder->right_dongle = NULL;
		coder->last_compile_start_ms = table->start_time_ms;
		coder->last_round = -1;
		coder->compiles_in_round = 0;
		coder->compiles_done = 0;
		coder->in_wait_queue = false;
		coder->state = STATE_IDLE;
		i++;
	}
	return (0);
}

int	init_dongles(t_table *table)
{
	int	i;

	table->dongles = (t_dongle *)malloc(sizeof(t_dongle) * table->coder_count);
	if (!table->dongles)
		return (1);
	i = 0;
	while (i < table->coder_count)
	{
		table->dongles[i].in_use = false;
		table->dongles[i].available_at_ms = table->start_time_ms;
		if (pthread_mutex_init(&table->dongles[i].mutex, NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&table->dongles[i].mutex);
			return (1);
		}
		i++;
	}
	return (0);
}

int	init_table(t_table *table, int argc, char **argv)
{
	memset(table, 0, sizeof(*table));
	if (parse_args(table, argc, argv) != 0)
		return (1);
	if (init_mutexes(table) != 0)
		return (1);
	table->start_time_ms = get_timestamp_ms();
	table->simulation_end = false;
	table->start_released = false;
	table->next_sequence = 0;
	table->round = 0;
	table->completed_in_round = 0;
	if (init_coders(table) != 0)
		return (1);
	if (init_dongles(table) != 0)
		return (1);
	if (init_request_queue(&table->request_heap, table->coder_count * 4) != 0)
		return (1);
	assign_dongles(table);
	return (0);
}
