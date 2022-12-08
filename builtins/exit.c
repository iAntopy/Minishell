/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 04:08:20 by iamongeo          #+#    #+#             */
/*   Updated: 2022/12/08 04:32:22 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_exit_cmd(char *cmd, int *exit_status)
{
	while (!ft_isspace(*cmd))
		cmd++;
	while (ft_isspace(*cmd))
		cmd++;
	if (cmd)
		*exit_status = ft_atoi(cmd);
	*exit_status = EXIT_SUCCESS;
	return (0);
}

int	msh_builtin_exit(t_msh *msh, char *cmd)
{
	int	exit_status;

	if (!msh || !cmd)
		return (-1);
	parse_exit_cmd(cmd, &exit_status);
	msh->request_exit = 1;
	msh->shell_exit_status = exit_status;
	return (0);
}
