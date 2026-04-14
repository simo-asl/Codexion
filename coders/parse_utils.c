/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 11:33:09 by mel-asla          #+#    #+#             */
/*   Updated: 2026/04/14 18:32:53 by mel-asla         ###   ########.fr       */
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

int	parse_positive_int(const char *str, int *value)
{
	long	result;
	int		i;

	if (!is_number(str))
		return (1);
	result = 0;
	i = 0;
	while (str[i])
	{
		result = result * 10 + (str[i] - '0');
		if (result > 2147483647)
			return (1);
		i++;
	}
	if (result <= 0)
		return (1);
	*value = (int)result;
	return (0);
}

int	parse_non_negative_int(const char *str, int *value)
{
	long	result;
	int		i;

	if (!is_number(str))
		return (1);
	result = 0;
	i = 0;
	while (str[i])
	{
		result = result * 10 + (str[i] - '0');
		if (result > 2147483647)
			return (1);
		i++;
	}
	*value = (int)result;
	return (0);
}

int	parse_positive_long(const char *str, long *value)
{
	long long	result;
	int			i;

	if (!is_number(str))
		return (1);
	result = 0;
	i = 0;
	while (str[i])
	{
		result = result * 10 + (str[i] - '0');
		if (result > 9223372036854775807LL)
			return (1);
		i++;
	}
	if (result <= 0)
		return (1);
	*value = (long)result;
	return (0);
}

int	parse_non_negative_long(const char *str, long *value)
{
	long long	result;
	int			i;

	if (!is_number(str))
		return (1);
	result = 0;
	i = 0;
	while (str[i])
	{
		result = result * 10 + (str[i] - '0');
		if (result > 9223372036854775807LL)
			return (1);
		i++;
	}
	*value = (long)result;
	return (0);
}
