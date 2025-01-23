/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beaudibe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 08:04:43 by beaudibe          #+#    #+#             */
/*   Updated: 2022/11/20 09:43:42 by beaudibe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int		ft_verif_base(char *b);

void	ft_base(long nbr, char *base);

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

int	ft_verif_base(char *b)
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

void	ft_base(long nb, char *base)
{
	int	len;

	len = ft_verif_base(base);
	if (len == 1 || !len)
		return ;
	if (nb < 0)
	{
		nb = -nb;
		write(1, "-", 1);
	}
	if (nb < len)
		ft_putchar(base[nb]);
	if (nb >= len)
	{
		ft_base(nb / len, base);
		ft_base(nb % len, base);
	}
}

void	ft_putnbr_base(int nbr, char *base)
{
	ft_base((long) nbr, base);
}
