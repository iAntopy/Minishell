/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_on_bools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 01:59:49 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/09 03:50:29 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skip_open_quotes(char **line)
{
	if (!**line)
		return (0);
	if (**line == '\'' || **line == '\"')
		*line = ft_strchr(*line + 1, **line);
	return (1);
}

static int	validate_parentheses(t_msh *msh, char *line)
{
	char	*close;
	char	*c;

	printf("validating pars : line : %s\n", line);
	if (!ft_strchr_set(line, "()"))
		return (0);
	while (*line && ft_isspace(*line))
		line++;
	close = ft_strrchr(line, ')');
	if (!close)
		return (report_syntax_error(line, 1, &msh->exit_status, 0));
	c = close + 1;
	while (*c && ft_isspace(*c))
		c++;
	if (*c)
		return (report_syntax_error(close, 1, &msh->exit_status, 0));
	if (*line != '(')
		return (report_syntax_error(close, 1, &msh->exit_status, 0));
	if ((line + 1) == close)
		return (report_syntax_error(close, 1, &msh->exit_status, 0));
	return (0);
}

static int	skip_dup_open_pars(t_msh *msh, char **line, char **r_p, char **ret)
{
	char	*close;
	char	*new_line;

	while (*line && ft_isspace(*line))
		line++;
	if (*line == '(' && !ft_strchr_p(line + 1, ')', &close))
		return (report_syntax_error(line, 1, &msh->exit_status, 0));
	*ret = ft_strndup(line, close - line);
	ft_free_p((void **)line);
	*r_p = close;
	return (0);
}

static int	add_pln_swap_line(t_msh *msh, char **line, char **r_p, int bl)
{
	char	*new_line;
	char	*r;
	int		mlen;
	char	*trim;

	if (msh->nb_plns >= MAX_PIPELINES)
		return (report_max_nb_pipelines_exceeded(msh));
	r = *r_p;
	trim = ft_strndup(*line, r - *line);
	if (validate_parentheses(msh, trim) < 0)
		return (-1);
	msh->pipelines[msh->nb_plns] = ft_strtrim(trim, " ()");
	if (ft_free_p((void **)&trim) && !msh->pipelines[msh->nb_plns])
		return (-1);
	msh->pl_meta_bools[msh->nb_plns++] = bl;
	is_meta_char(r, &mlen);
	new_line = ft_strndup(r + mlen, ft_strlen(r + mlen));
	if (!new_line)
		return (-1);
	ft_free_p((void **)line);
	*line = new_line;
	*r_p = new_line;
	return (0);
}

int	split_on_bools(t_msh *msh)
{
	char	*r;
	int		mlen;

	if (!msh->rawline)
		return (-1);
	r = msh->rawline;
	while (*r && skip_open_quotes(&r))
	{
		if (is_meta_char(r, &mlen))
		{
			if (ft_strncmp(r, "&&", 2) == 0
				&& add_pln_swap_line(msh, &msh->rawline, &r, BOOL_AND) == -1)
				return (-1);
			else if (ft_strncmp(r, "||", 2) == 0
				&& add_pln_swap_line(msh, &msh->rawline, &r, BOOL_OR) == -1)
				return (-1);
			else if (ft_strncmp(r, ";", 1) == 0
				&& add_pln_swap_line(msh, &msh->rawline, &r, SEMI_COL) == -1)
				return (-1);
		}
		r++;
	}
	msh->pipelines[msh->nb_plns++] = msh->rawline;
	if (validate_parentheses(msh, msh->pipelines[msh->nb_plns]) < 0)
		return (-1);
	msh->rawline = NULL;
	return (0);
}
