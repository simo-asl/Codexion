/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla <marvin@42.fr>>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 11:33:09 by mel-asla          #+#    #+#             */
/*   Updated: 2026/06/19 13:44:16 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	is_number(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (false);
	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

int	parse_positive_int(const char *str, int *value)
{
	long long	result;

	if (!is_number(str))
		return (1);
	result = ft_atol(str);
	if (result <= 0 || result > INT_MAX)
		return (1);
	*value = (int)result;
	return (0);
}

int	parse_non_negative_int(const char *str, int *value)
{
	long long	result;

	if (!is_number(str))
		return (1);
	result = ft_atol(str);
	if (result < 0 || result > INT_MAX)
		return (1);
	*value = (int)result;
	return (0);
}
