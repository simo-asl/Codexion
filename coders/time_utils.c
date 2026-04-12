/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 09:55:18 by mel-asla          #+#    #+#             */
/*   Updated: 2026/04/12 08:23:32 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_timestamp_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
		return (0);
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000L));
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
