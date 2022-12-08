/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd_on_pipes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 23:20:16 by iamongeo          #+#    #+#             */
/*   Updated: 2022/12/07 18:52:28 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/// Splits on pipes respecting quote blocks

static char	is_quote(char c, char *quote_switch)
{
	*quote_switch = '\0';
	if (c == '\"' || c == '\'')
		*quote_switch = c;
	return (*quote_switch);
}

static int	node_counter(char *s)
{
	int		wcount;
	char	quote_switch;

	if (*s == '\0')
		return (0);
	wcount = ft_strncmp(s, " | ", 3) != 0;
	while (*s)
	{
		if (is_quote(*s, &quote_switch))
			while (*(++s) && *s != is_quote(*s, &quote_switch))
				continue ;
		else
			wcount += (ft_strncmp(s, " | ", 3) == 0) && *(s + 3);
			//wcount += (*s == c) && *(s + 1) && (*(s + 1) != c);
		s++;
	}
	return (wcount);
}

static int	pipe_splitter(char **tab, char *str)
{
	int		wcount;
	char	quote_switch;
	char	*s;

	if (!str)
		return (-1);
	wcount = 0;
	s = (char *)str;
	while (*s)
	{
		if (is_quote(*s, &quote_switch))
			while (*(++s) && *s != is_quote(*s, &quote_switch))
				continue ;
		if (ft_strncmp(s, " | ", 3) == 0)
		{
			if (s != str)
				tab[wcount++] = ft_strndup(str, s - str);
			str = s + 3;
			s = str - 1;
		}
		else if (!(*(s + 1)))
			tab[wcount++] = ft_strndup(str, SIZE_MAX);
		s++;
	}
	return (wcount);
}

int	split_cmd_on_pipes(char *s, char ***ret)
{
//	char	**tab;
	int		wcount;

	if (!s)
		return (-1);
//	tab = NULL;
	*ret = NULL;
	wcount = node_counter(s);
	printf("split cmd : wcount %d\n", wcount);
	if (!ft_calloc_p(sizeof(char *) * (wcount + 1), (void **)ret))
		return (-1);
	if (wcount && pipe_splitter(*ret, s) <= 0)
	{
		strtab_clear(ret);
		return (-1);
	}
	printf("strtab after pipe split : \n");
	strtab_print(*ret);
	return (0);
}
/*
int	main(int argc, char **argv)
{
	char	*str = "";
	char	del = 'a';
	char	**tab;

	if (argc < 2)
		return (1);
	tab = split_cmd_on_pipes(argv[1]);
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
