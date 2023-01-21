/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 15:51:29 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/20 21:49:29 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	rec_putnbr(ssize_t n)
{
	if (n > 9)
		rec_putnbr(n / 10);
	ft_putchar('0' + (n % 10));
}

void	ft_putnbr(ssize_t n)
{
	rec_putnbr(n * (1 - (2 * write(1, "-", (n < 0)))));
}
/*
void	ft_putnbr(int nbr)
{
	long long int	n;

	n = (long long int)nbr;
	write(1, "-", (n < 0));
	n *= 1 - (2 * (n < 0));
	rec_putnbr(n);
}

int	main(int argc, char *argv[])
{
	if (argc > 1)
	{
		ft_putnbr(ft_atol(argv[1]));
		ft_printf("-123 % 10  = %d\n", -123 % 10);
	}
	return (0);
}
*/
