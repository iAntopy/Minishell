/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 08:32:40 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/02 20:10:39 by tbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_unset_cmd(t_job *job, char *cmd, char *cmd_name)
{
	int	i;

	i = 0;
	if (!(ft_isalpha(cmd[0]) || cmd[0] == '_'))
	{
		ft_eprintf("%s: `%s': not a valid identifier\n", cmd_name, cmd);
		job->msh->exit_status = 1;
		return (-1);
	}
	while (cmd[i] != '\0' && cmd[i] != '=')
	{
		if (!(ft_isalnum(cmd[i]) || cmd[i] == '_'))
		{
			ft_eprintf("%s: `%s': not a valid identifier\n", cmd_name, cmd);
			job->msh->exit_status = 1;
			return (-1);
		}
		i++;
	}
	return (0);
}

static int	pre_parse(t_job *job, char **token_tab)
{
	int		i;

	i = 0;
	while (token_tab[++i])
	{
		if (parse_unset_cmd(job, token_tab[i], "unset") == 0)
		{
			if (msh_envp_remove_entry(job->msh, token_tab[i]) < 0)
			{
				job->msh->exit_status = 1;
				return (job->msh->exit_status);
			}
		}
	}
	return (0);
}

int	msh_builtin_unset(t_job *job, t_cmd *cmd)
{
	job->msh->exit_status = 0;
	if (cmd->tokens == NULL)
		return (-1);
	if (pre_parse(job, cmd->tokens) < 0)
		return (job->msh->exit_status);
	return (job->msh->exit_status);
}
