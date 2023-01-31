/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_redirector.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 20:54:13 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/31 00:58:39 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	apply_redirections_for_single_cmd(t_cmd *cmd)
{
	int		i;
	char	**tks;
	int		status;
	
	printf("\n\nAllying redirections :\n");
	tks = cmd->tokens;
	i = -1;
	status = 0;
	while (tks[++i] && status != -1)
	{
		status = 0;
		printf("\nApplying redirections to token : %s\n", tks[i]);
		if (ft_strncmp(tks[i], "<<", 2) == 0)	// run heredoc file input
			status = get_heredoc_input(cmd, tks + i, &cmd->job->heredoc_id);//&cmd->redir_in, &cmd->job->heredoc_id);
		else if (ft_strncmp(tks[i], ">>", 2) == 0)	// redirect output in append mode
		{
			printf("Redirection output in APPEND MODE \n");
			status = redirect_outfile(cmd, tks + i, O_APPEND);//&cmd->redir_out, O_APPEND);
		}
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
	char	*cmdname;
	char	*cmd_path;
	int		builtin_status;
	
	printf("\nValidating executable cmd :\n");
	intercept_builtin_call(cmd, &builtin_status);
	cmdname = cmd->tokens[0];
	if (builtin_status == BUILTIN_FOUND)
	{
		printf("BUILTIN INTERCEPTED\n");
		return (0);
	}
	if (ft_strncmp(cmdname, "./", 2) == 0 && access(cmdname, F_OK | X_OK) == 0)
		return (0);
	else if (find_file_in_paths(cmdname, cmd->job->msh->paths, &cmd_path,
		F_OK | X_OK) < 0
		|| !cmd_path)
	{
		ft_free_p((void **)cmd_path);
		cmd->job->msh->exit_status = 127;
		cmd->doa = 1;
		return (report_cmd_not_found(cmdname));
	}
	ft_free_p((void **)cmd->tokens);
	cmd->tokens[0] = cmd_path;
	return (0);
}

int	setup_all_cmds(t_job *job)
{
	int		i;
	t_cmd	*cmd;

	printf("Setting up cmds. %d cmds to setup\n", job->nb_cmds);
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
		apply_redirections_for_single_cmd(cmd);
		restore_substrings_in_tab(cmd->tokens, cmd->job->sc, 1);
		if (cmd->tokens[0])
		{
			printf("Validating cmd file of cmd %d\n", i);
			find_and_validate_cmd_file(cmd);
		}
	}
	print_all_cmds(job);
	strtab_clear(&job->pipe_split);
	return (0);
}
