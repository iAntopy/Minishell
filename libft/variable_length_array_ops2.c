/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_length_array_ops2.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 03:53:24 by iamongeo          #+#    #+#             */
/*   Updated: 2022/10/09 04:24:18 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_varr	*varr_iadd(t_varr *va, int value)
{
	size_t	i;

	if (!va)
		return (NULL);
	i = -1;
	while (++i < va->len)
		va->arr[i] += value;
	return (va);
}

t_varr	*varr_isub(t_varr *va, int value)
{
	size_t	i;

	if (!va)
		return (NULL);
	i = -1;
	while (++i < va->len)
		va->arr[i] -= value;
	return (va);
}

t_varr	*varr_imul(t_varr *va, int value)
{
	size_t	i;

	if (!va)
		return (NULL);
	i = -1;
	while (++i < va->len)
		va->arr[i] *= value;
	return (va);
}

t_varr	*varr_idiv(t_varr *va, int value)
{
	size_t	i;

	if (!va || !value)
		return (NULL);
	i = -1;
	while (++i < va->len)
		va->arr[i] /= value;
	return (va);
}

t_varr	*varr_imod(t_varr *va, int value)
{
	size_t	i;

	if (!va || !value)
		return (NULL);
	i = -1;
	while (++i < va->len)
		va->arr[i] %= value;
	return (va);
}
