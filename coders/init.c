/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 10:17:06 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/09 21:17:00 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_shared(t_sim *sim)
{
	if (pthread_mutex_init(&sim->state, NULL))
		return (1);
	sim->state_ready = 1;
	if (pthread_mutex_init(&sim->log, NULL))
		return (1);
	sim->log_ready = 1;
	if (pthread_cond_init(&sim->state_changed, NULL))
		return (1);
	sim->state_cond_ready = 1;
	return (0);
}

static int	init_dongles(t_sim *sim)
{
	int	i;

	sim->dongles = malloc(sizeof(t_dongle) * sim->cfg.number);
	if (!sim->dongles)
		return (1);
	memset(sim->dongles, 0, sizeof(t_dongle) * sim->cfg.number);
	i = 0;
	while (i < sim->cfg.number)
	{
		sim->dongles[i].queue.capacity = 2;
		sim->dongles[i].queue.item = malloc(sizeof(t_coder *)
				* sim->dongles[i].queue.capacity);
		if (!sim->dongles[i].queue.item
			|| pthread_mutex_init(&sim->dongles[i].mutex, NULL))
			return (1);
		sim->dongles[i].mutex_ready = 1;
		if (pthread_cond_init(&sim->dongles[i].resource_changed, NULL))
			return (1);
		sim->dongles[i].change_cond_ready = 1;
		i++;
	}
	return (0);
}

static int	init_coders(t_sim *sim)
{
	int	i;

	sim->coders = malloc(sizeof(t_coder) * sim->cfg.number);
	if (!sim->coders)
		return (1);
	memset(sim->coders, 0, sizeof(t_coder) * sim->cfg.number);
	i = 0;
	while (i < sim->cfg.number)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].right = &sim->dongles[i];
		sim->coders[i].left = &sim->dongles[(i + 1) % sim->cfg.number];
		sim->coders[i].sim = sim;
		i++;
	}
	return (0);
}

int	initialize_simulation(t_sim *sim)
{
	if (init_shared(sim) || init_dongles(sim) || init_coders(sim))
		return (1);
	return (0);
}
