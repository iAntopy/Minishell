/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 04:08:20 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/30 18:51:57 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	report_exit_too_many_args(void)
{
	ft_eprintf("minishell: exit: too many arguments\n");
	return (-1);
}

static int	report_exit_alpha_arg(void)
{
	ft_eprintf("minishell: exit: numeric argument required\n");
	return (-1);
}

static int	validate_is_all_num(char *arg)
{
	while (arg && *arg)
	{
		if (!('0' <= *arg && *arg <= '9'))
			return (0);
		arg++;
	}
	return (1);
}

static int	parse_exit_cmd(char **cmd, int *exit_status)
{
	int		must_exit;

	*exit_status = 0;
	must_exit = 1;
	if (cmd[1])
	{
		if (!exit_status)
			report_malloc_err(__FUNCTION__);
		else if (!validate_is_all_num(cmd[1]) && report_exit_alpha_arg())
			*exit_status = -1;
		else if (strtab_len(cmd) > 2 && report_exit_too_many_args())
			must_exit = 0;
		else
			*exit_status = ft_atoi(cmd[1]);
	}		
	return (must_exit);
}

int	msh_builtin_exit(t_job *job, t_cmd *cmd)
{
	int	exit_status;
	int	must_exit;	

	if (!job || !cmd->tokens)
		return (-1);
	ft_eprintf("exit\n");
	must_exit = parse_exit_cmd(cmd->tokens, &exit_status);
	job->msh->request_exit = must_exit;
	job->msh->shell_exit_status = exit_status;
	return (0);
}
