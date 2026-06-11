/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 09:55:18 by mel-asla          #+#    #+#             */
/*   Updated: 2026/06/11 09:13:51 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	get_time_in_ms(void)
{
	long long		now;
	struct timeval	time_value;

	if (gettimeofday(&time_value, NULL) == -1)
		return (1);
	now = (time_value.tv_sec * 1000) + (time_value.tv_usec / 1000);
	return (now);
}

void	smart_sleep(long long time_to_sleep, t_runtime *sim)
{
	long long	start;

	start = get_time_in_ms();
	while ((get_time_in_ms() - start) < time_to_sleep)
	{
		if (need_to_stop(sim))
			break ;
		usleep(50);
	}
}
