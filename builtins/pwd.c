/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 05:06:50 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/30 18:36:25 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Done, for now
int	msh_builtin_pwd(t_job *job, t_cmd *cmd)
{
	char	buff[PATH_MAX + 2];

	(void)job;
	(void)cmd;
	if (getcwd(buff, PATH_MAX + 1) == NULL)
		return (-1);
	ft_printf("%s\n", buff);
	return (0);
}
