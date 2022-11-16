/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 15:15:10 by iamongeo          #+#    #+#             */
/*   Updated: 2022/10/27 16:52:09 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	get_substr(char *str, size_t start, size_t n, char **ret)
{
	char	*r;
	char	*s;
	int		to_end;

	to_end = (n == SIZE_MAX);
	if (!str)
		return (0);
	r = str + start;
	if (to_end)
		while ((++n < SIZE_MAX) && *r)
			r++;
	if (n)
	{
		*ret = NULL;
		if (!ft_malloc_p(sizeof(char) * (n + 1), (void **)ret))
			return (0);
		r = *ret;
		s = str + start;
		while (n--)
			*(r++) = *(s++);
		*r = '\0';
	}
	if (to_end)
		ft_free_p((void **)&str);
	return (1);
}

// psh_app determines weither the new element is pushed behind dlst
// (psh_app >= 1) or appended after dlst (SIZE_MAX). (0) only creates 
// new initialized element at *elem pointer. When text chunks are
// inserted in the list the psh_app value will represent the chunk len.
int	gdl_insert(t_gdl **dlst, t_gdl **elem, char *str, size_t psh_app)
{
	if (!ft_malloc_p(sizeof(t_gdl), (void **)elem))
		return (0);
	(*elem)->prev = NULL;
	(*elem)->next = NULL;
	(*elem)->str = str;
	(*elem)->n = psh_app;
	if (dlst && *dlst && psh_app == SIZE_MAX)
	{
		if ((*dlst)->next)
			(*dlst)->next->prev = *elem;
		(*elem)->next = (*dlst)->next;
		(*dlst)->next = *elem;
		(*elem)->prev = (*dlst);
	}
	else if (dlst && *dlst && psh_app >= 1)
	{
		if ((*dlst)->prev)
			(*dlst)->prev->next = *elem;
		(*elem)->prev = (*dlst)->prev;
		(*dlst)->prev = *elem;
		(*elem)->next = *dlst;
	}
	return (1);
}

int	join_clear_list(char *line, t_gdl **elem)
{
	char	*s;

	if (!(*elem))
		return (1);
	while ((*elem)->next)
		*elem = (*elem)->next;
	while (1)
	{
		s = (*elem)->str;
		if (line && s)
			while (*s)
				*(line++) = *(s++);
		ft_free_p((void **)&((*elem)->str));
		if (!((*elem)->prev))
			break ;
		*elem = (*elem)->prev;
		ft_free_p((void **)&((*elem)->next));
	}
	ft_free_p((void **)elem);
	return (1);
}

char	*gather_line(t_gdl **chks)
{
	size_t	total_len;
	t_gdl	*elem;
	char	*line;

	elem = *chks;
	if (!elem)
		return (NULL);
	if (!(elem->next))
	{
		line = elem->str;
		ft_free_p((void **)chks);
		return (line);
	}
	total_len = elem->n;
	while (elem->next)
	{
		elem = elem->next;
		total_len += elem->n;
	}
	if (!ft_malloc_p(sizeof(char) * (total_len + 1), (void **)(&line))
		|| !join_clear_list(line, &elem))
		return ((char *)E_MLC);
	line[total_len] = '\0';
	return (line);
}
