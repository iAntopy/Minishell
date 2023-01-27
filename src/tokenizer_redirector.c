/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_redirector.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 20:54:13 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/27 09:25:48 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// DELETE ME
static void	print_all_cmds(t_job *job)
{
	int	i;

	i = -1;
	while (++i < job->nb_cmds)
	{
		ft_eprintf("\nCMD %d : \n", i + 1);
		strtab_print(job->cmds[i].tokens);
	}
}

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
		if (ft_strncmp(tks[i], "<<", 2) == 0)	// run heredoc file input
			status = get_heredoc_input(cmd, tks + i, &cmd->job->heredoc_id);//&cmd->redir_in, &cmd->job->heredoc_id);
		else if (ft_strncmp(tks[i], ">>", 2) == 0)	// redirect output in append mode
			status = redirect_outfile(cmd, tks + i, O_APPEND);//&cmd->redir_out, O_APPEND);
		else if (ft_strncmp(tks[i], "<", 1) == 0)	// redirect input file
			status = redirect_infile(cmd, tks + i);//&cmd->redir_in);
		else if (ft_strncmp(tks[i], ">", 1) == 0)	// redirect output in truncate mode
			status = redirect_outfile(cmd, tks + i, O_TRUNC);//&cmd->redir_out, O_TRUNC);
		if (status == 1)
			i--;
	}
	
	return (0);
}

int	find_and_validate_cmd_file(t_cmd *cmd)
{
	char	*cmd_path;
	int		builtin_status;
	

	intercept_builtin_call(cmd, &builtin_status);
	if (builtin_status == BUILTIN_FOUND)
	{
		printf("BUILTIN INTERCEPTED\n");
		return (0);
	}
	if (find_file_in_paths(cmd->tokens[0], cmd->job->msh->paths, &cmd_path,
		F_OK | X_OK) < 0
		|| !cmd_path)
	{
		ft_free_p((void **)cmd_path);
		cmd->job->msh->exit_status = 127;
		cmd->doa = 1;
		return (report_cmd_not_found(cmd->tokens[0]));
	}
	ft_free_p((void **)cmd->tokens);
	cmd->tokens[0] = cmd_path;
	return (0);
}

int	setup_all_cmds(t_job *job)
{
	int		i;
	t_cmd	*cmd;

	if (!job->pipe_split)
		return (-1);
	i = -1;
	while (++i < job->nb_cmds)
	{
		cmd = job->cmds + i;
		cmd->job = job;
		cmd->tokens = ft_split_space(job->pipe_split[i]);
		if (!cmd->tokens)
			return (-1);
		restore_substrings_in_tab(cmd->tokens, cmd->job->sc);
		find_and_validate_cmd_file(cmd);
		apply_redirections_for_single_cmd(cmd);
	}
	print_all_cmds(job);
	strtab_clear(&job->pipe_split);
	return (0);
}
