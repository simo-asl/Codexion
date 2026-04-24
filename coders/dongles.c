/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 07:58:11 by mel-asla          #+#    #+#             */
/*   Updated: 2026/04/24 04:05:50 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	set_lock_order(t_coder *coder, t_dongle **first, t_dongle **second)
{
	*first = coder->left_dongle;
	*second = coder->right_dongle;
	if (*second < *first)
	{
		*first = coder->right_dongle;
		*second = coder->left_dongle;
	}
}

bool	dongle_is_available(t_dongle *dongle, long now)
{
	if (dongle->in_use)
		return (false);
	if (now < dongle->available_at_ms)
		return (false);
	return (true);
}

void	set_dongle_cooldown(t_dongle *dongle, long cooldown, long now)
{
	dongle->available_at_ms = now + cooldown;
}

int	take_dongles(t_coder *coder)
{
	t_table	*table;

	table = coder->table;
	pthread_mutex_lock(&table->request_mutex);
	if (request_dongles(coder) != 0)
	{
		pthread_mutex_unlock(&table->request_mutex);
		return (1);
	}
	pthread_mutex_unlock(&table->request_mutex);
	print_status(coder, "has taken a dongle");
	if (coder->left_dongle != coder->right_dongle)
		print_status(coder, "has taken a dongle");
	return (0);
}

void	release_dongles(t_coder *coder)
{
	t_table		*table;
	t_dongle	*first;
	t_dongle	*second;
	long		now;

	table = coder->table;
	now = get_timestamp_ms();
	set_lock_order(coder, &first, &second);
	pthread_mutex_lock(&first->mutex);
	if (second != first)
		pthread_mutex_lock(&second->mutex);
	coder->left_dongle->in_use = false;
	coder->right_dongle->in_use = false;
	set_dongle_cooldown(coder->left_dongle, table->dongle_cooldown, now);
	set_dongle_cooldown(coder->right_dongle, table->dongle_cooldown, now);
	if (second != first)
		pthread_mutex_unlock(&second->mutex);
	pthread_mutex_unlock(&first->mutex);
	pthread_mutex_lock(&table->request_mutex);
	pthread_cond_broadcast(&table->request_cond);
	pthread_mutex_unlock(&table->request_mutex);
}
