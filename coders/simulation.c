/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 13:55:22 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/14 13:37:12 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	set_start_state(t_sim *sim)
{
	int			i;
	long long	start_ms;

	start_ms = current_time_ms();
	i = 0;
	while (i < sim->config.coder_count)
	{
		pthread_mutex_lock(&sim->coders[i].state_mutex);
		sim->coders[i].deadline = start_ms
			+ sim->config.time_to_burnout;
		pthread_mutex_unlock(&sim->coders[i].state_mutex);
		i++;
	}
}

static void	broadcast_start(t_sim *sim)
{
	pthread_mutex_lock(&sim->state_mutex);
	sim->start_time_ms = current_time_ms();
	pthread_cond_broadcast(&sim->start_condition);
	pthread_mutex_unlock(&sim->state_mutex);
}

static int	create_coder_threads(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.coder_count)
	{
		if (pthread_create(&sim->coders[i].thread, NULL,
				coder_thread, &sim->coders[i]) != 0)
		{
			stop_simulation(sim);
			join_coder_threads(sim, i);
			return (FAIL);
		}
		i++;
	}
	return (SUCCESS);
}

int	join_coder_threads(t_sim *sim, int end)
{
	int	i;

	i = 0;
	if (!end)
		end = sim->config.coder_count;
	while (i < end)
	{
		if (pthread_join(sim->coders[i].thread, NULL) != 0)
		{
			stop_simulation(sim);
			while (++i < end)
				pthread_join(sim->coders[i].thread, NULL);
			return (FAIL);
		}
		i++;
	}
	return (SUCCESS);
}

int	run_simulation(t_sim *sim)
{
	if (create_coder_threads(sim) != SUCCESS)
	{
		stop_simulation(sim);
		return (FAIL);
	}
	set_start_state(sim);
	if (pthread_create(&sim->monitor_thread, NULL, monitor_thread, sim) != 0)
	{
		stop_simulation(sim);
		join_coder_threads(sim, 0);
		return (FAIL);
	}
	broadcast_start(sim);
	if (join_coder_threads(sim, 0) != SUCCESS)
	{
		pthread_join(sim->monitor_thread, NULL);
		return (FAIL);
	}
	stop_simulation(sim);
	if (pthread_join(sim->monitor_thread, NULL) != 0)
		return (FAIL);
	return (SUCCESS);
}
