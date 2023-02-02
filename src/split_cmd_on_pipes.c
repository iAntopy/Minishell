/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd_on_pipes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 23:20:16 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/02 00:22:12 by iamongeo         ###   ########.fr       */
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

	if (*s == '\0')
		return (0);
	wcount = ft_strncmp(s, " | ", 3) != 0;
	while (*s)
	{
		if (is_quote(*s, &quote_switch))
		{
			while (*(++s + 1) && *(s + 1) != is_quote(*(s + 1), &quote_switch))
				continue ;
		}
		else
			wcount += (ft_strncmp(s, " | ", 3) == 0) && *(s) && *(s + 1) && *(s + 2) && *(s + 3);
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
	s = str;
	while (*s)
	{
		if (is_quote(*s, &quote_switch))
		{
			while (*(++s + 1) && *(s + 1) != is_quote(*(s + 1), &quote_switch))
				continue ;
			printf("");
		}
		if (ft_strncmp(s, " | ", 3) == 0)
		{
			if (s != str)
				tab[wcount++] = ft_strndup(str, s - str);
			str = s + 3;
			s = str - 1;
		}
		else if (!(*s) || !(*(s + 1)))
			tab[wcount++] = ft_strndup(str, SIZE_MAX);
		s++;
	}
	return (wcount);
}

int	split_on_pipes(t_job *job)//char *s, char ***ret)
{
	int		wcount;
	char	*p;

	if (!job)
		return (-1);
	job->pipe_split = NULL;
	p = job->parsed;
	wcount = node_counter(p);
	if (!ft_calloc_p(sizeof(char *) * (wcount + 1), (void **)&job->pipe_split))
		return (-1);
	if (wcount && pipe_splitter(job->pipe_split, p) <= 0)
	{
		strtab_clear(&job->pipe_split);
		return (report_jm_mlc_err(__FUNCTION__));
	}
	return (0);
}
