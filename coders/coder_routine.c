/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 09:34:22 by mel-asla          #+#    #+#             */
/*   Updated: 2026/06/11 09:15:17 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_event(t_coder *coder, const char *action)
{
	long long	timestamp;

	pthread_mutex_lock(&coder->sim->log_mutex);
	timestamp = get_time_in_ms() - coder->sim->start_time_ms;
	if (!need_to_stop(coder->sim))
		printf("%lld %lld %s\n", timestamp, coder->id, action);
	pthread_mutex_unlock(&coder->sim->log_mutex);
}

static void	run_cycle(t_coder *coder)
{
	if (coder->left_dongle == coder->right_dongle)
		return ;
	if (acquire_dongles(coder) != 0)
		return ;
	do_compile(coder);
	if (!need_to_stop(coder->sim))
		debug_and_refactor(coder);
}

static void	wait_until_start(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->request_mutex);
	while (!coder->sim->stop_requested && coder->sim->start_time_ms == 0)
		pthread_cond_wait(&coder->sim->request_cond,
			&coder->sim->request_mutex);
	pthread_mutex_unlock(&coder->sim->request_mutex);
}

void	*coder_thread(void *arg)
{
	t_coder		*coder;

	coder = (t_coder *)arg;
	wait_until_start(coder);
	if (coder->id % 2 == 0)
		smart_sleep((coder->sim->config.time_to_compile / 4), coder->sim);
	while (!need_to_stop(coder->sim))
		run_cycle(coder);
	return (NULL);
}
