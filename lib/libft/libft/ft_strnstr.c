/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beaudibe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 09:43:07 by beaudibe          #+#    #+#             */
/*   Updated: 2022/11/17 15:26:02 by beaudibe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str, const char *s, size_t n)
{
	size_t	i;
	size_t	j;

	i = -1;
	if (!str || !s)
		return (0);
	if (!s[0])
		return ((char *) str);
	while (str[++i] && i < n)
	{
		j = 0;
		while (str[i + j] == s[j] && str[i + j] && i + j < n)
			j++;
		if (s[j] == '\0')
			return ((char *) str + i);
	}
	return (NULL);
}
