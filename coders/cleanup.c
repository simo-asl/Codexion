/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 08:12:47 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/09 21:18:00 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	cleanup_dongles(t_sim *sim)
{
	int	i;

	if (!sim->dongles)
		return ;
	i = 0;
	while (i < sim->cfg.number)
	{
		free(sim->dongles[i].queue.item);
		if (sim->dongles[i].cond_ready)
			pthread_cond_destroy(&sim->dongles[i].changed);
		if (sim->dongles[i].mutex_ready)
			pthread_mutex_destroy(&sim->dongles[i].mutex);
		i++;
	}
	free(sim->dongles);
}

void	destroy_simulation(t_sim *sim)
{
	cleanup_dongles(sim);
	free(sim->coders);
	if (sim->cond_ready)
		pthread_cond_destroy(&sim->changed);
	if (sim->log_ready)
		pthread_mutex_destroy(&sim->log);
	if (sim->state_ready)
		pthread_mutex_destroy(&sim->state);
}
