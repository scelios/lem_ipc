/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beaudibe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 11:22:22 by beaudibe          #+#    #+#             */
/*   Updated: 2022/10/01 11:30:49 by beaudibe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	t_list	*cur;

	if (!lst || !f)
		return ;
	cur = lst;
	f(cur->content);
	while (cur->next)
	{
		cur = cur->next;
		f(cur->content);
	}
}
