/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_on_bools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 01:59:49 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/07 22:20:28 by iamongeo         ###   ########.fr       */
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

static int	add_pln_swap_line(t_msh *msh, char **line, char **r_p, int bl)
{
	char	*new_line;
	char	*r;
	int		mlen;

	if (msh->nb_plns >= MAX_PIPELINES)
		return (report_max_nb_pipelines_exceeded(msh));
	r = *r_p;
	msh->pipelines[msh->nb_plns] = ft_strndup(*line, r - *line);
	if (!msh->pipelines[msh->nb_plns])
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
	msh->rawline = NULL;
	return (0);
}
