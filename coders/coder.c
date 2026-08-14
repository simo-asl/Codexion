/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 12:32:46 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/14 13:37:12 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	compile_code(t_coder *coder)
{
	if (simulation_stopped(coder->sim))
		return ;
	pthread_mutex_lock(&coder->state_mutex);
	coder->deadline = current_time_ms()
		+ coder->sim->config.time_to_burnout;
	pthread_mutex_unlock(&coder->state_mutex);
	print_coder_state(coder, "is compiling");
	interruptible_sleep(coder->sim->config.time_to_compile, coder->sim);
	pthread_mutex_lock(&coder->state_mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->state_mutex);
	release_dongles(coder);
}

void	do_other_work(t_coder *coder)
{
	print_coder_state(coder, "is debugging");
	interruptible_sleep(coder->sim->config.time_to_debug, coder->sim);
	if (simulation_stopped(coder->sim))
		return ;
	print_coder_state(coder, "is refactoring");
	interruptible_sleep(coder->sim->config.time_to_refactor, coder->sim);
}
