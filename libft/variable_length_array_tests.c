/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_length_array_tests.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 21:06:24 by iamongeo          #+#    #+#             */
/*   Updated: 2022/10/09 04:40:25 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	varr_isempty(t_varr *va)
{
	if (!va)
		return (0);
	return (va->len == 0);
}

int	varr_is_empty(t_varr *va)
{
	return (varr_isempty(va));
}

int	*varr_is_in(t_varr *va, int value)
{
	size_t	i;

	if (!va || !va->arr)
		return (NULL);
	i = -1;
	while (++i < va->len)
		if (va->arr[i] == value)
			return (va->arr + i);
	return (NULL);
}
/*
int	main()
{
	t_varr	*va;
	int		*res;
	int		s;
	int		i;

	va = varr_create(10);
	i = -1;
	while (++i < va->len)
		varr_set(va, i, 100 + i);
	varr_print(va);
	res = varr_min(va);
	ft_printf("varr min : %d\n", *res);
	res = varr_max(va);
	ft_printf("varr max : %d\n", *res);
	s = varr_sum(va);
	ft_printf("varr sum : %d\n", s);

	return (0);
}
*/
