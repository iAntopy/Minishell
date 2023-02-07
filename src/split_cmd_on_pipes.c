/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd_on_pipes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 23:20:16 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/07 07:15:19 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

	if (!s || *s == '\0')
		return (0);
	wcount = ft_strncmp(s, " | ", 3) != 0;
	while (*s)
	{
		printf("node_counter : s : %s\n", s);
		if (is_quote(*s, &quote_switch))
			while (*s && *(++s) && *s && *s != is_quote(*s, &quote_switch))
				continue ;
		else
			wcount += (ft_strncmp(s, " | ", 3) == 0);
		s += (*s != '\0');
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
	s = str;
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
		else if (!(*s) || !(*(s + 1)))
			tab[wcount++] = ft_strndup(str, SIZE_MAX);
		s += (*s != '\0');
	}
	return (wcount);
}

int	split_on_pipes(t_job *job)
{
	int		wcount;

	if (!job || !job->parsed)
		return (-1);
	job->pipe_split = NULL;
	wcount = node_counter(job->parsed);
	if (!ft_calloc_p(sizeof(char *) * (wcount + 1), (void **)&job->pipe_split))
		return (report_malloc_err());
	if (wcount && pipe_splitter(job->pipe_split, job->parsed) <= 0)
	{
		strtab_clear(&job->pipe_split);
		return (report_malloc_err());
	}
	return (0);
}
