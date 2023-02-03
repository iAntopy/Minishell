/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 20:41:48 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/02 16:31:00 by tbeaudoi         ###   ########.fr       */
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
