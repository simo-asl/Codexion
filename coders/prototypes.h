/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prototypes.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 09:34:38 by mel-asla          #+#    #+#             */
/*   Updated: 2026/06/15 13:30:32 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTOTYPES_H
# define PROTOTYPES_H

# include "structs.h"

int				parse_args(t_parse_input *input, int argc, char **argv);
int				parse_scheduler(const char *str, t_scheduler_type *scheduler);
bool			is_number(const char *str);
long long		ft_atol(const char *str);
int				parse_positive_int(const char *str, int *value);
int				parse_non_negative_int(const char *str, int *value);

int				init_runtime(t_runtime *sim);

int				run_coders(t_runtime *sim);
void			*monitor_loop(void *arg);
void			*coder_thread(void *arg);

void			do_compile(t_coder *coder);
void			debug_and_refactor(t_coder *coder);
int				acquire_dongles(t_coder *coder);
void			release_dongles(t_coder *coder);

bool			is_top_of_heap(t_dongle *dongle, t_coder *coder);
void			lock_order(t_coder *coder, t_dongle **first, t_dongle **second);
void			cleanup_heaps_locked(t_coder *coder);

int				join_coder_threads(t_runtime *sim, int end);
bool			need_to_stop(t_runtime *sim);
void			request_stop(t_runtime *sim);

void			cleanup_all(t_runtime *sim);
void			cleanup_dongles(t_runtime *sim, int num_coders);

long long		get_time_in_ms(void);
void			smart_sleep(long long time_to_sleep, t_runtime *sim);

void			log_event(t_coder *coder, const char *action);

t_queue			*heap_init(int max_size, t_scheduler_type scheduler);
bool			heap_is_empty(t_queue *queue);
bool			heap_is_full(t_queue *queue);
int				heap_insert(t_queue *queue, t_coder *coder);
void			bubble_up(t_queue *queue, int index);
void			bubble_down(t_queue *queue, int index, int size);
void			heap_remove_at(t_queue *queue, int index);
int				heap_find_index(t_queue *queue, t_coder *coder);
void			lock_dongles(t_dongle *first, t_dongle *second, int heap_lock);
void			unlock_dongles(t_dongle *first,
					t_dongle *second, int heap_lock);

#endif
