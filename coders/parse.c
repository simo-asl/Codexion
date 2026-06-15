/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 19:21:48 by mel-asla          #+#    #+#             */
/*   Updated: 2026/06/15 13:32:44 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	validate_args(int argc, char **argv)
{
	int	idx;

	if (argc != 9)
		return (false);
	idx = 1;
	while (idx <= 7)
	{
		if (!is_number(argv[idx]))
			return (false);
		idx++;
	}
	if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
		return (false);
	return (true);
}

int	parse_scheduler(const char *str, t_scheduler_type *scheduler)
{
	if (strcmp(str, "fifo") == 0)
	{
		*scheduler = SCH_FIFO;
		return (0);
	}
	if (strcmp(str, "edf") == 0)
	{
		*scheduler = SCH_EDF;
		return (0);
	}
	return (1);
}

int	parse_args(t_parse_input *input, int argc, char **argv)
{
	if (!validate_args(argc, argv))
		return (1);
	if (parse_positive_int(argv[1], &input->coder_count) != 0)
		return (1);
	if (parse_non_negative_int(argv[2], &input->time_to_burnout) != 0)
		return (1);
	if (parse_non_negative_int(argv[3], &input->time_to_compile) != 0)
		return (1);
	if (parse_non_negative_int(argv[4], &input->time_to_debug) != 0)
		return (1);
	if (parse_non_negative_int(argv[5], &input->time_to_refactor) != 0)
		return (1);
	if (parse_positive_int(argv[6], &input->required_compiles) != 0)
		return (1);
	if (parse_non_negative_int(argv[7], &input->dongle_cooldown) != 0)
		return (1);
	if (parse_scheduler(argv[8], &input->scheduler) != 0)
		return (1);
	return (0);
}
