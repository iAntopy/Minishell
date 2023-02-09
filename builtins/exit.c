/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 04:08:20 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/07 05:11:17 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	report_exit_too_many_args(t_job *job)
{
	ft_eprintf("minishell: exit: too many arguments\n");
	job->msh->exit_status = 1;
	return (-1);
}

static int	report_exit_alpha_arg(t_job *job, char *str)
{
	ft_eprintf("minishell: exit: %s: numeric argument required\n", str);
	job->msh->exit_status = 255;
	return (-1);
}

static int	validate_is_all_num(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	while (arg[i])
	{
		if (!('0' <= arg[i] && arg[i] <= '9'))
			return (0);
		i++;
	}
	if (i > 18)
		return (0);
	return (1);
}

static int	parse_exit_cmd(t_job *job, char **cmd, int *exit_status)
{
	int		must_exit;

	*exit_status = 0;
	must_exit = 1;
	if (cmd[1])
	{
		if (!exit_status)
			report_malloc_err();
		else if (!validate_is_all_num(cmd[1])
			&& report_exit_alpha_arg(job, cmd[1]))
			*exit_status = -1;
		else if (strtab_len(cmd) > 2 && report_exit_too_many_args(job))
			must_exit = 0;
		else
			*exit_status = (ft_atoi(cmd[1]) % 256);
	}		
	return (must_exit);
}

int	msh_builtin_exit(t_job *job, t_cmd *cmd)
{
	int	exit_status;
	int	must_exit;	

	job->msh->exit_status = 0;
	if (!job || !cmd->tokens)
		return (-1);
	ft_eprintf("exit\n");
	must_exit = parse_exit_cmd(job, cmd->tokens, &exit_status);
	job->msh->request_exit = must_exit;
	job->msh->shell_exit_status = exit_status;
	return (job->msh->exit_status);
}
