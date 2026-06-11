/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logs_state.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 03:08:31 by mel-asla          #+#    #+#             */
/*   Updated: 2026/06/11 09:14:59 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	do_compile(t_coder *coder)
{
	if (need_to_stop(coder->sim))
		return ;
	pthread_mutex_lock(&coder->state_mutex);
	coder->burnout_deadline = get_time_in_ms() + \
coder->sim->config.time_to_burnout;
	pthread_mutex_unlock(&coder->state_mutex);
	log_event(coder, "is compiling");
	smart_sleep(coder->sim->config.time_to_compile, coder->sim);
	pthread_mutex_lock(&coder->state_mutex);
	coder->completed_compiles++;
	pthread_mutex_unlock(&coder->state_mutex);
	release_dongles(coder);
}

void	debug_and_refactor(t_coder *coder)
{
	log_event(coder, "is debugging");
	smart_sleep(coder->sim->config.time_to_debug, coder->sim);
	if (need_to_stop(coder->sim))
		return ;
	log_event(coder, "is refactoring");
	smart_sleep(coder->sim->config.time_to_refactor, coder->sim);
}
