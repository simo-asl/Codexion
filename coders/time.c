/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 09:55:18 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/11 22:05:00 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	current_time_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	return ((long long)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	interruptible_sleep(t_sim *sim, long long duration)
{
	long long	end;

	end = current_time_ms() + duration;
	while (!simulation_stopped(sim) && current_time_ms() < end)
		timed_wait_until(sim, end);
}

void	timed_wait_until(t_sim *sim, long long when)
{
	struct timespec	limit;

	limit.tv_sec = when / 1000;
	limit.tv_nsec = (when % 1000) * 1000000;
	pthread_mutex_lock(&sim->state);
	if (!sim->stop)
		pthread_cond_timedwait(&sim->changed, &sim->state, &limit);
	pthread_mutex_unlock(&sim->state);
}

int	simulation_stopped(t_sim *sim)
{
	int	value;

	pthread_mutex_lock(&sim->state);
	value = sim->stop;
	pthread_mutex_unlock(&sim->state);
	return (value);
}

void	stop_simulation(t_sim *sim)
{
	pthread_mutex_lock(&sim->state);
	sim->stop = 1;
	pthread_cond_broadcast(&sim->changed);
	pthread_mutex_unlock(&sim->state);
}
