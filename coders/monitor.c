/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:06:00 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/14 13:37:12 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	simulation_stopped(t_sim *sim)
{
	int	stop_flag;

	pthread_mutex_lock(&sim->state_mutex);
	stop_flag = sim->stop_requested;
	pthread_mutex_unlock(&sim->state_mutex);
	return (stop_flag);
}

void	stop_simulation(t_sim *sim)
{
	pthread_mutex_lock(&sim->state_mutex);
	sim->stop_requested = 1;
	pthread_cond_broadcast(&sim->start_condition);
	pthread_mutex_unlock(&sim->state_mutex);
}

static int	has_burned_out(t_sim *sim, int index, long long current_time,
	int *all_finish)
{
	long long	burn;
	long long	deadline;
	int			compile_count;

	pthread_mutex_lock(&sim->coders[index].state_mutex);
	deadline = sim->coders[index].deadline;
	compile_count = sim->coders[index].compile_count;
	pthread_mutex_unlock(&sim->coders[index].state_mutex);
	if (compile_count >= sim->config.compiles_required)
	{
		*all_finish += 1;
		return (0);
	}
	if (current_time < deadline)
		return (0);
	stop_simulation(sim);
	pthread_mutex_lock(&sim->state_mutex);
	burn = current_time_ms() - sim->start_time_ms;
	pthread_mutex_unlock(&sim->state_mutex);
	pthread_mutex_lock(&sim->log_mutex);
	printf("%lld %lld burned out\n", burn,
		sim->coders[index].id);
	pthread_mutex_unlock(&sim->log_mutex);
	return (1);
}

void	*monitor_thread(void *arg)
{
	t_sim		*sim;
	long long	current_time;
	int			index;
	int			all_finish;

	sim = (t_sim *)arg;
	while (!simulation_stopped(sim))
	{
		current_time = current_time_ms();
		index = 0;
		all_finish = 0;
		while (index < sim->config.coder_count)
		{
			if (has_burned_out(sim, index, current_time, &all_finish))
				return (NULL);
			index++;
		}
		if (all_finish >= sim->config.coder_count)
		{
			stop_simulation(sim);
			return (NULL);
		}
		interruptible_sleep(5, sim);
	}
	return (NULL);
}
