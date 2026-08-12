/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_pair.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 08:25:36 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/11 19:32:00 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	lock_dongle_pair(t_coder *coder)
{
	if (coder->left < coder->right)
	{
		pthread_mutex_lock(&coder->left->mutex);
		pthread_mutex_lock(&coder->right->mutex);
	}
	else
	{
		pthread_mutex_lock(&coder->right->mutex);
		pthread_mutex_lock(&coder->left->mutex);
	}
}

void	unlock_dongle_pair(t_coder *coder)
{
	pthread_mutex_unlock(&coder->left->mutex);
	pthread_mutex_unlock(&coder->right->mutex);
}

void	leave_queues(t_coder *coder)
{
	lock_dongle_pair(coder);
	heap_remove(&coder->left->queue, coder, coder->sim->cfg.edf);
	heap_remove(&coder->right->queue, coder, coder->sim->cfg.edf);
	notify_dongle_waiters(coder->left);
	notify_dongle_waiters(coder->right);
	unlock_dongle_pair(coder);
}

void	release_dongles(t_coder *coder)
{
	long long	ready_at;

	lock_dongle_pair(coder);
	ready_at = current_time_ms() + coder->sim->cfg.cooldown;
	coder->left->busy = 0;
	coder->right->busy = 0;
	coder->left->ready_at = ready_at;
	coder->right->ready_at = ready_at;
	notify_dongle_waiters(coder->left);
	notify_dongle_waiters(coder->right);
	unlock_dongle_pair(coder);
}
