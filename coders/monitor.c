/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 10:41:27 by mel-asla          #+#    #+#             */
/*   Updated: 2026/05/01 11:04:19 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	get_simulation_end(t_table *table)
{
	bool	value;

	pthread_mutex_lock(&table->state_mutex);
	value = table->simulation_end;
	pthread_mutex_unlock(&table->state_mutex);
	return (value);
}

void	set_simulation_end(t_table *table, bool value)
{
	pthread_mutex_lock(&table->state_mutex);
	table->simulation_end = value;
	pthread_mutex_unlock(&table->state_mutex);
	if (value)
	{
		pthread_mutex_lock(&table->request_mutex);
		pthread_cond_broadcast(&table->request_cond);
		pthread_mutex_unlock(&table->request_mutex);
	}
}

int	check_burnout(t_coder *coder)
{
	t_table	*table;
	long	now;
	long	deadline;

	table = coder->table;
	now = get_timestamp_ms();
	pthread_mutex_lock(&table->state_mutex);
	if (table->required_compiles > 0
		&& coder->compiles_done >= table->required_compiles)
	{
		pthread_mutex_unlock(&table->state_mutex);
		return (0);
	}
	deadline = coder->last_compile_start_ms + table->time_to_burnout;
	if (!table->simulation_end && now >= deadline)
	{
		pthread_mutex_unlock(&table->state_mutex);
		set_simulation_end(table, true);
		print_burnout(coder);
		return (1);
	}
	pthread_mutex_unlock(&table->state_mutex);
	return (0);
}

bool	all_compiles_completed(t_table *table)
{
	int	i;

	if (table->required_compiles <= 0)
		return (false);
	pthread_mutex_lock(&table->state_mutex);
	i = 0;
	while (i < table->coder_count)
	{
		if (table->coders[i].compiles_done < table->required_compiles)
		{
			pthread_mutex_unlock(&table->state_mutex);
			return (false);
		}
		i++;
	}
	pthread_mutex_unlock(&table->state_mutex);
	set_simulation_end(table, true);
	return (true);
}

void	*monitor_routine(void *arg)
{
	t_table	*table;
	int		i;

	table = (t_table *)arg;
	while (!get_simulation_end(table))
	{
		i = 0;
		while (i < table->coder_count)
		{
			if (check_burnout(&table->coders[i]) != 0)
				return (NULL);
			i++;
		}
		if (all_compiles_completed(table))
			return (NULL);
		usleep(500);
	}
	return (NULL);
}
