/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 12:26:14 by mel-asla          #+#    #+#             */
/*   Updated: 2026/04/15 11:47:44 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_status(t_coder *coder, const char *msg)
{
	t_table	*table;
	long	ts;

	table = coder->table;
	pthread_mutex_lock(&table->log_mutex);
	if (!get_simulation_end(table))
	{
		ts = get_elapsed_time_ms(table->start_time_ms);
		printf("%ld %d %s\n", ts, coder->id, msg);
	}
	pthread_mutex_unlock(&table->log_mutex);
}

void	print_burnout(t_coder *coder)
{
	t_table	*table;
	long	ts;

	table = coder->table;
	pthread_mutex_lock(&table->log_mutex);
	ts = get_elapsed_time_ms(table->start_time_ms);
	printf("%ld %d burned out\n", ts, coder->id);
	pthread_mutex_unlock(&table->log_mutex);
}
