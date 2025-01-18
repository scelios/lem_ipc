/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beaudibe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 12:04:58 by beaudibe          #+#    #+#             */
/*   Updated: 2022/11/17 15:25:25 by beaudibe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	int		i;

	i = -1;
	if (!s || !len)
		return (0);
	if ((size_t) start >= ft_strlen(s))
	{
		str = malloc(1);
		if (!str)
			return (0);
		str[0] = '\0';
		return (str);
	}
	if (len > ft_strlen(s))
		len = ft_strlen(s) - start;
	str = malloc(1 + len);
	if (!s || !str)
		return (0);
	while ((size_t)++i < len)
		str[i] = s[start++];
	str[i] = '\0';
	return (str);
}
