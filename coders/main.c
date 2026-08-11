/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 09:02:33 by mel-asla          #+#    #+#             */
/*   Updated: 2026/06/15 14:50:27 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	number(const char *text, int positive, int *value)
{
	long long	n;
	int			i;

	if (!text || !text[0])
		return (1);
	n = 0;
	i = 0;
	while (text[i])
	{
		if (text[i] < '0' || text[i] > '9')
			return (1);
		if (n > (INT_MAX - (text[i] - '0')) / 10)
			return (1);
		n = n * 10 + text[i++] - '0';
	}
	if ((positive && n == 0) || n > INT_MAX)
		return (1);
	*value = (int)n;
	return (0);
}

int	parse_arguments(t_config *c, int ac, char **av)
{
	if (ac != 9 || number(av[1], 1, &c->number)
		|| number(av[2], 0, &c->burnout)
		|| number(av[3], 0, &c->compile)
		|| number(av[4], 0, &c->debug)
		|| number(av[5], 0, &c->refactor)
		|| number(av[6], 1, &c->required)
		|| number(av[7], 0, &c->cooldown))
		return (1);
	if (!strcmp(av[8], "fifo"))
		c->edf = 0;
	else if (!strcmp(av[8], "edf"))
		c->edf = 1;
	else
		return (1);
	return (0);
}

int	main(int ac, char **av)
{
	t_sim	sim;

	memset(&sim, 0, sizeof(sim));
	if (parse_arguments(&sim.cfg, ac, av))
	{
		write(2, "Error: invalid arguments\n", 25);
		return (1);
	}
	if (initialize_simulation(&sim) || run_simulation(&sim))
	{
		destroy_simulation(&sim);
		write(2, "Error: simulation failed\n", 25);
		return (1);
	}
	destroy_simulation(&sim);
	return (0);
}
