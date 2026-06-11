/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 09:02:33 by mel-asla          #+#    #+#             */
/*   Updated: 2026/06/11 09:14:51 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	convert_table_to_config(t_runtime *sim, t_parse_input *input)
{
	sim->config.num_coders = (long long)input->coder_count;
	sim->config.time_to_burnout = (long long)input->time_to_burnout;
	sim->config.time_to_compile = (long long)input->time_to_compile;
	sim->config.time_to_debug = (long long)input->time_to_debug;
	sim->config.time_to_refactor = (long long)input->time_to_refactor;
	sim->config.compiles_required = (long long)input->required_compiles;
	sim->config.dongle_cooldown = (long long)input->dongle_cooldown;
	sim->config.scheduler = input->scheduler;
	return (true);
}

static bool	prepare_simulation(t_runtime *sim, int argc, char **argv)
{
	t_parse_input	input;

	memset(&input, 0, sizeof(t_parse_input));
	if (parse_args(&input, argc, argv) != 0)
	{
		fprintf(stderr, "Error: Invalid arguments.\n");
		return (false);
	}
	if (!convert_table_to_config(sim, &input))
	{
		fprintf(stderr, "Error: Failed to start simulation.\n");
		return (false);
	}
	if (init_runtime(sim) != 0)
	{
		fprintf(stderr, "Error: Failed to initialize runtime.\n");
		return (false);
	}
	return (true);
}

static bool	run_simulation(int argc, char **argv)
{
	t_runtime	*sim;

	sim = malloc(sizeof(t_runtime));
	if (!sim)
		return (false);
	memset(sim, 0, sizeof(t_runtime));
	if (!prepare_simulation(sim, argc, argv))
	{
		cleanup_all(sim);
		return (false);
	}
	if (run_coders(sim) != 0)
	{
		cleanup_all(sim);
		return (false);
	}
	cleanup_all(sim);
	return (true);
}

int	main(int argc, char **argv)
{
	if (argc != 9)
	{
		fprintf(stderr, USAGE_MSG, argv[0]);
		return (1);
	}
	if (run_simulation(argc, argv))
		return (0);
	return (1);
}
