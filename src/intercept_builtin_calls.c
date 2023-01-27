/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intercept_builtin_calls.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 03:01:53 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/27 07:39:23 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	_validate_is_builtin(char *cmd_str)
{
	if (ft_strncmp(cmd_str, "echo", 4) == 0)
		cmd->bltin_func = msh_builtin_echo;
//		return (msh_builtin_echo(msh, cmd_str));
	else if (ft_strncmp(cmd_str, "cd", 2) == 0)
		cmd->bltin_func = msh_builtin_cd;
//		return (msh_builtin_cd(msh, cmd_str));
	else if (ft_strncmp(cmd_str, "export", 6) == 0)
		cmd->bltin_func = msh_builtin_export;
//		return (msh_builtin_export(msh, cmd_str));
	else if (ft_strncmp(cmd_str, "unset", 5) == 0)
		cmd->bltin_func = msh_builtin_unset;
//		return (msh_builtin_unset(msh, cmd_str));
	else if (ft_strncmp(cmd_str, "pwd", 3) == 0)
		cmd->bltin_func = msh_builtin_pwd;
//		return (msh_builtin_pwd(msh));
	else if (ft_strncmp(cmd_str, "env", 3) == 0)
		cmd->bltin_func = msh_builtin_env;
//		return (msh_builtin_env(msh));
	else if (ft_strncmp(cmd_str, "exit", 4) == 0)
		cmd->bltin_func = msh_builtin_exit;
//		return (msh_builtin_exit(msh, cmd_str));
	else
		return (BUILTIN_NOT_FOUND);
	return (BUILTIN_FOUND);
}

int	intercept_builtin_call(t_cmd *cmd, int *builtin_status)
{
	*builtin_status = _validate_is_builtin(cmd->tokens[0]);
	return (0);
}
