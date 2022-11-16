/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 15:55:28 by iamongeo          #+#    #+#             */
/*   Updated: 2022/10/16 22:49:09 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*jstr;
	char	*j;

	if (!s1 || !s2)
		return (NULL);
	jstr = NULL;
	if (!ft_malloc_p(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1),
			(void **)&jstr))
		return (NULL);
	j = jstr;
	while (*s1)
		*(j++) = *(s1++);
	while (*s2)
		*(j++) = *(s2++);
	*j = '\0';
	return (jstr);
}
