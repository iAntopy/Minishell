/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_length_array_min_max.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 04:35:35 by iamongeo          #+#    #+#             */
/*   Updated: 2022/10/12 18:34:34 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	*varr_min_p(t_varr *va)
{
	size_t	i;
	int		best_value;
	int		*best;

	if (!va)
		return (NULL);
	best_value = INT_MAX;
	best = &best_value;
	i = -1;
	while (++i < va->len)
		if (va->arr[i] < *best)
			best = va->arr + i;
	return (best);
}

int	*varr_max_p(t_varr *va)
{
	size_t	i;
	int		best_value;
	int		*best;

	if (!va)
		return (NULL);
	best_value = INT_MIN;
	best = &best_value;
	i = -1;
	while (++i < va->len)
		if (va->arr[i] > *best)
			best = va->arr + i;
	return (best);
}

int	varr_min(t_varr *va)
{
	int	*val_p;

	val_p = varr_min_p(va);
	if (val_p)
		return (*val_p);
	return (0);
}

int	varr_max(t_varr *va)
{
	int	*val_p;

	val_p = varr_max_p(va);
	if (val_p)
		return (*val_p);
	return (0);
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
