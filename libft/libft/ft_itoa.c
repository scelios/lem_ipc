/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beaudibe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 14:48:29 by beaudibe          #+#    #+#             */
/*   Updated: 2022/09/29 16:37:02 by beaudibe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa(int n)
{
	char	str[12];
	int		nbr;
	int		i;

	i = 0;
	nbr = n;
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n == 0)
		return (ft_strdup("0"));
	while (nbr && ++i)
		nbr /= 10;
	if (n < 0 && i++)
	{
		n = -n;
		str[0] = '-';
	}
	str[i] = '\0';
	while (n)
	{
		str[--i] = '0' + n % 10;
		n /= 10;
	}
	return (ft_strdup(str));
}
