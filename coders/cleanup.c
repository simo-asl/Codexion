/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 08:12:47 by mel-asla          #+#    #+#             */
/*   Updated: 2026/04/15 11:43:13 by mel-asla         ###   ########.fr       */
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

void	cleanup_and_exit(t_table *table, int exit_code)
{
	(void)exit_code;
	destroy_mutexes(table);
	free_table(table);
}