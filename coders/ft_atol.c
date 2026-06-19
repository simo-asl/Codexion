/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla <marvin@42.fr>>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 00:08:11 by mel-asla          #+#    #+#             */
/*   Updated: 2026/06/19 13:45:20 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	ft_atol(const char *str)
{
	long long	res;
	int			i;
	int			digit;

	i = 0;
	res = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		digit = str[i] - '0';
		if (res > (LLONG_MAX - digit) / 10)
			return (LLONG_MAX);
		res = res * 10 + digit;
		i++;
	}
	return (res);
}
