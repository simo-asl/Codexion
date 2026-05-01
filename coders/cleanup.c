/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 08:12:47 by mel-asla          #+#    #+#             */
/*   Updated: 2026/05/01 10:16:58 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	destroy_mutexes(t_table *table)
{
	int	i;

	if (!table)
		return ;
	i = 0;
	while (table->dongles && i < table->coder_count)
	{
		pthread_mutex_destroy(&table->dongles[i].mutex);
		i++;
	}
	pthread_mutex_destroy(&table->state_mutex);
	pthread_mutex_destroy(&table->log_mutex);
	pthread_mutex_destroy(&table->request_mutex);
	pthread_cond_destroy(&table->request_cond);
	pthread_mutex_destroy(&table->start_mutex);
	pthread_cond_destroy(&table->start_cond);
}

void	free_table(t_table *table)
{
	if (!table)
		return ;
	free_heap(&table->request_heap);
	free(table->dongles);
	free(table->coders);
	table->dongles = NULL;
	table->coders = NULL;
}
