/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sqrt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 18:59:29 by iamongeo          #+#    #+#             */
/*   Updated: 2022/10/04 19:14:00 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#define MAX_SQRT_ITERS 20

//Newton-Raphson methode with max 20 iterations.
static int	_sqrt(const int nb)
{
	int	i;
	int	guess;
	int	best_guess;
	int	delta_guess;
	int	delta_best_guess;

	i = 0;
	guess = 1;
	best_guess = 1;
	delta_best_guess = ft_abs(nb - guess);
	while (i < MAX_SQRT_ITERS)
	{
		guess = (guess + (nb / guess)) / 2;
		if (guess * guess == nb)
			return (guess);
		delta_guess = ft_abs((nb - (guess * guess)));
		if (delta_guess < delta_best_guess)
		{
			best_guess = guess;
			delta_best_guess = delta_guess;
		}
		i++;
	}
	return (best_guess);
}

int	ft_sqrt(const int nb)
{
	if (nb < 0)
		return (0);
	return (_sqrt(nb));
}
/*
int	main()
{
	int	res;
	int	nb;

	nb = -1;
	res = ft_sqrt(nb);
	ft_printf("ft_sqrt(%d) = %d\n", nb, ft_sqrt(nb));

	nb = -67162351;
	res = ft_sqrt(nb);
	ft_printf("ft_sqrt(%d) = %d\n", nb, ft_sqrt(nb));

	nb = 1;
	res = ft_sqrt(nb);
	ft_printf("ft_sqrt(%d) = %d\n", nb, ft_sqrt(nb));

	nb = 3;
	res = ft_sqrt(nb);
	ft_printf("ft_sqrt(%d) = %d\n", nb, ft_sqrt(nb));

	nb = 7;
	res = ft_sqrt(nb);
	ft_printf("ft_sqrt(%d) = %d\n", nb, ft_sqrt(nb));

	nb = 25;
	res = ft_sqrt(nb);
	ft_printf("ft_sqrt(%d) = %d\n", nb, ft_sqrt(nb));

	nb = 24;
	res = ft_sqrt(nb);
	ft_printf("ft_sqrt(%d) = %d\n", nb, ft_sqrt(nb));

	nb = 140;
	res = ft_sqrt(nb);
	ft_printf("ft_sqrt(%d) = %d\n", nb, ft_sqrt(nb));

	nb = 144;
	res = ft_sqrt(nb);
	ft_printf("ft_sqrt(%d) = %d\n", nb, ft_sqrt(nb));

	nb = 150;
	res = ft_sqrt(nb);
	ft_printf("ft_sqrt(%d) = %d\n", nb, ft_sqrt(nb));

	nb = 170;
	res = ft_sqrt(nb);
	ft_printf("ft_sqrt(%d) = %d\n", nb, ft_sqrt(nb));

	nb = 2000000000;
	res = ft_sqrt(nb);
	ft_printf("ft_sqrt(%d) = %d\n", nb, ft_sqrt(nb));

	return (0);
}
*/
