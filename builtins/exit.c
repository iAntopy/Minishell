/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 04:08:20 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/19 19:52:50 by tbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	repport_exit_too_many_args(void)
{
	ft_eprintf("minishell: exit: too many arguments\n");
	return (-1);
}

static int	repport_exit_alpha_arg(void)
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

// exit will close shell with the given exit_code. If none are
// provided exits with 0 (EXIT_SUCCESS).
// 1. If more then 1 argument is provided, repports the error but 
// DOES NOT exit the shell.
// 2. If the single argument from exit contains anything other 
// the numeric characters, repport error and exit shell with 
// exit_code -1 (255).
// 3. If more then 1 arg provided and first contains non-numeric
// characters, same as case 2.
static int	parse_exit_cmd(char *cmd, int *exit_status)
{
	char	**split;
	int		must_exit;

	*exit_status = 0;
	must_exit = 1;
	split = ft_split(cmd, ' ');
	if (split[1])
	{
		if (!exit_status)
			repport_malloc_err(__FUNCTION__);
		else if (!validate_is_all_num(split[1]) && repport_exit_alpha_arg())
			*exit_status = -1;
		else if (strtab_len(split) > 2 && repport_exit_too_many_args())
			must_exit = 0;
		else
			*exit_status = ft_atoi(split[1]);
	}		
	strtab_clear(&split);
	return (must_exit);
}

// Yep outputs "exit" to stderr
int	msh_builtin_exit(t_job *job, char *cmd)
{
	int	exit_status;
	int	must_exit;	

	if (!job || !cmd)
		return (-1);
	ft_eprintf("exit\n");
	must_exit = parse_exit_cmd(cmd, &exit_status);
	job->msh->request_exit = must_exit;
	job->msh->shell_exit_status = exit_status;
	return (0);
}
