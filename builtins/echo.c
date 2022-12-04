/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 05:39:33 by iamongeo          #+#    #+#             */
/*   Updated: 2022/12/04 06:00:58 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	msh_builtin_echo(t_msh *msh, char **strs)
{
	int	new_line;

	if (!strs)
		return (-1);
	new_line = 1;
	while (*strs)
	{
		if (ft_strncmp(*strs, "-n", 2) == 0)
			new_line = 0;
		else if (*(strs + 1))
			ft_printf("%s ", *strs);
		else
			ft_putstr(*strs);
		strs++;
	}
	if (new_line)
		write(1, "\n", 1);
	return (0);
}
