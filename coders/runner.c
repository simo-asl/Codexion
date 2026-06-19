/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla <marvin@42.fr>>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 11:08:01 by mel-asla          #+#    #+#             */
/*   Updated: 2026/06/19 13:29:07 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	set_start_state(t_runtime *sim)
{
	int			i;
	long long	start_ms;

	start_ms = get_time_in_ms();
	i = 0;
	while (i < sim->config.num_coders)
	{
		pthread_mutex_lock(&sim->coders[i].state_mutex);
		sim->coders[i].burnout_deadline = start_ms + \
sim->config.time_to_burnout;
		pthread_mutex_unlock(&sim->coders[i].state_mutex);
		i++;
	}
}

static void	broadcast_start(t_runtime *sim)
{
	pthread_mutex_lock(&sim->request_mutex);
	sim->start_time_ms = get_time_in_ms();
	pthread_cond_broadcast(&sim->request_cond);
	pthread_mutex_unlock(&sim->request_mutex);
}

static int	create_coder_threads(t_runtime *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.num_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL,
				coder_thread, &sim->coders[i]) != 0)
		{
			request_stop(sim);
			join_coder_threads(sim, i);
			return (1);
		}
		i++;
	}
	return (0);
}

int	join_coder_threads(t_runtime *sim, int end)
{
	int	i;

	i = 0;
	if (!end)
		end = sim->config.num_coders;
	while (i < end)
	{
		if (pthread_join(sim->coders[i].thread, NULL) != 0)
		{
			request_stop(sim);
			return (1);
		}
		i++;
	}
	return (0);
}

int	run_coders(t_runtime *sim)
{
	if (create_coder_threads(sim) != 0)
	{
		request_stop(sim);
		return (1);
	}
	set_start_state(sim);
	if (pthread_create(&sim->monitor_thread, NULL, monitor_loop, sim) != 0)
	{
		request_stop(sim);
		join_coder_threads(sim, 0);
		return (1);
	}
	broadcast_start(sim);
	if (join_coder_threads(sim, 0) != 0)
	{
		pthread_join(sim->monitor_thread, NULL);
		return (1);
	}
	request_stop(sim);
	if (pthread_join(sim->monitor_thread, NULL) != 0)
		return (1);
	return (0);
}
