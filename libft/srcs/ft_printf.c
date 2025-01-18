/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beaudibe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 10:35:57 by beaudibe          #+#    #+#             */
/*   Updated: 2022/11/21 12:13:06 by beaudibe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_assign_type(char c, va_list list, int j)
{
	if (c == 'd')
		j = ft_putnbr_printf(va_arg(list, int), 0);
	if (c == 'i')
		j = ft_putnbr_printf(va_arg(list, int), 0);
	if (c == 'u')
		j = ft_putnbr_printf(va_arg(list, unsigned int), 1);
	if (c == 'x')
		j = ft_putnbr_base_printf(va_arg(list, int), "0123456789abcdef", 1);
	if (c == 'X')
		j = ft_putnbr_base_printf(va_arg(list, int), "0123456789ABCDEF", 1);
	if (c == 'p')
		j += ft_putnbr_base_printf(va_arg(list, uintptr_t), \
			"0123456789abcdef", 0);
	if (c == 's')
		j = ft_putstr_printf(va_arg(list, char *));
	if (c == 'c')
		j = ft_putchar_printf(va_arg(list, int));
	if (c == '%')
		j = write(1, "%", 1);
	return (j);
}

int	ft_printf(const char *str, ...)
{
	va_list	list;
	int		i;
	int		j;
	int		temp;

	i = 0;
	j = 0;
	if (!str)
		return (0);
	va_start (list, str);
	while (str[i])
	{
		while (str[i] && str[i] != '%' && ++j)
			write(1, &str[i++], 1);
		if (str[i++] != '%')
			break ;
		temp = ft_assign_type(str[i++], list, 0);
		if (temp == -1)
			return (0);
		j += temp;
	}
	va_end (list);
	return (j);
}
