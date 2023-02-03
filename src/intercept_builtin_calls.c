/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intercept_builtin_calls.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 03:01:53 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/02 20:25:24 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	intercept_builtin_call(t_cmd *cmd)
{
	char	*cmd_str;

	cmd_str = cmd->tokens[0];
	if (ft_strncmp(cmd_str, "echo", 5) == 0)
		cmd->builtin = msh_builtin_echo;
	else if (ft_strncmp(cmd_str, "cd", 3) == 0)
		cmd->builtin = msh_builtin_cd;
	else if (ft_strncmp(cmd_str, "export", 7) == 0)
		cmd->builtin = msh_builtin_export;
	else if (ft_strncmp(cmd_str, "unset", 6) == 0)
		cmd->builtin = msh_builtin_unset;
	else if (ft_strncmp(cmd_str, "pwd", 4) == 0)
		cmd->builtin = msh_builtin_pwd;
	else if (ft_strncmp(cmd_str, "env", 4) == 0)
		cmd->builtin = msh_builtin_env;
	else if (ft_strncmp(cmd_str, "exit", 5) == 0)
		cmd->builtin = msh_builtin_exit;
	else
		return (BUILTIN_NOT_FOUND);
	return (BUILTIN_FOUND);
}
