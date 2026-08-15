/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_pair.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla <marvin@42.fr>>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 10:10:00 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/15 17:25:57 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_queue_head(t_dongle *dongle, t_coder *coder)
{
	if (!dongle || !dongle->queue)
		return (FAIL);
	if (heap_is_empty(dongle->queue))
		return (FAIL);
	return (dongle->queue->item[0] == coder);
}

void	order_dongles(t_coder *coder, t_dongle **first, t_dongle **second)
{
	if (coder->left < coder->right)
	{
		*first = coder->left;
		*second = coder->right;
	}
	else
	{
		*first = coder->right;
		*second = coder->left;
	}
}

void	remove_from_queues(t_coder *coder)
{
	heap_remove_index(coder->left->queue,
		heap_find_index(coder->left->queue, coder));
	heap_remove_index(coder->right->queue,
		heap_find_index(coder->right->queue, coder));
}

void	lock_dongles(t_dongle *first, t_dongle *second, int heap_lock)
{
	if (heap_lock)
	{
		pthread_mutex_lock(&first->queue_mutex);
		pthread_mutex_lock(&second->queue_mutex);
	}
	pthread_mutex_lock(&first->mutex);
	pthread_mutex_lock(&second->mutex);
}

void	unlock_dongles(t_dongle *first, t_dongle *second, int heap_lock)
{
	if (heap_lock)
	{
		pthread_mutex_unlock(&second->queue_mutex);
		pthread_mutex_unlock(&first->queue_mutex);
	}
	pthread_mutex_unlock(&second->mutex);
	pthread_mutex_unlock(&first->mutex);
}
