/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 04:00:34 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/02 20:16:49 by tbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Done, for now

static int	swap_pwd(t_msh *msh, char *new_pwd, char *old_pwd)
{
	if (getcwd(new_pwd, PATH_MAX + 1) == NULL)
		return (1);
	msh_envp_remove_entry(msh, "PWD");
	msh_envp_add_entry(msh, "PWD", new_pwd);
	msh_envp_remove_entry(msh, "OLDPWD");
	msh_envp_add_entry(msh, "OLDPWD", old_pwd);
	return (0);
}

char	*get_user_path(t_job *job, int *status)
{
	char	*str;
	char	*str2;
	char	*str3;

	str = "/Users/";
	str2 = msh_getenv(job->msh, "USER");
	str3 = ft_strjoin(str, str2);
	*status = 1;
	return (str3);
}

int	msh_builtin_cd(t_job *job, t_cmd *cmd)
{
	char	*path;
	char	old_pwd[PATH_MAX + 2];
	char	new_pwd[PATH_MAX + 2];
	int		status;

	status = 0;
	if (!job || !cmd->tokens || getcwd(old_pwd, PATH_MAX + 1) == NULL)
		return (-1);
	if (!cmd->tokens[1])
		path = get_user_path(job, &status);
	else
		path = cmd->tokens[1];
	if (chdir(path) == 0)
		swap_pwd(job->msh, new_pwd, old_pwd);
	else
	{
		printf("cd: %s: No such file or directory\n", path);
		job->msh->exit_status = 1;
		return (job->msh->exit_status);
	}
	if (status == 1)
		free(path);
	job->msh->exit_status = 0;
	return (job->msh->exit_status);
}
