/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 04:00:34 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/19 19:53:33 by tbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Done, for now

static int	swap_pwd(t_msh *msh, char *new_pwd, char *old_pwd)
{
	msh_envp_remove_entry(msh, "PWD");
	msh_envp_add_entry(msh, "PWD", new_pwd);
	msh_envp_remove_entry(msh, "OLDPWD");
	msh_envp_add_entry(msh, "OLDPWD", old_pwd);
	return (0);
}

int	msh_builtin_cd(t_job *job, char *cmd)
{
	char	path[PATH_MAX + 1];
	char	old_pwd[PATH_MAX + 2];
	char	new_pwd[PATH_MAX + 2];
	int		i;
	int		j;

	i = 2;
	j = 0;
	if (!job || !cmd || getcwd(old_pwd, PATH_MAX + 1) == NULL)
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
		swap_pwd(job->msh, new_pwd, old_pwd);
	}
	return (0);
}
