/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 05:39:33 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/30 18:51:09 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	msh_builtin_echo(t_job *job, t_cmd *cmd)
{
	int		i;
	int		j;

	(void)job;
	i = 1;
	if (!cmd->tokens[1])
		printf("\n");
	if (!ft_strncmp(cmd->tokens[1], "-n", 3))
		i++;
	while (cmd->tokens[i])
	{
		j = 0;
		while (cmd->tokens[i][j])
		{
			printf("%c", cmd->tokens[i][j]);
			j++;
		}
		if (cmd->tokens[i + 1])
			printf(" ");
		i++;
	}
	if (ft_strncmp(cmd->tokens[1], "-n", 3))
		printf("\n");
	return (1);
}
