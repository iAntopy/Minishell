/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 15:48:33 by iamongeo          #+#    #+#             */
/*   Updated: 2022/10/16 22:35:56 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*elem;

	elem = NULL;
	if (!ft_malloc_p(sizeof(t_list), (void **)&elem))
		return (NULL);
	elem->content = content;
	elem->next = NULL;
	return (elem);
}
