/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intercept_builtin_calls.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 03:01:53 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/27 00:53:32 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	_validate_is_builtin(t_msh *msh, t_cmd *cmd)
{
	char	*cmd_str;

	cmd_str = cmd->tokens[0];
	printf("\n\n INTERCEPTING BUILTINS ! pid : %d\n", getpid());
	if (ft_strncmp(cmd_str, "echo", 4) == 0 && (!cmd_str[4]
			|| ft_isspace(cmd_str[4])))
		return (msh_builtin_echo(msh, cmd_str));
	else if (ft_strncmp(cmd_str, "cd", 2) == 0 && (!cmd_str[2]
			|| ft_isspace(cmd_str[2])))
		return (msh_builtin_cd(msh, cmd_str));
	else if (ft_strncmp(cmd_str, "export", 6) == 0 && (!cmd_str[6]
			|| ft_isspace(cmd_str[6])))
		return (msh_builtin_export(msh, cmd_str));
	else if (ft_strncmp(cmd_str, "unset", 5) == 0 && (!cmd_str[5]
			|| ft_isspace(cmd_str[5])))
		return (msh_builtin_unset(msh, cmd_str));
	else if (ft_strncmp(cmd_str, "pwd", 3) == 0 && (!cmd_str[3]
			|| ft_isspace(cmd_str[3])))
		return (msh_builtin_pwd(msh));
	else if (ft_strncmp(cmd_str, "env", 3) == 0 && (!cmd_str[3]
			|| ft_isspace(cmd_str[3])))
		return (msh_builtin_env(msh));
	else if (ft_strncmp(cmd_str, "exit", 4) == 0 && (!cmd_str[4]
			|| ft_isspace(cmd_str[4])))
		return (msh_builtin_exit(msh, cmd_str));
	else
		return (BUILTIN_NOT_FOUND);
}

int	intercept_builtin_call(t_job *job, t_cmd *cmd, int *builtin_status)
{
	*builtin_status = _validate_is_builtin(job->msh, cmd);
	return (0);
}
