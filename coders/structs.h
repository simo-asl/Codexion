/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 09:34:47 by mel-asla          #+#    #+#             */
/*   Updated: 2026/06/11 09:13:58 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include <pthread.h>

typedef struct s_parse_input	t_parse_input;
typedef struct s_settings		t_settings;
typedef struct s_dongle			t_dongle;
typedef struct s_queue			t_queue;
typedef struct s_coder			t_coder;
typedef struct s_runtime		t_runtime;

typedef enum e_scheduler_type
{
	SCH_FIFO = 0,
	SCH_EDF = 1
}	t_scheduler_type;

typedef struct s_parse_input
{
	t_scheduler_type	scheduler;
	int					coder_count;
	long				time_to_burnout;
	long				time_to_compile;
	long				time_to_debug;
	long				time_to_refactor;
	int					required_compiles;
	long				dongle_cooldown;

}	t_parse_input;

typedef struct s_settings
{
	t_scheduler_type	scheduler;
	long long			num_coders;
	long long			time_to_burnout;
	long long			time_to_compile;
	long long			time_to_debug;
	long long			time_to_refactor;
	long long			compiles_required;
	long long			dongle_cooldown;
}	t_settings;

typedef struct s_dongle
{
	pthread_mutex_t	queue_mutex;
	int				in_use;
	long long		cooldown;
	pthread_mutex_t	mutex;
	t_queue			*queue;
	int				queue_mutex_ready;
	int				mutex_ready;
}	t_dongle;

typedef struct s_queue
{
	t_scheduler_type	scheduler;
	t_coder				**items;
	int					max_size;
	int					size;
}	t_queue;

typedef struct s_coder
{
	long long		id;
	long long		burnout_deadline;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	long long		request_timestamp;
	pthread_t		thread;
	pthread_mutex_t	state_mutex;
	int				completed_compiles;
	t_runtime		*sim;
	int				state_mutex_ready;
}	t_coder;

typedef struct s_runtime
{
	t_settings		config;
	t_dongle		*dongles;
	t_coder			*coders;
	pthread_t		monitor_thread;
	long long		start_time_ms;
	int				stop_requested;
	pthread_mutex_t	request_mutex;
	pthread_cond_t	request_cond;
	pthread_mutex_t	log_mutex;
	int				request_mutex_ready;
	int				request_cond_ready;
	int				log_mutex_ready;
}	t_runtime;

#endif
