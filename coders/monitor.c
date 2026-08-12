/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla <marvin@42.fr>>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 10:41:27 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/11 20:15:00 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	inspect_coders(t_sim *sim, int *burned)
{
	int			i;
	int			done;
	long long	now;

	i = 0;
	done = 0;
	now = current_time_ms();
	*burned = -1;
	while (i < sim->cfg.number)
	{
		if (sim->coders[i].compiled >= sim->cfg.required)
			done++;
		else if (now >= sim->coders[i].deadline)
		{
			*burned = i;
			break ;
		}
		i++;
	}
	return (done == sim->cfg.number);
}

static void	end_monitor(t_sim *sim, int burned)
{
	long long	stamp;

	stamp = current_time_ms() - sim->start;
	pthread_mutex_unlock(&sim->state);
	if (burned >= 0)
	{
		pthread_mutex_lock(&sim->log);
		printf("%lld %d burned out\n", stamp, sim->coders[burned].id);
		pthread_mutex_unlock(&sim->log);
	}
}

void	*monitor_thread(void *arg)
{
	t_sim		*sim;
	int			burned;

	sim = (t_sim *)arg;
	burned = -1;
	pthread_mutex_lock(&sim->state);
	while (!sim->stop)
	{
		if (inspect_coders(sim, &burned) || burned >= 0)
		{
			sim->stop = 1;
			pthread_cond_broadcast(&sim->state_changed);
			break ;
		}
		pthread_mutex_unlock(&sim->state);
		usleep(400);
		pthread_mutex_lock(&sim->state);
	}
	end_monitor(sim, burned);
	return (NULL);
}
