/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 09:55:18 by mel-asla          #+#    #+#             */
/*   Updated: 2026/04/26 18:39:16 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static long	get_raw_timestamp_ms(void)
{
	struct timespec	ts;

	if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
		return (0);
	return ((ts.tv_sec * 1000L) + (ts.tv_nsec / 1000000L));
}

long	get_timestamp_ms(void)
{
	return (get_raw_timestamp_ms());
}

long	get_elapsed_time_ms(long start_time)
{
	return (get_timestamp_ms() - start_time);
}

void	precise_sleep(long duration_ms, t_table *table)
{
	long	start;
	long	now;
	long	remaining;

	start = get_timestamp_ms();
	while (!get_simulation_end(table))
	{
		now = get_timestamp_ms();
		if (now - start >= duration_ms)
			break ;
		remaining = duration_ms - (now - start);
		if (remaining > 2)
			usleep(1000);
		else
			usleep(200);
	}
}
