/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beaudibe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 11:43:32 by beaudibe          #+#    #+#             */
/*   Updated: 2022/11/10 12:43:15 by beaudibe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*current;
	t_list	*new_lst;
	t_list	*new;

	new_lst = NULL;
	current = lst;
	if (!lst)
		return (0);
	while (current)
	{
		new = ft_lstnew((*f)(current->content));
		if (!new)
		{
			ft_lstclear(&new, del);
			return (0);
		}
		ft_lstadd_back(&new_lst, new);
		current = current->next;
	}
	return (new_lst);
}
