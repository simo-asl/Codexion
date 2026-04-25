/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 11:33:09 by mel-asla          #+#    #+#             */
/*   Updated: 2026/04/25 00:54:34 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_number(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

long	ft_atol(const char *str)
{
	long long	result;
	int			i;

	if (!is_number(str))
		return (-1);
	result = 0;
	i = 0;
	while (str[i])
	{
		result = result * 10 + (str[i] - '0');
		if (result > LONG_MAX)
			return (-1);
		i++;
	}
	return ((long)result);
}

int	parse_positive_int(const char *str, int *value)
{
	long	result;

	result = ft_atol(str);
	if (result <= 0 || result > INT_MAX)
		return (1);
	*value = (int)result;
	return (0);
}

int	parse_positive_long(const char *str, long *value)
{
	long	result;

	result = ft_atol(str);
	if (result <= 0)
		return (1);
	*value = result;
	return (0);
}

int	parse_non_negative_long(const char *str, long *value)
{
	long	result;

	result = ft_atol(str);
	if (result < 0)
		return (1);
	*value = result;
	return (0);
}
