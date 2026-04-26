/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 19:21:48 by mel-asla          #+#    #+#             */
/*   Updated: 2026/04/26 14:33:23 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	validate_args(int argc, char **argv)
{
	int	idx;

	if (argc != 9)
		return (1);
	idx = 1;
	while (idx <= 7)
	{
		if (!is_number(argv[idx]))
			return (1);
		idx++;
	}
	if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
		return (1);
	return (0);
}

int	parse_scheduler(const char *str, t_scheduler_type *scheduler)
{
	if (strcmp(str, "fifo") == 0)
	{
		*scheduler = SCHED_TYPE_FIFO;
		return (0);
	}
	if (strcmp(str, "edf") == 0)
	{
		*scheduler = SCHED_TYPE_EDF;
		return (0);
	}
	return (1);
}

int	parse_args(t_table *table, int argc, char **argv)
{
	if (validate_args(argc, argv) != 0)
		return (1);
	if (parse_positive_int(argv[1], &table->coder_count) != 0)
		return (1);
	if (parse_positive_long(argv[2], &table->time_to_burnout) != 0)
		return (1);
	if (parse_positive_long(argv[3], &table->time_to_compile) != 0)
		return (1);
	if (parse_positive_long(argv[4], &table->time_to_debug) != 0)
		return (1);
	if (parse_positive_long(argv[5], &table->time_to_refactor) != 0)
		return (1);
	if (parse_positive_int(argv[6], &table->required_compiles) != 0)
		return (1);
	if (parse_non_negative_long(argv[7], &table->dongle_cooldown) != 0)
		return (1);
	if (parse_scheduler(argv[8], &table->scheduler) != 0)
		return (1);
	return (0);
}
