/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exec_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 02:36:44 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/18 22:16:44 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	manage_redirections(t_job *job, char **tokens)
{
	int		i;
	char	*tk;
	int		status;

	i = -1;
	while (tokens[++i])
	{
		tk = tokens[i];
		if (ft_strncmp(tk, "<<", 2) == 0)	// run heredoc file input
			status = get_here_doc_input(tokens + i);
		else if (ft_strncmp(tk, ">>", 2) == 0)	// redirect output in append mode
			status = redirect_outfile(job, tokens + i, O_APPEND);
		else if (ft_strncmp(tk, "<", 1) == 0)	// redirect input file
			status = redirect_infile(job, tokens + 1);
		else if (ft_strncmp(tk, ">", 1) == 0)	// redirect output in truncate mode
			status = redirect_outfile(job, tokens + 1, O_TRUNC);

	}
}

int	parse_exec_cmd(t_job *job, int idx)
{
	char	*cmd;
	char	**tokens;
	char	*cmd_file_path;

//	usleep(10000);
	cmd = job->pipe_split[idx];
	if (!cmd)
		return (-1);
//	ft_eprintf("CHILD %d : executing cmd : %s\n", getpid(), cmd);
	tokens = NULL;
	cmd_file_path = NULL;
	tokens = ft_split(cmd, ' ');
	if (!tokens)
		return (-1);
	restore_substrings_in_tab(tokens, job->sc);
	strtab_print(tokens);
	if (find_file_in_paths(tokens[0], job->msh->paths, &cmd_file_path,
			F_OK | X_OK) < 0
		|| !cmd_file_path)
	{
		strtab_clear(&tokens);
		ft_free_p((void **)cmd_file_path);
		return (ft_eprintf("CHILD %d : Cannot find cmd \n", getpid()));
	}
//	ft_eprintf("CHILD %d : full cmd path : %s\n", getpid(), cmd_file_path);
	ft_free_p((void **)tokens);
	tokens[0] = cmd_file_path;
//	strtab_print(tokens);
//	ft_eprintf("CHILD %d : calling execve with cmd %s\n", getpid(), tokens[0]);
	ft_eprintf("\n\n Execution result : \n\n");
	execve(tokens[0], tokens, job->msh->envp);

	ft_eprintf("CHILD %d : execve call failed\n", getpid());
	strtab_clear(&tokens);
	return (-1);
}
