/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 00:00:00 by symo              #+#    #+#             */
/*   Updated: 2026/05/01 10:20:20 by mel-asla         ###   ########.fr       */
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

bool	both_dongles_available(t_coder *coder, long now)
{
	t_dongle	*first;
	t_dongle	*second;
	bool		left_available;
	bool		right_available;

	set_lock_order(coder, &first, &second);
	pthread_mutex_lock(&first->mutex);
	if (second != first)
		pthread_mutex_lock(&second->mutex);
	left_available = dongle_is_available(coder->left_dongle, now);
	right_available = dongle_is_available(coder->right_dongle, now);
	if (second != first)
		pthread_mutex_unlock(&second->mutex);
	pthread_mutex_unlock(&first->mutex);
	return (left_available && right_available);
}

bool	reserve_dongles_if_available(t_coder *coder, long now)
{
	t_dongle	*first;
	t_dongle	*second;
	bool		right_available;
	bool		left_available;

	set_lock_order(coder, &first, &second);
	pthread_mutex_lock(&first->mutex);
	if (second != first)
		pthread_mutex_lock(&second->mutex);
	left_available = dongle_is_available(coder->left_dongle, now);
	right_available = dongle_is_available(coder->right_dongle, now);
	if (left_available && right_available)
	{
		coder->left_dongle->in_use = true;
		coder->right_dongle->in_use = true;
	}
	if (second != first)
		pthread_mutex_unlock(&second->mutex);
	pthread_mutex_unlock(&first->mutex);
	return (left_available && right_available);
}

long	max_ready_time_ms(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;
	long		ready_at;

	set_lock_order(coder, &first, &second);
	pthread_mutex_lock(&first->mutex);
	if (second != first)
		pthread_mutex_lock(&second->mutex);
	ready_at = coder->left_dongle->available_at_ms;
	if (coder->right_dongle->available_at_ms > ready_at)
		ready_at = coder->right_dongle->available_at_ms;
	if (second != first)
		pthread_mutex_unlock(&second->mutex);
	pthread_mutex_unlock(&first->mutex);
	return (ready_at);
}

void	build_timeout(long delay_ms, struct timespec *timeout)
{
	struct timeval	now;
	long			sec;
	long			nsec;

	if (delay_ms < 0)
		delay_ms = 0;
	if (gettimeofday(&now, NULL) != 0)
	{
		timeout->tv_sec = 0;
		timeout->tv_nsec = 0;
		return ;
	}
	sec = now.tv_sec + (delay_ms / 1000);
	nsec = (now.tv_usec * 1000L) + ((delay_ms % 1000) * 1000000L);
	sec += nsec / 1000000000L;
	nsec %= 1000000000L;
	timeout->tv_sec = sec;
	timeout->tv_nsec = nsec;
}
