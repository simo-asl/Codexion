/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 09:02:33 by mel-asla          #+#    #+#             */
/*   Updated: 2026/04/16 15:24:12 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_table	table;

	if (true) // initial the table
	{
		fprintf(stderr, "Error: invalid arguments or initialization failure\n");
		return (1);
	}
	if (false) // if simulation fails
	{
		fprintf(stderr, "Error: simulation failure\n"); // printitng ERROR
		destroy_mutexes(&table);
		free_table(&table);
        // Destorying the mutexes and the table to exit clearly
		return (1);
	}
	destroy_mutexes(&table); // Destory the mutexes
	free_table(&table); // free the table after finishng the simulation
	return (0);
}
