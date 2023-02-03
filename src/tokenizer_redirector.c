/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_redirector.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 20:54:13 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/02 20:31:33 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
/// DELETE ME
void	print_all_cmds(t_job *job)
{
	int	i;

	i = -1;
	while (++i < job->nb_cmds)
	{
		ft_eprintf("\nCMD %d : \n", i + 1);
		strtab_print(job->cmds[i].tokens);
	}
}
*/

static int	apply_redirections_for_single_cmd(t_cmd *cmd)
{
	int		i;
	char	**tks;
	int		status;

	tks = cmd->tokens;
	i = -1;
	status = 0;
	while (tks[++i] && status != -1)
	{
		status = 0;
		if (ft_strncmp(tks[i], "<<", 2) == 0)
			status = get_heredoc_input(cmd, tks + i, &cmd->job->heredoc_id);
		else if (ft_strncmp(tks[i], ">>", 2) == 0)
			status = redirect_outfile(cmd, tks + i, O_APPEND);
		else if (ft_strncmp(tks[i], "<", 1) == 0)
			status = redirect_infile(cmd, tks + i);
		else if (ft_strncmp(tks[i], ">", 1) == 0)
			status = redirect_outfile(cmd, tks + i, O_TRUNC);
		if (status == 1)
			i--;
	}
	return (status);
}

int	find_and_validate_cmd_file(t_cmd *cmd)
{
	char	*cmdname;
	char	*cmd_path;

	intercept_builtin_call(cmd);
	if (cmd->builtin)
		return (0);
	cmdname = cmd->tokens[0];
	if (ft_strncmp(cmdname, "./", 2) == 0)
	{
		if (access(cmdname, F_OK | X_OK) == 0)
			return (0);
		return (report_file_error(cmdname, cmd));
	}
	else if (find_file_in_paths(cmdname, cmd->job->msh->paths, &cmd_path,
			F_OK | X_OK) < 0 || !cmd_path)
	{
		ft_free_p((void **)cmd_path);
		return (report_cmd_not_found(cmdname, cmd, 127));
	}
	ft_free_p((void **)cmd->tokens);
	cmd->tokens[0] = cmd_path;
	return (0);
}

int	setup_cmds(t_job *job)
{
	int		i;
	t_cmd	*cmd;

	job->nb_cmds = strtab_len(job->pipe_split);
	if (job->nb_cmds > MAX_CMDS)
		return (report_max_nb_cmds_exceeded(job));
	i = -1;
	while (++i < job->nb_cmds)
	{
		cmd = job->cmds + i;
		cmd->job = job;
		cmd->tokens = ft_split_space(job->pipe_split[i]);
		if (!cmd->tokens)
			return (report_jm_mlc_err(__FUNCTION__));
		if (apply_redirections_for_single_cmd(cmd) == -1)
			return (-1);
		restore_substrings_in_tab(cmd->tokens, cmd->job->sc, 1);
		if (cmd->tokens[0] && find_and_validate_cmd_file(cmd) < 0)
			return (-1);
	}
	strtab_clear(&job->pipe_split);
	return (0);
}
