/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 09:34:47 by mel-asla          #+#    #+#             */
/*   Updated: 2026/05/01 17:58:07 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include <pthread.h>
# include <stdbool.h>
# include <time.h>

typedef struct timespec	t_timespec;

typedef enum e_scheduler_type
{
	SCHED_TYPE_FIFO = 0,
	SCHED_TYPE_EDF = 1
}	t_scheduler_type;

typedef enum e_coder_state
{
	STATE_IDLE = 0,
	STATE_COMPILING = 1,
	STATE_DEBUGGING = 2,
	STATE_REFACTORING = 3
}	t_coder_state;

typedef struct s_request
{
	int					coder_id;
	long				deadline_ms;
	long				sequence;
}	t_request;

typedef struct s_heap
{
	t_request			*items;
	int					size;
	int					capacity;
}	t_heap;

typedef struct s_dongle
{
	pthread_mutex_t		mutex;
	bool				in_use;
	long				available_at_ms;
}	t_dongle;

typedef struct s_table	t_table;

typedef struct s_coder
{
	int					id;
	t_table				*table;
	t_dongle			*left_dongle;
	t_dongle			*right_dongle;
	pthread_t			thread;
	long				last_compile_start_ms;
	int					compiles_done;
	bool				in_wait_queue;
	t_coder_state		state;
	int					last_round;
	int					compiles_in_round;
}	t_coder;

struct s_table
{
	int					coder_count;
	long				time_to_burnout;
	long				time_to_compile;
	long				time_to_debug;
	long				time_to_refactor;
	int					required_compiles;
	long				dongle_cooldown;
	t_scheduler_type	scheduler;
	long				start_time_ms;
	bool				simulation_end;
	long				next_sequence;
	t_coder				*coders;
	t_dongle			*dongles;
	t_heap				request_heap;
	pthread_t			monitor_thread;
	pthread_mutex_t		state_mutex;
	pthread_mutex_t		log_mutex;
	pthread_mutex_t		request_mutex;
	pthread_cond_t		request_cond;
	pthread_mutex_t		start_mutex;
	pthread_cond_t		start_cond;
	bool				start_released;
	int					round;
	int					completed_in_round;
};

#endif
