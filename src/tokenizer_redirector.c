/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_redirector.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 20:54:13 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/20 23:23:14 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	tokenize_all_cmds(t_job *job)
{
	int		i;
	char	*cmd;
	char	**tokens;

	if (!job->pipe_split)
		return (-1);
	i = -1;
	while (++i < job->nb_cmds)
	{
		cmd = job->pipe_split[i];
		job->cmds = ft_split_space(cmd);
		if (!job->cmds)
			return (-1);
		restore_substrings_in_tab(job->cmds[i], job->sc);
	}
	strtab_clear(&job->pipe_split);
	return (0);
}

static int	apply_redirections_for_single_cmd(t_job *job, int idx)
{
	int		i;
	char	**tks;
	char	*tk;
	int		status;

	tks = job->cmd[idx];
	i = -1;
	status = 0;
	while (tokens[++i] && status != -1)
	{
		status = 0;
		tk = tokens[i];
		if (ft_strncmp(tk, "<<", 2) == 0)	// run heredoc file input
			status = get_here_doc_input(tks + i, &job->redir_in[idx], &job->heredoc_id);
		else if (ft_strncmp(tk, ">>", 2) == 0)	// redirect output in append mode
			status = redirect_outfile(tks + i, &job->redir_out[idx], O_APPEND);
		else if (ft_strncmp(tk, "<", 1) == 0)	// redirect input file
			status = redirect_infile(tks + i, &job->redir_in[idx]);
		else if (ft_strncmp(tk, ">", 1) == 0)	// redirect output in truncate mode
			status = redirect_outfile(tks + i, &job->redir_out[idx], O_TRUNC);
		if (status == 1)
			i--;
	}
	return (0);
}

int	apply_all_redirections(t_job *job)
{
	int		i;

	i = -1;
	while (i < job->nb_cmds)
		apply_redirections_for_single_cmd(job, i);
	return (0);
}
