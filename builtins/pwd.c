/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 05:06:50 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/09 16:04:30 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	msh_builtin_pwd(t_job *job, t_cmd *cmd)
{
	char	buff[PATH_MAX + 2];

	(void)job;
	(void)cmd;
	if (getcwd(buff, PATH_MAX + 1) == NULL)
		return (-1);
	ft_printf("%s\n", buff);
	job->msh->exit_status = 0;
	return (job->msh->exit_status);
}
