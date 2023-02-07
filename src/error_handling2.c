/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 07:27:37 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/07 06:13:56 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	report_max_nb_cmds_exceeded(t_job *job)
{
	ft_eprintf("Minishell: Max number of cmds in pipeline exceeded : %d / %d\n",
		job->nb_cmds, MAX_CMDS);
	return (-1);
}

int	report_max_nb_pipelines_exceeded(t_msh *msh)
{
	ft_eprintf("Minishell: Max number of pipelines exceeded : %d / %d\n",
		msh->nb_plns, MAX_PIPELINES);
	return (-1);
}

int	report_file_error(char *filename, t_cmd *cmd)
{
	ft_eprintf("Minishell error :: %s : %s\n", strerror(errno), filename);
	cmd->doa = 1;
	cmd->job->msh->exit_status = errno;
	return (-1);
}

int	report_cmd_not_found(char *cmdname, t_cmd *cmd, int exit_code)
{
	ft_eprintf("Minishell: %s: command not found\n", cmdname);
	cmd->doa = 1;
	cmd->job->msh->exit_status = exit_code;
	return (-1);
}
