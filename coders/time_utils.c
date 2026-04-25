/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 09:55:18 by mel-asla          #+#    #+#             */
/*   Updated: 2026/04/24 04:04:38 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static long	get_raw_timestamp_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
		return (0);
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000L));
}

static pthread_mutex_t	*get_time_mutex(void)
{
	static pthread_mutex_t	time_mutex = PTHREAD_MUTEX_INITIALIZER;

	return (&time_mutex);
}

long	get_timestamp_ms(void)
{
	static long			state[3];
	long				raw_ms;
	long				delta;

	raw_ms = get_raw_timestamp_ms();
	pthread_mutex_lock(get_time_mutex());
	if (!state[0])
	{
		state[0] = 1;
		state[1] = raw_ms;
		state[2] = raw_ms;
		pthread_mutex_unlock(get_time_mutex());
		return (raw_ms);
	}
	delta = raw_ms - state[1];
	if (delta < 0)
		delta = 0;
	if (delta > 1000)
		delta = 1;
	state[2] += delta;
	state[1] = raw_ms;
	pthread_mutex_unlock(get_time_mutex());
	return (state[2]);
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
