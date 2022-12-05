/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr_space.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 05:43:46 by iamongeo          #+#    #+#             */
/*   Updated: 2022/12/05 05:48:11 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr_space(const char *s)
{
	if (!s)
		return (NULL);
	while (*s && !ft_isspace(*s))
		s++;
	if (*s)
		return ((char *)s);
	return (NULL);
}
