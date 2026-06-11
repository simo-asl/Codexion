/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 08:12:47 by mel-asla          #+#    #+#             */
/*   Updated: 2026/06/11 09:15:19 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	free_heap(t_queue *queue)
{
	if (!queue)
		return ;
	if (queue->items)
		free(queue->items);
	free(queue);
}

void	cleanup_dongles(t_runtime *sim, int num_coders)
{
	int	index;

	if (!sim || !sim->dongles)
		return ;
	index = 0;
	while (index < num_coders)
	{
		if (sim->dongles[index].queue)
			free_heap(sim->dongles[index].queue);
		if (sim->dongles[index].queue_mutex_ready)
			pthread_mutex_destroy(&sim->dongles[index].queue_mutex);
		if (sim->dongles[index].mutex_ready)
			pthread_mutex_destroy(&sim->dongles[index].mutex);
		index++;
	}
	free(sim->dongles);
	sim->dongles = NULL;
}

static void	destroy_coder_sync(t_runtime *sim)
{
	int	index;

	index = 0;
	while (index < sim->config.num_coders)
	{
		if (sim->coders[index].state_mutex_ready)
			pthread_mutex_destroy(&sim->coders[index].state_mutex);
		index++;
	}
}

void	cleanup_all(t_runtime *sim)
{
	if (!sim)
		return ;
	if (sim->coders)
	{
		destroy_coder_sync(sim);
		free(sim->coders);
		sim->coders = NULL;
	}
	cleanup_dongles(sim, sim->config.num_coders);
	if (sim->log_mutex_ready)
		pthread_mutex_destroy(&sim->log_mutex);
	if (sim->request_cond_ready)
		pthread_cond_destroy(&sim->request_cond);
	if (sim->request_mutex_ready)
		pthread_mutex_destroy(&sim->request_mutex);
	free(sim);
}
