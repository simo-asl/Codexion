/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_start.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 09:27:44 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/10 19:05:00 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	wait_for_start(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->state);
	while (!coder->sim->start && !coder->sim->stop)
		pthread_cond_wait(&coder->sim->changed, &coder->sim->state);
	pthread_mutex_unlock(&coder->sim->state);
}

void	startup_delay(t_coder *coder)
{
	long long	spread;

	spread = (coder->sim->cfg.number + 1) / 8;
	if (coder->id % 2 == 0)
		interruptible_sleep(coder->sim, spread);
}
