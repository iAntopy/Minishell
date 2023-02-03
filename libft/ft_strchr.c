/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 15:54:45 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/02 22:42:19 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	cc;

	if (!s)
		return (NULL);
	cc = (unsigned char)c;
	while (*s && (*s != cc))
		s++;
	if (*s == cc)
		return ((char *)s);
	return (NULL);
}

char	*ft_strchr_p(const char *s, int c, char **ret)
{
	*ret = ft_strchr(s, c);
	if (*ret)
		return (*ret);
	return (NULL);
}
