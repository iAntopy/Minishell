/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exec_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 02:36:44 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/18 06:51:37 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			F_OK | R_OK | X_OK) < 0
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
