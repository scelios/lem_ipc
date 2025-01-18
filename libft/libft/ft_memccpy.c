/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beaudibe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 09:42:53 by beaudibe          #+#    #+#             */
/*   Updated: 2022/09/28 13:45:45 by beaudibe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	size_t	i;

	i = -1;
	if (!dst && !src)
		return (0);
	while (++i < n && ((unsigned char *)src)[i] != (unsigned char)c)
		((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
	if ((unsigned char)c == ((unsigned char *)src)[i])
	{
		((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
		return (dst + i + 1);
	}
	return (0);
}
