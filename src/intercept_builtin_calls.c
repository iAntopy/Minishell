/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intercept_builtin_calls.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 03:01:53 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/17 12:31:49 by tbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	_validate_is_builtin(t_msh *msh, char *cmd)
{
	if (ft_strncmp(cmd, "echo", 4) == 0 && (!cmd[4]
			|| ft_isspace(cmd[4])))
		return (msh_builtin_echo(msh, cmd));
	else if (ft_strncmp(cmd, "cd", 2) == 0 && (!cmd[2]
			|| ft_isspace(cmd[2])))
		return (msh_builtin_cd(msh, cmd));
	else if (ft_strncmp(cmd, "export", 6) == 0 && (!cmd[6]
			|| ft_isspace(cmd[6])))
		return (msh_builtin_export(msh, cmd));
	else if (ft_strncmp(cmd, "unset", 5) == 0 && (!cmd[5]
			|| ft_isspace(cmd[5])))
		return (msh_builtin_unset(msh, cmd));
	else if (ft_strncmp(cmd, "pwd", 3) == 0 && (!cmd[3]
			|| ft_isspace(cmd[3])))
		return (msh_builtin_pwd(msh));
	else if (ft_strncmp(cmd, "env", 3) == 0 && (!cmd[3]
			|| ft_isspace(cmd[3])))
		return (msh_builtin_env(msh));
	else if (ft_strncmp(cmd, "exit", 4) == 0 && (!cmd[4]
			|| ft_isspace(cmd[4])))
		return (msh_builtin_exit(msh, cmd));
	else
		return (BUILTIN_NOT_FOUND);
}

/*
static int	_remove_builtin_cmd(char **cmds)
{
	ft_free_p((void **)cmds);
	while (*(cmds + 1))
	{
		*cmds = *(cmds + 1);
		cmds++;
	}
	*cmds = NULL;
	return (0);
}
*/

int	intercept_builtin_call(t_job *job, char *cmd, int *builtin_status)
{
	while (ft_isspace(*cmd))
		cmd++;
	*builtin_status = _validate_is_builtin(job->msh, cmd);
	return (0);
}
