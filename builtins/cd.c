/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 04:00:34 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/12 16:31:04 by tbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

	// TODO :	find second word in cmd that should be a path and put '\0'
	//		and the end. return ptr to start of second word.
static int	swap_pwd(t_msh *msh, char *new_pwd, char *old_pwd)
{
	msh_envp_remove_entry(msh, "PWD");
	msh_envp_add_entry(msh, "PWD", new_pwd);
	msh_envp_remove_entry(msh, "OLDPWD");
	msh_envp_add_entry(msh, "OLDPWD", old_pwd);
	return (0);
}

	// TODO : Change PWD and OLDPWD in envp with msh_envp_[remove/add]_entry()
int	msh_builtin_cd(t_msh *msh, char *cmd)
{
	char	path[PATH_MAX + 1];
	char	old_pwd[PATH_MAX + 2];
	char	new_pwd[PATH_MAX + 2];
	int		i;
	int		j;

	i = 2;
	j = 0;
	if (!msh || !cmd || getcwd(old_pwd, PATH_MAX + 1) == NULL)
		return (-1);
	while (ft_isspace(cmd[i]) > 0)
		i++;
	while (cmd[i])
		path[j++] = cmd[i++];
	path[j] = '\0';
	if (chdir(path) == 0)
	{
		if (getcwd(new_pwd, PATH_MAX + 1) == NULL)
			return (-1);
		swap_pwd(msh, new_pwd, old_pwd);
	}
	return (0);
}
