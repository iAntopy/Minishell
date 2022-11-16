/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_length_array_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 22:45:39 by iamongeo          #+#    #+#             */
/*   Updated: 2022/10/16 19:53:25 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*varr_clear(t_varr **va_p)
{
	t_varr	*va;

	if (!va_p)
		return (NULL);
	va = *va_p;
	if (!va)
		return (NULL);
	if (va->arr)
		ft_free_p((void **)&va->arr);
	ft_free_p((void **)va_p);
	return (NULL);
}

t_varr	*varr_copy(t_varr *src, t_varr **ret)
{
	t_varr	*va;

	va = varr_create(src->len);
	if (!va)
		return (NULL);
	va->len = src->len;
	va->__alloced_chks = src->__alloced_chks;
	va->__max_len = src->__max_len;
	va->__cur_size = src->__cur_size;
	ft_memcpy(va->arr, src->arr, src->len * sizeof(int));
	if (ret)
		*ret = va;
	return (va);
}

void	varr_print(t_varr *va)
{
	size_t	i;

	if (!va)
		return ;
	if (!va->arr)
		fperror("ERROR : varr_print : t_varr *va given but its array is NULL.");
	if (!va->len)
		ft_printf("[ ]\n");
	else if (va->len == 1)
		printf("[ %d ]\n", va->arr[0]);
	else
	{
		ft_printf("[ ");
		i = -1;
		while (++i < (va->len - 1))
			ft_printf("%d, ", va->arr[i]);
		ft_printf("%d ] \n", va->arr[i]);
	}
}

void	varr_eprint(t_varr *va)
{
	size_t	i;

	if (!va)
		return ;
	if (!va->arr)
		fperror("ERROR : varr_print : t_varr *va given but its array is NULL.");
	if (!va->len)
		ft_eprintf("[ ]\n");
	else if (va->len == 1)
		ft_eprintf("[ %d ]\n", va->arr[0]);
	else
	{
		ft_eprintf("[ ");
		i = -1;
		while (++i < (va->len - 1))
			ft_eprintf("%d, ", va->arr[i]);
		ft_eprintf("%d ] \n", va->arr[i]);
	}
}
