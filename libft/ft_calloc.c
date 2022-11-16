/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 05:57:42 by iamongeo          #+#    #+#             */
/*   Updated: 2022/10/21 23:38:10 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t n, size_t s)
{
	void	*ptr;
	size_t	mem_size;

	if (!n || !s)
		return (ft_calloc(1, 1));
	if (s > (SIZE_MAX / n))
		return (NULL);
	mem_size = n * s;
	ptr = NULL;
	if (!ft_malloc_p(mem_size, (void **)&ptr))
		return (NULL);
	ft_memclear(ptr, mem_size);
	return (ptr);
}

int	ft_calloc_p(size_t size, void **ret)
{
	void	*ptr;

	if (!ret || !size)
		return (0);
	ptr = NULL;
	if (!ft_malloc_p(size, (void **)&ptr))
		return (0);
	ft_memclear(ptr, size);
	*ret = ptr;
	return (1);
}
/*
int	main()
{
	char	*str;
	char	*str2;

	str = ft_calloc(30, 1);
	str2 = calloc(30, 1);

	write(2, str, 30);
	write(1, str2, 30);
	return (0);
}
*/
