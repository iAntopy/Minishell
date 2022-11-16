/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_length_array_ops.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:28:50 by iamongeo          #+#    #+#             */
/*   Updated: 2022/10/13 17:00:25 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	varr_get(t_varr *va, size_t i, int *ret)
{
	if (!va || (i >= va->len))
		return (-1);
	if (ret)
		*ret = va->arr[i];
	else
		return (va->arr[i]);
	return (0);
}

int	varr_set(t_varr *va, size_t i, int value)
{
	if (!va || (i >= va->len))
		return (-1);
	va->arr[i] = value;
	return (0);
}

int	varr_pop(t_varr *va, int *ret)
{
	if (!va || va->len == 0)
		return (-1);
	if (ret)
		*ret = va->arr[va->len - 1];
	varr_remove_idx(va, va->len - 1);
	return (0);
}

int	varr_pop_front(t_varr *va, int *ret)
{
	if (!va || va->len == 0)
		return (-1);
	if (ret)
		*ret = va->arr[0];
	varr_remove_idx(va, 0);
	return (0);
}

int	varr_sum(t_varr *va)
{
	size_t	i;
	int		acc;

	if (!va)
		return (0);
	acc = 0;
	i = -1;
	while (++i < va->len)
		acc += va->arr[i];
	return (acc);
}
/*
int	main()
{
	t_varr	*va;
	int		i;
	int		value;

	va = varr_create(10);
	i = -1;
	while (++i < 10)
		varr_append(va, i + 42);
	varr_print(va);
	varr_pop(va, &value);
	ft_printf("value after pop : %d\n", value);
	ft_printf("va len : %d\n", va->len);
	varr_print(va);
	varr_pop_front(va, &value);
	ft_printf("value after pop front : %d\n", value);
	ft_printf("va len : %d\n", va->len);
	varr_print(va);

	varr_get(va, 4, &value);
	ft_printf("value after get index 4 : %d\n", value);
	ft_printf("va len : %d\n", va->len);
	varr_print(va);

	varr_set(va, 4, 99);
	ft_printf("value after set index 4 to 99 : %d\n", value);
	varr_print(va);

	ft_printf("va is empty ? %d. (expected = 0)\n", varr_isempty(va));
	while (va->len != 0)
	{
		ft_printf("va len : %d\n", va->len);
		varr_pop_front(va, NULL);
	}

	ft_printf("va is empty ? %d. (expected = 1)\n", varr_isempty(va));
	return (0);
}
*/
