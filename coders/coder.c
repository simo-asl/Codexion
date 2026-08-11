/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 09:20:11 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/10 18:42:00 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	finished(t_coder *coder)
{
	int	done;

	pthread_mutex_lock(&coder->sim->state);
	done = coder->compiled >= coder->sim->cfg.required;
	pthread_mutex_unlock(&coder->sim->state);
	return (done);
}

static void	compile(t_coder *coder)
{
	long long	remaining;

	remaining = coder->compile_start + coder->sim->cfg.compile
		- current_time_ms();
	if (remaining > 0)
		interruptible_sleep(coder->sim, remaining);
	pthread_mutex_lock(&coder->sim->state);
	coder->compiled++;
	pthread_mutex_unlock(&coder->sim->state);
	release_dongles(coder);
}

static void	other_work(t_coder *coder)
{
	print_coder_state(coder, "is debugging");
	interruptible_sleep(coder->sim, coder->sim->cfg.debug);
	if (simulation_stopped(coder->sim))
		return ;
	print_coder_state(coder, "is refactoring");
	interruptible_sleep(coder->sim, coder->sim->cfg.refactor);
}

static void	single_coder(t_coder *coder)
{
	int	alive;

	pthread_mutex_lock(&coder->sim->state);
	alive = !coder->sim->stop && current_time_ms() < coder->deadline;
	pthread_mutex_unlock(&coder->sim->state);
	if (!alive)
		return ;
	pthread_mutex_lock(&coder->left->mutex);
	coder->left->busy = 1;
	pthread_mutex_unlock(&coder->left->mutex);
	print_coder_state(coder, "has taken a dongle");
	while (!simulation_stopped(coder->sim))
		timed_wait_until(coder->sim, coder->deadline);
}

void	*coder_thread(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	wait_for_start(coder);
	if (coder->sim->cfg.number == 1)
	{
		single_coder(coder);
		return (NULL);
	}
	startup_delay(coder);
	while (!simulation_stopped(coder->sim) && !finished(coder))
	{
		if (request_dongles(coder))
			break ;
		compile(coder);
		if (!simulation_stopped(coder->sim) && !finished(coder))
			other_work(coder);
	}
	return (NULL);
}
