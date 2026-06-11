/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 03:08:41 by mel-asla          #+#    #+#             */
/*   Updated: 2026/06/11 09:15:13 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	is_top_of_heap(t_dongle *dongle, t_coder *coder)
{
	if (!dongle || !dongle->queue)
		return (false);
	if (heap_is_empty(dongle->queue))
		return (false);
	return (dongle->queue->items[0] == coder);
}

void	lock_order(t_coder *coder, t_dongle **first, t_dongle **second)
{
	if (coder->left_dongle < coder->right_dongle)
	{
		*first = coder->left_dongle;
		*second = coder->right_dongle;
	}
	else
	{
		*first = coder->right_dongle;
		*second = coder->left_dongle;
	}
}

void	cleanup_heaps_locked(t_coder *coder)
{
	if (coder->left_dongle == coder->right_dongle)
	{
		heap_remove_at(coder->left_dongle->queue,
			heap_find_index(coder->left_dongle->queue, coder));
		return ;
	}
	heap_remove_at(coder->left_dongle->queue,
		heap_find_index(coder->left_dongle->queue, coder));
	heap_remove_at(coder->right_dongle->queue,
		heap_find_index(coder->right_dongle->queue, coder));
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
