/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beaudibe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 19:20:40 by beaudibe          #+#    #+#             */
/*   Updated: 2022/11/17 15:37:21 by beaudibe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_countw(const char *str, char sp)
{
	int	i;
	int	k;

	k = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && sp == str[i])
			i++;
		if (str[i] && sp != str[i])
		{
			k++;
			while (str[i] && sp != str[i])
				i++;
		}
	}
	return (k);
}

static int	ft_count(const char *str, char sp)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != sp)
		i++;
	return (i);
}

char	**ft_split(char const *str, char c)
{
	char	**split;
	int		i;
	int		j;
	int		k;

	i = -1;
	k = 0;
	split = malloc(sizeof(char *) * (ft_countw(str, c) + 1));
	if (!split)
		return (0);
	while ((size_t)++i <= ft_strlen(str))
	{
		j = 0;
		if (ft_count(str + i, c))
		{
			split[k] = malloc(ft_count(str + i, c) + 1);
			if (!split[k])
				return (0);
			while (str[i] && c != str[i])
				split[k][j++] = str[i++];
			split[k++][j] = '\0';
		}
	}
	split[k] = 0;
	return (split);
}
