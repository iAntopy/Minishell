/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 15:55:00 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/07 21:20:16 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	word_counter(const char *s, char c)
{
	int		wcount;

	if (*s == '\0')
		return (0);
	wcount = *s != c;
	while (*(++s))
		wcount += (*(s - 1) == c) && (*s != c);
	return (wcount);
}

static int	word_splitter(char **tab, char const *s, char c)
{
	int	wcount;
	int	j;

	wcount = 0;
	while (*s)
	{
		j = 0;
		while (*s && (*s == c))
			s++;
		if (*s)
		{
			while (*(s + j) && (*(s + j) != c))
				j++;
			tab[wcount] = ft_substr(s, 0, j);
			if (!tab[wcount++])
				return (-1);
		}
		s += j;
	}
	return (wcount);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	int		wcount;

	if (!s)
		return (NULL);
	tab = NULL;
	wcount = word_counter(s, c);
	if (!ft_calloc_p(sizeof(char *) * (wcount + 1), (void **)&tab))
		return (NULL);
	if (wcount && word_splitter(tab, s, c) <= 0)
		return (strtab_clear(&tab));
	return (tab);
}
