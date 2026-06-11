/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 10:41:27 by mel-asla          #+#    #+#             */
/*   Updated: 2026/06/11 09:14:46 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	need_to_stop(t_runtime *sim)
{
	bool	stop_flag;

	pthread_mutex_lock(&sim->request_mutex);
	stop_flag = sim->stop_requested;
	pthread_mutex_unlock(&sim->request_mutex);
	return (stop_flag);
}

void	request_stop(t_runtime *sim)
{
	pthread_mutex_lock(&sim->request_mutex);
	sim->stop_requested = 1;
	pthread_cond_broadcast(&sim->request_cond);
	pthread_mutex_unlock(&sim->request_mutex);
}

static int	has_burned_out(t_runtime *sim,
	int index, long long current_time, int *all_finish)
{
	long long	burn_out;
	long long	time_to_die;
	int			compile_count;

	pthread_mutex_lock(&sim->coders[index].state_mutex);
	time_to_die = sim->coders[index].burnout_deadline;
	compile_count = sim->coders[index].completed_compiles;
	pthread_mutex_unlock(&sim->coders[index].state_mutex);
	if (compile_count >= sim->config.compiles_required)
	{
		*all_finish += 1;
		return (0);
	}
	if (current_time < time_to_die)
		return (0);
	request_stop(sim);
	pthread_mutex_lock(&sim->request_mutex);
	burn_out = get_time_in_ms() - sim->start_time_ms;
	pthread_mutex_unlock(&sim->request_mutex);
	pthread_mutex_lock(&sim->log_mutex);
	printf("%lld %lld burned out\n", burn_out,
		sim->coders[index].id);
	pthread_mutex_unlock(&sim->log_mutex);
	return (1);
}

void	*monitor_loop(void *arg)
{
	t_runtime	*sim;
	long long	current_time;
	int			index;
	int			all_finish;

	sim = (t_runtime *)arg;
	while (!need_to_stop(sim))
	{
		current_time = get_time_in_ms();
		index = 0;
		all_finish = 0;
		while (index < sim->config.num_coders)
		{
			if (has_burned_out(sim, index, current_time, &all_finish))
				return (NULL);
			index++;
		}
		if (all_finish >= sim->config.num_coders)
		{
			request_stop(sim);
			return (NULL);
		}
		smart_sleep(5, sim);
	}
	return (NULL);
}
