/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 04:00:34 by iamongeo          #+#    #+#             */
/*   Updated: 2022/12/08 04:07:50 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_cd_cmd(char *cmd, char **path)
{
	(void)cmd;
	(void)path;
	// TODO :	find second word in cmd that should be a path and put '\0'
	//		and the end. return ptr to start of second word.
	return (0);
}

int	msh_builtin_cd(t_msh *msh, char *cmd)
{
	char	*path;

	if (!msh || !cmd)
		return (-1);
	parse_cd_cmd(cmd, &path);
	// TODO : Change PWD and OLDPWD in envp with msh_envp_[remove/add]_entry()

	return (0);
}
