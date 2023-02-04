/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 15:53:40 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/03 22:26:20 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] && s2[i] && (i < n))
		i++;
	return (((unsigned char)s1[i] - (unsigned char)s2[i]) * (i < n));
}
