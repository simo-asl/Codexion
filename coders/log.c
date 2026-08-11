/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 10:02:17 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/09 20:55:00 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_coder_state(t_coder *coder, const char *text)
{
	long long	stamp;

	pthread_mutex_lock(&coder->sim->log);
	pthread_mutex_lock(&coder->sim->state);
	stamp = current_time_ms() - coder->sim->start;
	if (!coder->sim->stop)
		printf("%lld %d %s\n", stamp, coder->id, text);
	pthread_mutex_unlock(&coder->sim->state);
	pthread_mutex_unlock(&coder->sim->log);
}
