/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 10:17:06 by mel-asla          #+#    #+#             */
/*   Updated: 2026/06/11 09:15:02 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_sync(t_runtime *sim)
{
	if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
		return (1);
	sim->log_mutex_ready = 1;
	if (pthread_mutex_init(&sim->request_mutex, NULL) != 0)
		return (1);
	sim->request_mutex_ready = 1;
	if (pthread_cond_init(&sim->request_cond, NULL) != 0)
		return (1);
	sim->request_cond_ready = 1;
	return (0);
}

static int	init_alloc(t_runtime *sim)
{
	sim->dongles = malloc\
(sizeof(t_dongle) * sim->config.num_coders);
	if (!sim->dongles)
		return (1);
	memset(sim->dongles, 0, sizeof(t_dongle) * sim->config.num_coders);
	sim->coders = malloc(sizeof(t_coder) * sim->config.num_coders);
	if (!sim->coders)
	{
		free(sim->dongles);
		sim->dongles = NULL;
		return (1);
	}
	memset(sim->coders, 0, sizeof(t_coder) * sim->config.num_coders);
	return (0);
}

static int	init_dongles(t_runtime *sim)
{
	int	index;

	index = 0;
	while (index < sim->config.num_coders)
	{
		sim->dongles[index].in_use = 0;
		sim->dongles[index].cooldown = 0;
		sim->dongles[index].queue_mutex_ready = 0;
		sim->dongles[index].mutex_ready = 0;
		if (pthread_mutex_init(&sim->dongles[index].mutex, NULL) != 0)
			return (1);
		sim->dongles[index].queue = heap_init(2, sim->config.scheduler);
		if (!sim->dongles[index].queue)
			return (1);
		if (pthread_mutex_init(&sim->dongles[index].queue_mutex, NULL) != 0)
			return (1);
		sim->dongles[index].mutex_ready = 1;
		sim->dongles[index].queue_mutex_ready = 1;
		index++;
	}
	return (0);
}

static int	init_coders(t_runtime *sim)
{
	int	index;
	int	next_index;

	index = 0;
	while (index < sim->config.num_coders)
	{
		sim->coders[index].state_mutex_ready = 0;
		next_index = (index + 1) % sim->config.num_coders;
		sim->coders[index].id = index + 1;
		sim->coders[index].completed_compiles = 0;
		sim->coders[index].right_dongle = &sim->dongles[index];
		sim->coders[index].left_dongle = &sim->dongles[next_index];
		sim->coders[index].request_timestamp = 0;
		sim->coders[index].sim = sim;
		if (pthread_mutex_init(&sim->coders[index].state_mutex, NULL) != 0)
			return (1);
		sim->coders[index].state_mutex_ready = 1;
		index++;
	}
	return (0);
}

int	init_runtime(t_runtime *sim)
{
	if (init_sync(sim) != 0 || init_alloc(sim) != 0
		|| init_dongles(sim) != 0
		|| init_coders(sim) != 0)
	{
		cleanup_dongles(sim, sim->config.num_coders);
		sim->dongles = NULL;
		sim->coders = NULL;
		return (1);
	}
	return (0);
}
