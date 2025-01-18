/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base_printf.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beaudibe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 08:04:43 by beaudibe          #+#    #+#             */
/*   Updated: 2022/11/21 11:58:32 by beaudibe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_verif_base(char *b)
{
	int	i;
	int	j;

	i = 0;
	j = 1;
	while (*b)
	{
		if (*b == '+' || *b == '-' || (*b >= 0 && *b <= 31))
			return (0);
		while (*(b + j))
		{
			if (*b == *(b + j))
				return (0);
			j++;
		}
		j = 1;
		b++;
		i++;
	}
	return (i);
}

static int	ft_base(long long nb, char *base, int j)
{
	long long	len;

	len = ft_verif_base(base);
	if (len == 1 || !len)
		return (0);
	if (nb < 0)
	{
		nb = -nb;
		j += write(1, "-", 1);
	}
	if (nb < len)
		j += ft_putchar_printf(base[nb]);
	if (nb >= len)
	{
		j += ft_base(nb / len, base, 0);
		j += ft_base(nb % len, base, 0);
	}
	return (j);
}

int	ft_putnbr_base_printf(long long nbr, char *base, int a)
{
	int	j;

	if (a)
	{
		if (nbr < 0)
			nbr += 4294967296;
		return (ft_base(nbr, base, 0));
	}
	j = write(1, "0x", 2);
	j += ft_base((uintptr_t) nbr, base, 0);
	return (j);
}
