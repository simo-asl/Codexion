/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prototypes.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 09:34:38 by mel-asla          #+#    #+#             */
/*   Updated: 2026/03/19 09:34:41 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTOTYPES_H
# define PROTOTYPES_H

# include "structs.h"

int		parse_args(t_table *table, int argc, char **argv);
int		parse_scheduler(const char *str, t_scheduler_type *scheduler);
int		validate_args(int argc, char **argv);
int		parse_positive_int(const char *str, int *value);
int		parse_positive_long(const char *str, long *value);
int		parse_non_negative_int(const char *str, int *value);
int		parse_non_negative_long(const char *str, long *value);
int		is_number(const char *str);

int		init_table(t_table *table, int argc, char **argv);
int		init_coders(t_table *table);
int		init_dongles(t_table *table);
int		init_mutexes(t_table *table);
int		init_request_queue(t_heap *heap, int capacity);
void	assign_dongles(t_table *table);

int		start_simulation(t_table *table);
void	*coder_routine(void *arg);
int		coder_compile(t_coder *coder);
int		coder_debug(t_coder *coder);
int		coder_refactor(t_coder *coder);
int		try_start_compile(t_coder *coder);

int		take_dongles(t_coder *coder);
void	release_dongles(t_coder *coder);
int		lock_dongle(t_dongle *dongle);
void	unlock_dongle(t_dongle *dongle);
bool	dongle_is_available(t_dongle *dongle, long now);
void	set_dongle_cooldown(t_dongle *dongle, long cooldown, long now);

int		request_dongles(t_coder *coder);
void	remove_request(t_table *table, int coder_id);
bool	can_grant_request(t_table *table, t_coder *coder);
int		compare_requests(t_request a, t_request b, t_scheduler_type type);

int		heap_push(t_heap *heap, t_request request, t_scheduler_type type);
int		heap_pop(t_heap *heap, t_request *out, t_scheduler_type type);
t_request	heap_peek(t_heap *heap);
void	heapify_up(t_heap *heap, int index, t_scheduler_type type);
void	heapify_down(t_heap *heap, int index, t_scheduler_type type);
void	swap_requests(t_request *a, t_request *b);
void	free_heap(t_heap *heap);

void	*monitor_routine(void *arg);
int		check_burnout(t_coder *coder);
bool	all_compiles_completed(t_table *table);
bool	simulation_should_end(t_table *table);
void	set_simulation_end(t_table *table, bool value);
bool	get_simulation_end(t_table *table);

void	print_status(t_coder *coder, const char *msg);
void	print_burnout(t_coder *coder);

long	get_timestamp_ms(void);
long	get_elapsed_time_ms(long start_time);
void	precise_sleep(long duration_ms, t_table *table);

void	destroy_mutexes(t_table *table);
void	free_table(t_table *table);
void	cleanup_and_exit(t_table *table, int exit_code);

#endif
