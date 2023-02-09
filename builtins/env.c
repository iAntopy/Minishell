/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 06:02:27 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/09 16:04:20 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	msh_builtin_env(t_job *job, t_cmd *cmd)
{
	(void)cmd;
	if (!job || !job->msh->envp)
	{
		job->msh->exit_status = -1;
		return (job->msh->exit_status);
	}
	if (cmd->tokens[1])
		msh_builtin_export(job, cmd);
	strtab_print(job->msh->envp);
	job->msh->exit_status = 0;
	return (job->msh->exit_status);
}
