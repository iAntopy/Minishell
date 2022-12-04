/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 05:06:50 by iamongeo          #+#    #+#             */
/*   Updated: 2022/12/04 05:39:04 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	msh_builtin_pwd(void)
{
	char	buff[PATH_MAX + 2];

	if (getcwd(buff, PATH_MAX + 1) < 0)
		return (-1);
	ft_printf("%s\n", buff);
	return (0);
}
