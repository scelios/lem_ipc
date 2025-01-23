/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beaudibe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 15:01:57 by beaudibe          #+#    #+#             */
/*   Updated: 2022/11/07 12:15:41 by beaudibe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	int		i;
	t_list	*cur;

	cur = lst;
	i = ft_lstsize(lst);
	if (!i)
		return (lst);
	while (--i)
		cur = cur->next;
	return (cur);
}
