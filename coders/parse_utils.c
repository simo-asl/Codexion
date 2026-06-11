/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 11:33:09 by mel-asla          #+#    #+#             */
/*   Updated: 2026/06/11 09:14:48 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	is_number(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (false);
	i = 0;
	if (str[i] == '+')
		i++;
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

int	parse_positive_long(const char *str, long *value)
{
	long long	result;

	if (!is_number(str))
		return (1);
	result = ft_atol(str);
	if (result <= 0)
		return (1);
	*value = (long)result;
	return (0);
}

int	parse_non_negative_long(const char *str, long *value)
{
	long long	result;

	if (!is_number(str))
		return (1);
	result = ft_atol(str);
	if (result < 0)
		return (1);
	*value = (long)result;
	return (0);
}
