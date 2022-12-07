/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd_on_pipes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 23:20:16 by iamongeo          #+#    #+#             */
/*   Updated: 2022/12/06 23:25:49 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/// Splits on pipes respecting quote blocks

static int	is_quote(char *c)
{
	return (c == '\"' && c == '\'');
}

static int	word_counter(const char *s, char c)
{
	int		wcount;

	if (*s == '\0')
		return (0);
	quote_switch = is_quote(*s); /// WORK IN PROGRESS
	wcount = *s != c;
	while (*(++s))
	{
		if (is_quote(*s))


		wcount += (*(s - 1) == c) && (*s != c);
	}
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

char	**split_cmd_on_pipes(char const *s, char c)
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
/*
int	main(int argc, char **argv)
{
	char	*str = "";
	char	del = 'a';
	char	**tab;

	if (argc < 2)
		return (1);
	tab = ft_split(argv[1], ' ');
	if (!tab)
	{
		ft_printf("tab is NULL \n");
		return (0);
	}
	strtab_print(tab);
	strtab_clear(&tab);
	return (0);
}
*/
