/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_abs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/02 19:12:42 by iamongeo          #+#    #+#             */
/*   Updated: 2022/10/02 19:31:13 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_abs(const int nb)
{
	int	mask;

	mask = nb >> 31;
	return ((nb ^ mask) - mask);
}
/*
int	main()
{
	int	a = 15;
	int	b = -15;

	ft_printf("a, ft_abs(a) : %d, %d\n", a, ft_abs(a));
	ft_printf("b, ft_abs(b) : %d, %d\n", b, ft_abs(b));
	return (0);
}
*/
