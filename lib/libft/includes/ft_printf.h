/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beaudibe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 09:31:50 by beaudibe          #+#    #+#             */
/*   Updated: 2022/11/21 12:11:38 by beaudibe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "../libft/includes/libft.h"
# include "stdio.h"
# include <limits.h>
# include <stdarg.h>
#include <stdint.h>

int	ft_printf(const char *str, ...);
int	ft_putstr_printf(char *s);
int	ft_putnbr_base_printf(long long nbr, char *base, int a);
int	ft_putchar_printf(char c);
int	ft_putnbr_printf(long long n, int a);

#endif
