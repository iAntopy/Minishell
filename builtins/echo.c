/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 05:39:33 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/03 19:59:52 by tbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_n(char *str)
{
	int	i;

	i = 0;
	if (str[i] != '-')
		return (-1);
	else
		i++;
	while (str[i] != '\0')
	{
		if (str[i] != 'n')
			return (-1);
		i++;
	}
	return (0);
}

int	msh_builtin_echo(t_job *job, t_cmd *cmd)
{
	int		i;
	int		j;

	(void)job;
	i = 1;
	if (!cmd->tokens[1])
		return (printf("\n") & 0);
	while (check_n(cmd->tokens[i]) == 0)
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
	if (check_n(cmd->tokens[1]) == -1)
		printf("\n");
	job->msh->exit_status = 0;
	return (job->msh->exit_status);
}
