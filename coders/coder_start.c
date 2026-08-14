/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_start.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 17:45:23 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/14 13:37:12 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	run_cycle(t_coder *coder)
{
	if (coder->left == coder->right)
		return ;
	if (request_dongles(coder) != SUCCESS)
		return ;
	compile_code(coder);
	if (!simulation_stopped(coder->sim))
		do_other_work(coder);
}

static void	wait_for_start(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->state_mutex);
	while (!coder->sim->stop_requested && coder->sim->start_time_ms == 0)
		pthread_cond_wait(&coder->sim->start_condition,
			&coder->sim->state_mutex);
	pthread_mutex_unlock(&coder->sim->state_mutex);
}

void	*coder_thread(void *arg)
{
	t_coder		*coder;

	coder = (t_coder *)arg;
	wait_for_start(coder);
	if (coder->id % 2 == 0)
		interruptible_sleep(coder->sim->config.time_to_compile / 4,
			coder->sim);
	while (!simulation_stopped(coder->sim))
		run_cycle(coder);
	return (NULL);
}
