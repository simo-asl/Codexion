/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 08:49:55 by mel-asla          #+#    #+#             */
/*   Updated: 2026/04/24 08:30:40 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_mutexes(t_table *table)
{
	if (pthread_mutex_init(&table->state_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&table->log_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&table->state_mutex);
		return (1);
	}
	if (pthread_mutex_init(&table->request_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&table->log_mutex);
		pthread_mutex_destroy(&table->state_mutex);
		return (1);
	}
	if (pthread_cond_init(&table->request_cond, NULL) != 0)
	{
		pthread_mutex_destroy(&table->request_mutex);
		pthread_mutex_destroy(&table->log_mutex);
		pthread_mutex_destroy(&table->state_mutex);
		return (1);
	}
	return (0);
}

int	init_request_queue(t_heap *heap, int capacity)
{
	heap->size = 0;
	heap->capacity = capacity;
	heap->items = (t_request *)malloc(sizeof(t_request) * capacity);
	if (!heap->items)
		return (1);
	return (0);
}

void	assign_dongles(t_table *table)
{
	int	i;
	int	right_index;

	i = 0;
	while (i < table->coder_count)
	{
		right_index = i + 1;
		if (right_index == table->coder_count)
			right_index = 0;
		table->coders[i].left_dongle = &table->dongles[i];
		table->coders[i].right_dongle = &table->dongles[right_index];
		i++;
	}
}
