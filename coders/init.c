/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla <marvin@42.fr>>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 20:37:08 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/18 22:13:00 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_shared_sync(t_sim *sim)
{
	if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
		return (FAIL);
	sim->log_mutex_ready = 1;
	if (pthread_mutex_init(&sim->state_mutex, NULL) != 0)
		return (FAIL);
	sim->state_mutex_ready = 1;
	if (pthread_cond_init(&sim->start_condition, NULL) != 0)
		return (FAIL);
	sim->start_condition_ready = 1;
	return (SUCCESS);
}

static int	allocate_arrays(t_sim *sim)
{
	sim->dongles = malloc(sizeof(t_dongle) * sim->config.coder_count);
	if (!sim->dongles)
		return (FAIL);
	memset(sim->dongles, 0, sizeof(t_dongle) * sim->config.coder_count);
	sim->coders = malloc(sizeof(t_coder) * sim->config.coder_count);
	if (!sim->coders)
	{
		return (FAIL);
	}
	memset(sim->coders, 0, sizeof(t_coder) * sim->config.coder_count);
	return (SUCCESS);
}

static int	init_dongles(t_sim *sim)
{
	int	index;

	index = 0;
	while (index < sim->config.coder_count)
	{
		if (pthread_mutex_init(&sim->dongles[index].mutex, NULL) != 0)
			return (FAIL);
		sim->dongles[index].mutex_ready = 1;
		sim->dongles[index].queue = create_heap(2, sim->config.is_edf);
		if (!sim->dongles[index].queue)
			return (FAIL);
		if (pthread_mutex_init(&sim->dongles[index].queue_mutex, NULL) != 0)
			return (FAIL);
		sim->dongles[index].queue_mutex_ready = 1;
		index++;
	}
	return (SUCCESS);
}

static int	init_coders(t_sim *sim)
{
	int	index;
	int	next_index;

	index = 0;
	while (index < sim->config.coder_count)
	{
		next_index = (index + 1) % sim->config.coder_count;
		sim->coders[index].id = index + 1;
		sim->coders[index].right = &sim->dongles[index];
		sim->coders[index].left = &sim->dongles[next_index];
		sim->coders[index].sim = sim;
		if (pthread_mutex_init(&sim->coders[index].state_mutex, NULL) != 0)
			return (FAIL);
		sim->coders[index].state_mutex_ready = 1;
		index++;
	}
	return (SUCCESS);
}

int	initialize_simulation(t_sim *sim)
{
	if (init_shared_sync(sim) || allocate_arrays(sim) != SUCCESS
		|| init_dongles(sim) != SUCCESS
		|| init_coders(sim) != SUCCESS)
		return (FAIL);
	return (SUCCESS);
}
