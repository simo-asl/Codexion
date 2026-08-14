/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 17:42:22 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/14 13:37:12 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	current_time_ms(void)
{
	long long		current_time;
	struct timeval	time_value;

	if (gettimeofday(&time_value, NULL) == -1)
		return (1);
	current_time = (time_value.tv_sec * 1000) + (time_value.tv_usec / 1000);
	return (current_time);
}

void	interruptible_sleep(long long time_to_sleep, t_sim *sim)
{
	long long	start_time;

	start_time = current_time_ms();
	while ((current_time_ms() - start_time) < time_to_sleep)
	{
		if (simulation_stopped(sim))
			break ;
		usleep(50);
	}
}
