/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 17:45:23 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/14 13:37:12 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_coder_state(t_coder *coder, const char *action)
{
	long long	timestamp;

	pthread_mutex_lock(&coder->sim->log_mutex);
	timestamp = current_time_ms() - coder->sim->start_time_ms;
	if (!simulation_stopped(coder->sim))
		printf("%lld %lld %s\n", timestamp, coder->id, action);
	pthread_mutex_unlock(&coder->sim->log_mutex);
}
