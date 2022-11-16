/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_length_array.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 18:49:37 by iamongeo          #+#    #+#             */
/*   Updated: 2022/10/25 20:39:55 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Variable length array that somewhat acts like a linked list. Can be indexed,
// appended to other varrs, split, copied. int type only.

// declare t_varr struct in some previous function and give ptr to this func.
t_varr	*varr_create(size_t n)
{
	t_varr	*va;
	int		div;
	int		mod;
	size_t	size;

	div = n / VARR_CHUNK_LEN;
	mod = n % VARR_CHUNK_LEN;
	div += !!mod + !n;
	size = sizeof(int) * VARR_CHUNK_LEN * div;
	va = NULL;
	if (!ft_calloc_p(sizeof(t_varr), (void **)&va)
		|| !ft_malloc_p(size, (void **)&va->arr))
		return (NULL);
	va->len = 0;
	va->__alloced_chks = div;
	va->__max_len = div * VARR_CHUNK_LEN;
	va->__cur_size = size;
	return (va);
}

t_varr	*varr_append(t_varr *va, int value)
{
	int		*new_arr;
	size_t	new_size;
	size_t	old_size;

	if ((va->len + 1) > va->__max_len)
	{
		old_size = va->__cur_size;
		va->__alloced_chks++;
		va->__max_len += VARR_CHUNK_LEN;
		new_size = va->__max_len * sizeof(int);
		new_arr = NULL;
		if (!ft_malloc_p(new_size, (void **)&new_arr))
			return (NULL);
		ft_memcpy(new_arr, va->arr, old_size);
		ft_free_p((void **)&va->arr);
		va->arr = new_arr;
		va->__cur_size = new_size;
	}
	va->arr[va->len++] = value;
	return (va);
}

t_varr	*varr_remove_idx(t_varr *va, size_t i)
{
	int		*new_arr;
	size_t	new_size;
	size_t	old_size;

	if (i >= va->len)
		return (NULL);
	else if ((va->len - 1) < (va->__max_len / 4) && (va->len >= VARR_CHUNK_LEN))
	{
		old_size = va->len * sizeof(int);
		va->__alloced_chks /= 2;
		va->__max_len = va->__alloced_chks * VARR_CHUNK_LEN;
		new_size = va->__max_len * sizeof(int);
		new_arr = NULL;
		if (!ft_malloc_p(new_size, (void **)&new_arr))
			return (NULL);
		ft_memcpy(new_arr, va->arr, old_size);
		ft_free_p((void **)&va->arr);
		va->arr = new_arr;
		va->__cur_size = new_size;
	}
	va->len--;
	ft_memmove(va->arr + i, va->arr + i + 1, (va->len - i) * sizeof(int));
	return (va);
}

t_varr	*varr_remove(t_varr *va, int value)
{
	int	*vp;

	if (!va)
		return (NULL);
	vp = varr_is_in(va, value);
	if (!vp)
		return (va);
	varr_remove_idx(va, vp - va->arr);
	return (va);
}

// concatenates inplace in dst.
t_varr	*varr_concatenate(t_varr *dst, t_varr *va)
{
	int	*arr;

	if (!dst || !va)
		return (NULL);
	arr = NULL;
	if (!ft_malloc_p(dst->__cur_size + va->__cur_size, (void **)&arr))
		return (NULL);
	dst->__cur_size += va->__cur_size;
	dst->__alloced_chks += va->__alloced_chks;
	dst->__max_len += va->__max_len;
	ft_memcpy(arr, dst->arr, dst->len * sizeof(int));
	ft_memcpy(arr + dst->len, va->arr, va->len * sizeof(int));
	dst->len += va->len;
	ft_free_p((void **)&dst->arr);
	dst->arr = arr;
	return (dst);
}
/*
int	main()
{
	t_varr	*va;
	t_varr	*va2;
	t_varr	*va3;
	int		n;
	int		i;
	int		*value_p;

	n = 8;
	va = varr_create(n);
	va2 = varr_create(n);
	varr_print(va);
	varr_print(va2);
	i = -1;
	while (++i < n)
	{
		varr_append(va, 42 + i);
	}
	i = -1;
	while (++i < n)
	{
		varr_append(va2, 1000 + i);
	}

	varr_print(va);
	varr_print(va2);
	printf("sortie 2e print \n");
	printf("max_len : %zu\n", va->__max_len);
	i = -1;
	printf("après i = -1\n");
	while (++i < 6)
	{
		printf("len : %zu\n", va->len);
		varr_remove_idx(va, 1);
	}
	
	ft_printf("Concatenating va2 to va variable array : \n");
	ft_printf("va : ");
	varr_print(va);
	ft_printf("va2 : ");
	varr_print(va2);

	varr_concatenate(va, va2);

	ft_printf("Result : \nva : ");
	varr_print(va);
	ft_printf("va2 : ");
	varr_print(va2);

	va3 = varr_copy(va);
	ft_printf("va3 modified post copy : ");
	varr_append(va3, INT_MAX);
	varr_print(va3);

	varr_remove(va3, 1003);
	ft_printf("va3 after remove 1003 : \n");
	varr_print(va3);
	varr_remove(va3, 1005);
	ft_printf("va3 after remove 1005 : \n");
	varr_print(va3);
	varr_remove(va3, -11234);
	ft_printf("va3 after remove -11234 : \n");
	varr_print(va3);

	value_p = varr_is_in(va3, 1004);
	ft_printf("Checking if 1004 in va3 : \n");
	if (value_p)
		ft_printf("1004 IN va3 ! %d at ptr %p\n", *value_p, value_p);
	else
		ft_printf("1004 NOT in va3 ! \n");
	value_p = varr_is_in(va3, 1005);
	ft_printf("Checking if 1005 in va3 : \n");
	if (value_p)
		ft_printf("1005 IN va3 ! %d at ptr %p\n", *value_p, value_p);
	else
		ft_printf("1005 NOT in va3 ! \n");


	varr_clear(&va);
	varr_clear(&va2);
	varr_clear(&va3);

	return (0);
}
*/
