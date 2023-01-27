/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 05:06:50 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/27 05:52:38 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Done, for now

int	msh_builtin_pwd(t_msh *msh, char *cmd)
{
	char	buff[PATH_MAX + 2];

	(void)msh;
	(void)cmd;
	if (getcwd(buff, PATH_MAX + 1) == NULL)
		return (-1);
	ft_printf("%s\n", buff);
	return (0);
}
