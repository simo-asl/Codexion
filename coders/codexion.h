/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla <marvin@42.fr>>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 09:34:01 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/12 01:53:44 by mel-asla         ###   ########.fr       */
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
# include <time.h>
# include <unistd.h>

typedef struct s_sim		t_sim;
typedef struct s_coder		t_coder;

typedef struct s_heap
{
	t_coder	**item;
	int		size;
	int		capacity;
}	t_heap;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	changed;
	t_heap			queue;
	long long		ready_at;
	unsigned long	version;
	int				busy;
	int				mutex_ready;
	int				cond_ready;
}	t_dongle;

typedef struct s_config
{
	int	number;
	int	burnout;
	int	compile;
	int	debug;
	int	refactor;
	int	required;
	int	cooldown;
	int	edf;
}	t_config;

struct s_coder
{
	int			id;
	pthread_t	thread;
	t_dongle	*left;
	t_dongle	*right;
	t_sim		*sim;
	long long	deadline;
	long long	compile_start;
	long long	ticket;
	int			compiled;
};

struct s_sim
{
	t_config		cfg;
	t_coder			*coders;
	t_dongle		*dongles;
	pthread_t		monitor;
	pthread_mutex_t	state;
	pthread_mutex_t	log;
	pthread_cond_t	changed;
	long long		start;
	long long		next_ticket;
	int				stop;
	int				state_ready;
	int				log_ready;
	int				cond_ready;
};

int			parse_arguments(t_config *cfg, int ac, char **av);
int			initialize_simulation(t_sim *sim);
int			run_simulation(t_sim *sim);
void		destroy_simulation(t_sim *sim);

void		*coder_thread(void *arg);
void		*monitor_thread(void *arg);
void		wait_for_start(t_coder *coder);
void		startup_delay(t_coder *coder);

int			request_dongles(t_coder *coder);
void		release_dongles(t_coder *coder);
void		lock_dongle_pair(t_coder *coder);
void		unlock_dongle_pair(t_coder *coder);
void		leave_queues(t_coder *coder);
void		notify_dongle_waiters(t_dongle *dongle);
t_dongle	*select_blocking_dongle(t_coder *coder, long long now,
				unsigned long *version, long long *until);
void		wait_on_dongle(t_dongle *dongle, unsigned long version,
				long long until);

int			has_higher_priority(t_coder *a, t_coder *b, int edf);
int			heap_push(t_heap *heap, t_coder *coder, int edf);
void		heap_remove(t_heap *heap, t_coder *coder, int edf);
void		heap_up(t_heap *heap, int i, int edf);
void		heap_down(t_heap *heap, int i, int edf);

long long	current_time_ms(void);
void		interruptible_sleep(t_sim *sim, long long duration_ms);
void		timed_wait_until(t_sim *sim, long long wake_at);
int			simulation_stopped(t_sim *sim);
void		stop_simulation(t_sim *sim);
void		print_coder_state(t_coder *coder, const char *text);

#endif
