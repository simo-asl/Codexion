/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 19:16:10 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/14 14:07:22 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define FIFO 0
# define EDF 1
# define SUCCESS 0
# define FAIL 1

typedef struct s_coder	t_coder;
typedef struct s_heap	t_heap;

typedef struct s_config
{
	int	coder_count;
	int	time_to_burnout;
	int	time_to_compile;
	int	time_to_debug;
	int	time_to_refactor;
	int	compiles_required;
	int	dongle_cooldown;
	int	is_edf;
}	t_config;

typedef struct s_dongle
{
	int				busy;
	long long		ready_at;
	pthread_mutex_t	mutex;
	t_heap			*queue;
	pthread_mutex_t	queue_mutex;
	int				queue_mutex_ready;
	int				mutex_ready;
}	t_dongle;

struct s_heap
{
	t_coder	**item;
	int		capacity;
	int		size;
	int		is_edf;
};

typedef struct s_sim
{
	t_config		config;
	t_dongle		*dongles;
	t_coder			*coders;
	pthread_t		monitor_thread;
	long long		start_time_ms;
	int				stop_requested;
	pthread_mutex_t	state_mutex;
	pthread_cond_t	start_condition;
	pthread_mutex_t	log_mutex;
	int				state_mutex_ready;
	int				start_condition_ready;
	int				log_mutex_ready;
}	t_sim;

struct s_coder
{
	long long		id;
	long long		deadline;
	t_dongle		*left;
	t_dongle		*right;
	long long		requested_at;
	pthread_t		thread;
	pthread_mutex_t	state_mutex;
	int				compile_count;
	t_sim			*sim;
	int				state_mutex_ready;
};

int			parse_arguments(t_config *config, int ac, char **av);
int			initialize_simulation(t_sim *sim);
int			run_simulation(t_sim *sim);
void		destroy_simulation(t_sim *sim);
void		*monitor_thread(void *argument);
void		*coder_thread(void *argument);

void		compile_code(t_coder *coder);
void		do_other_work(t_coder *coder);
int			request_dongles(t_coder *coder);
void		release_dongles(t_coder *coder);
int			is_queue_head(t_dongle *dongle, t_coder *coder);
void		order_dongles(t_coder *coder, t_dongle **first,
				t_dongle **second);
void		remove_from_queues(t_coder *coder);
void		lock_dongles(t_dongle *first, t_dongle *second,
				int lock_queue);
void		unlock_dongles(t_dongle *first, t_dongle *second,
				int lock_queue);

t_heap		*create_heap(int capacity, int is_edf);
int			heap_is_empty(t_heap *heap);
int			heap_is_full(t_heap *heap);
int			heap_push(t_heap *heap, t_coder *coder);
void		heap_up(t_heap *heap, int index);
void		heap_down(t_heap *heap, int index, int size);
void		heap_remove_index(t_heap *heap, int index);
int			heap_find_index(t_heap *heap, t_coder *coder);

int			join_coder_threads(t_sim *sim, int end);
int			simulation_stopped(t_sim *sim);
void		stop_simulation(t_sim *sim);
void		cleanup_dongles(t_sim *sim, int coder_count);
long long	current_time_ms(void);
void		interruptible_sleep(long long duration, t_sim *sim);
void		print_coder_state(t_coder *coder, const char *action);

#endif
