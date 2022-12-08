/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 05:39:33 by iamongeo          #+#    #+#             */
/*   Updated: 2022/12/08 04:29:40 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_echo_cmd(char *cmd, char ***strs)
{
	(void)cmd;
	(void)strs;
	return (0);
}

int	msh_builtin_echo(t_msh *msh, char *cmd)//char **strs)
{
	int	new_line;
	char	**strs;

	// TODO : parse cmd to substitute environment variables with 
	//		substitute_env_vars(t_msh *msh, char *line, char **ret)
	//		then print without quotes chars.
	//		ps : first word in cmd is 'echo'.
	(void)msh;
	if (parse_echo_cmd(cmd, &strs) < 0)
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
