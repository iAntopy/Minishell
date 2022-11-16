/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_buff.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 00:03:32 by iamongeo          #+#    #+#             */
/*   Updated: 2022/11/03 00:04:58 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*rec_putnbr(char *buff, size_t n)
{
	if (n > 9)
		buff = rec_putnbr(buff, n / 10);
	*buff = '0' + (n % 10);
	return (buff + 1);
}

int	ft_putnbr_buff(char *buff, ssize_t n)
{
	char	*b;

	b = buff;
	*b = '-';
	b = rec_putnbr(b + (n < 0), n * (1 - (2 * (n < 0))));
	*(b++) = ' ';
	*b = '\0';
	return (b - buff);
}
