/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 13:52:22 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/14 13:37:12 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	cleanup_dongles(t_sim *sim, int coder_count)
{
	int	index;

	if (!sim || !sim->dongles)
		return ;
	index = 0;
	while (index < coder_count)
	{
		if (sim->dongles[index].queue)
		{
			free(sim->dongles[index].queue->item);
			free(sim->dongles[index].queue);
		}
		if (sim->dongles[index].queue_mutex_ready)
			pthread_mutex_destroy(&sim->dongles[index].queue_mutex);
		if (sim->dongles[index].mutex_ready)
			pthread_mutex_destroy(&sim->dongles[index].mutex);
		index++;
	}
}

static void	destroy_coder_sync(t_sim *sim)
{
	int	index;

	index = 0;
	while (index < sim->config.coder_count)
	{
		if (sim->coders[index].state_mutex_ready)
			pthread_mutex_destroy(&sim->coders[index].state_mutex);
		index++;
	}
}

void	destroy_simulation(t_sim *sim)
{
	if (sim->coders)
		destroy_coder_sync(sim);
	cleanup_dongles(sim, sim->config.coder_count);
	if (sim->log_mutex_ready)
		pthread_mutex_destroy(&sim->log_mutex);
	if (sim->start_condition_ready)
		pthread_cond_destroy(&sim->start_condition);
	if (sim->state_mutex_ready)
		pthread_mutex_destroy(&sim->state_mutex);
	free(sim->coders);
	free(sim->dongles);
}
