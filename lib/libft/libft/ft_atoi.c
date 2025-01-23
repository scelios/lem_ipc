/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beaudibe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 10:24:04 by beaudibe          #+#    #+#             */
/*   Updated: 2022/11/10 16:28:50 by beaudibe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return ((unsigned int)(s1[i] - s2[i]));
}

static	int	ft_next_digit(char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] >= '0' && s[i] <= '9')
		i++;
	return (i);
}

int	ft_atoi(const char *n)
{
	int	nbr;
	int	sign;
	int	i;

	i = 0;
	nbr = 0;
	sign = 1;
	while (n[i] && ((n[i] >= 9 && n[i] <= 13) || n[i] == ' '))
		i++;
	if (n[i] == '+' || n[i] == '-')
	{
		if (n[i] == '-')
			sign = -1;
		i++;
	}
	if (n[i] >= '0' && n[i] <= '9' && ft_next_digit((char *)(n + i)) >= 18
		&& ft_strcmp((char *)(n + i), "9223372036854775808") >= 0)
	{
		if (sign == 1)
			return (-1);
		return (0);
	}
	while (n[i] >= '0' && n[i] <= '9')
		nbr = (nbr * 10) + (n[i++] - '0');
	return (nbr * sign);
}
