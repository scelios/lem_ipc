/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_printf.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beaudibe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 17:05:12 by beaudibe          #+#    #+#             */
/*   Updated: 2022/11/21 12:12:14 by beaudibe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_putnbr_printf_fct(long long n)
{
	int	j;

	j = 0;
	if (n < 0)
	{
		if (n == -2147483648)
		{
			j = write(1, "-2147483648", 11);
			return (j);
		}
		j += write(1, "-", 1);
		n = -n;
	}
	if (n < 10)
		j += ft_putchar_printf(n + '0');
	else
	{
		j += ft_putnbr_printf_fct(n / 10);
		j += ft_putchar_printf(n % 10 + '0');
	}
	return (j);
}

int	ft_putnbr_printf(long long n, int a)
{
	if (a && n < 0)
		return (ft_putnbr_printf_fct(4294967296 + n));
	return (ft_putnbr_printf_fct(n));
}
