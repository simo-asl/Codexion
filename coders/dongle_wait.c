/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_wait.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:00:00 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/11 00:00:00 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	notify_dongle_waiters(t_dongle *dongle)
{
	dongle->version++;
	pthread_cond_broadcast(&dongle->changed);
}

static t_dongle	*cooldown_dongle(t_coder *coder)
{
	if (coder->left->ready_at > coder->right->ready_at)
		return (coder->left);
	return (coder->right);
}

t_dongle	*select_blocking_dongle(t_coder *coder, long long now,
	unsigned long *version, long long *until)
{
	t_dongle	*blocked;

	blocked = coder->left;
	if (coder->left->queue.item[0] != coder)
		blocked = coder->left;
	else if (coder->right->queue.item[0] != coder)
		blocked = coder->right;
	else if (!coder->left->busy && !coder->right->busy
		&& (now < coder->left->ready_at || now < coder->right->ready_at))
		blocked = cooldown_dongle(coder);
	else if (coder->right->busy)
		blocked = coder->right;
	*version = blocked->version;
	*until = coder->deadline;
	if (!blocked->busy && blocked->ready_at > now
		&& blocked->ready_at < *until)
		*until = blocked->ready_at;
	return (blocked);
}

void	wait_on_dongle(t_dongle *dongle, unsigned long version,
	long long until)
{
	struct timespec	limit;

	limit.tv_sec = until / 1000;
	limit.tv_nsec = (until % 1000) * 1000000;
	pthread_mutex_lock(&dongle->mutex);
	if (dongle->version == version)
		pthread_cond_timedwait(&dongle->changed, &dongle->mutex, &limit);
	pthread_mutex_unlock(&dongle->mutex);
}
