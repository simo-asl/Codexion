/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 10:29:43 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/10 21:10:00 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	create_coders(t_sim *sim, int *created)
{
	while (*created < sim->cfg.number)
	{
		if (pthread_create(&sim->coders[*created].thread, NULL,
				coder_thread, &sim->coders[*created]))
			return (1);
		(*created)++;
	}
	return (0);
}

static void	start_sim(t_sim *sim)
{
	int	i;

	pthread_mutex_lock(&sim->state);
	sim->start = current_time_ms();
	i = 0;
	while (i < sim->cfg.number)
		sim->coders[i++].deadline = sim->start + sim->cfg.burnout;
	pthread_cond_broadcast(&sim->state_changed);
	pthread_mutex_unlock(&sim->state);
}

int	run_simulation(t_sim *sim)
{
	int	i;
	int	created;
	int	monitor_ok;

	created = 0;
	monitor_ok = 0;
	if (!create_coders(sim, &created))
	{
		start_sim(sim);
		if (!pthread_create(&sim->monitor, NULL, monitor_thread, sim))
			monitor_ok = 1;
	}
	if (!monitor_ok)
		stop_simulation(sim);
	i = 0;
	while (i < created)
		pthread_join(sim->coders[i++].thread, NULL);
	if (monitor_ok)
		pthread_join(sim->monitor, NULL);
	return (!monitor_ok);
}
