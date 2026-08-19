/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla <marvin@42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 21:56:33 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/19 19:37:14 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	parse_number(const char *text, int positive, int *value)
{
	long long	n;
	int			i;

	if (!text || !text[0])
		return (FAIL);
	n = 0;
	i = 0;
	while (text[i])
	{
		if (text[i] < '0' || text[i] > '9')
			return (FAIL);
		if (n > (INT_MAX - (text[i] - '0')) / 10)
			return (FAIL);
		n = n * 10 + text[i++] - '0';
	}
	if ((positive && n == 0) || n > INT_MAX)
		return (FAIL);
	*value = (int)n;
	return (SUCCESS);
}

int	parse_arguments(t_config *config, int ac, char **av)
{
	if (ac != 9 || parse_number(av[1], 1, &config->coder_count)
		|| parse_number(av[2], 0, &config->time_to_burnout)
		|| parse_number(av[3], 0, &config->time_to_compile)
		|| parse_number(av[4], 0, &config->time_to_debug)
		|| parse_number(av[5], 0, &config->time_to_refactor)
		|| parse_number(av[6], 1, &config->compiles_required)
		|| parse_number(av[7], 0, &config->dongle_cooldown))
		return (FAIL);
	if (!strcmp(av[8], "fifo"))
		config->is_edf = FIFO;
	else if (!strcmp(av[8], "edf"))
		config->is_edf = EDF;
	else
		return (FAIL);
	return (SUCCESS);
}

static int	prepare_app(t_sim *sim, int argc, char **argv)
{
	if (parse_arguments(&sim->config, argc, argv) != SUCCESS)
	{
		write(2, "Error: invalid arguments\n", 25);
		return (FAIL);
	}
	if (initialize_simulation(sim) != SUCCESS)
	{
		write(2, "Error: initialization failed\n", 29);
		return (FAIL);
	}
	return (SUCCESS);
}

static int	run_app(int argc, char **argv)
{
	t_sim	*sim;

	sim = malloc(sizeof(t_sim));
	if (!sim)
	{
		write(2, "Error: sim allocation failed\n", 29);
		return (FAIL);
	}
	memset(sim, 0, sizeof(t_sim));
	if (prepare_app(sim, argc, argv) != SUCCESS)
	{
		destroy_simulation(sim);
		free(sim);
		return (FAIL);
	}
	if (run_simulation(sim) != SUCCESS)
	{
		destroy_simulation(sim);
		free(sim);
		write(2, "Error: simulation failed\n", 25);
		return (FAIL);
	}
	destroy_simulation(sim);
	free(sim);
	return (SUCCESS);
}

int	main(int argc, char **argv)
{
	if (run_app(argc, argv) == 0)
		return (SUCCESS);
	return (FAIL);
}
