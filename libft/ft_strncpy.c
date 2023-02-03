/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 20:41:48 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/01 20:52:05 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncpy(char *dst, const char *src, size_t n)
{
	char	*d;
	if (!dst || !src)
		return (NULL);
	d = dst;
	while (*src && n--)
		*(d++) = *(src++);
	*d = '\0';
	return (dst);
}
