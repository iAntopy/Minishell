/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intercept_builtin_calls.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 03:01:53 by iamongeo          #+#    #+#             */
/*   Updated: 2022/12/08 05:00:35 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	_validate_is_builtin(t_msh *msh, char *cmd)
{
	if (ft_strncmp(cmd, "echo", 4) == 0)
		msh_builtin_echo(msh, cmd);
	else if (ft_strncmp(cmd, "cd", 2) == 0)
		msh_builtin_cd(msh, cmd);
	else if (ft_strncmp(cmd, "export", 6) == 0)
		msh_builtin_export(msh, cmd);
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		msh_builtin_unset(msh, cmd);
	else if (ft_strncmp(cmd, "pwd", 3) == 0)
		msh_builtin_pwd(msh);
	else if (ft_strncmp(cmd, "env", 3) == 0)
		msh_builtin_env(msh);
	else if (ft_strncmp(cmd, "exit", 4) == 0)
		msh_builtin_exit(msh, cmd);
	else
		return (0);
	return (1);
}

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

int	intercept_builtin_calls(t_job *job)
{
	char	**cmds;
	char	*c;

	cmds = job->pipe_split;
	while (*cmds)
	{
		c = *cmds;
		while (ft_isspace(*c))
			c++;
		if (_validate_is_builtin(job->msh, c))
			return (_remove_builtin_cmd(cmds));
		cmds++;
	}
	return (0);
}
