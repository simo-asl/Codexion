/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 09:34:22 by mel-asla          #+#    #+#             */
/*   Updated: 2026/04/24 04:06:00 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	coder_completed_required(t_coder *coder)
{
	t_table	*table;
	bool	completed;

	table = coder->table;
	if (table->required_compiles <= 0)
		return (false);
	pthread_mutex_lock(&table->state_mutex);
	completed = (coder->compiles_done >= table->required_compiles);
	pthread_mutex_unlock(&table->state_mutex);
	return (completed);
}

int	coder_compile(t_coder *coder)
{
	t_table	*table;

	table = coder->table;
	if (take_dongles(coder) != 0)
		return (1);
	if (get_simulation_end(table))
	{
		release_dongles(coder);
		return (1);
	}
	print_status(coder, "is compiling");
	precise_sleep(table->time_to_compile, table);
	pthread_mutex_lock(&table->state_mutex);
	coder->compiles_done++;
	pthread_mutex_unlock(&table->state_mutex);
	release_dongles(coder);
	if (all_compiles_completed(table))
		return (1);
	return (0);
}

int	coder_debug(t_coder *coder)
{
	t_table	*table;

	table = coder->table;
	pthread_mutex_lock(&table->state_mutex);
	coder->state = STATE_DEBUGGING;
	pthread_mutex_unlock(&table->state_mutex);
	print_status(coder, "is debugging");
	precise_sleep(table->time_to_debug, table);
	return (0);
}

int	coder_refactor(t_coder *coder)
{
	t_table	*table;

	table = coder->table;
	pthread_mutex_lock(&table->state_mutex);
	coder->state = STATE_REFACTORING;
	pthread_mutex_unlock(&table->state_mutex);
	print_status(coder, "is refactoring");
	precise_sleep(table->time_to_refactor, table);
	return (0);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_table	*table;

	coder = (t_coder *)arg;
	table = coder->table;
	if (table->coder_count == 1)
	{
		print_status(coder, "has taken a dongle");
		while (!get_simulation_end(table))
			usleep(1000);
		return (NULL);
	}
	while (!get_simulation_end(table))
	{
		if (coder_completed_required(coder) || coder_compile(coder) != 0)
			break ;
		if (get_simulation_end(table) || coder_completed_required(coder))
			break ;
		coder_debug(coder);
		if (get_simulation_end(table) || coder_completed_required(coder))
			break ;
		coder_refactor(coder);
	}
	return (NULL);
}
