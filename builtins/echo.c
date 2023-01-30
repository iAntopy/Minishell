/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 05:39:33 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/27 17:47:29 by tbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static int	parse_echo_cmd(char *cmd, char ***strs)
// {
// 	(void)cmd;
// 	(void)strs;
// 	return (0);
// }

	// TODO : parse cmd to substitute environment variables with 
	//		substitute_env_vars(t_msh *msh, char *line, char **ret)
	//		then print without quotes chars.
	//		ps : first word in cmd is 'echo'.
// int	msh_builtin_echo(t_job *job, char *cmd)//char **strs)
// {
// 	int		new_line;
// 	char	**strs;

// 	(void)job;
// 	if (parse_echo_cmd(cmd, &strs) < 0)
// 		return (-1);
// 	new_line = 1;
// 	while (*strs)
// 	{
// 		if (ft_strncmp(*strs, "-n", 2) == 0)
// 			new_line = 0;
// 		else if (*(strs + 1))
// 			ft_printf("%s ", *strs);
// 		else
// 			ft_putstr(*strs);
// 		strs++;
// 	}
// 	if (new_line)
// 		write(1, "\n", 1);
// 	return (0);
// }

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
