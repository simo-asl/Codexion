/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 09:02:33 by mel-asla          #+#    #+#             */
/*   Updated: 2026/04/24 04:05:17 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_table	table;

	if (init_table(&table, argc, argv) != 0)
	{
		fprintf(stderr, "Error: invalid arguments or initialization failure\n");
		return (1);
	}
	if (start_simulation(&table) != 0)
	{
		fprintf(stderr, "Error: simulation failure\n");
		destroy_mutexes(&table);
		free_table(&table);
		return (1);
	}
	destroy_mutexes(&table);
	free_table(&table);
	return (0);
}
