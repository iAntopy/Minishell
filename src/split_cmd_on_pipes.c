/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd_on_pipes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 23:20:16 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/25 19:48:38 by tbeaudoi         ###   ########.fr       */
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
		{
			while (*(++s) && *s != is_quote(*s, &quote_switch))
				continue ;
		}
		else
			wcount += (ft_strncmp(s, " | ", 3) == 0) && *(s + 3);
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
	int		wcount;

	if (!s)
		return (-1);
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
