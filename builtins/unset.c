/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 08:32:40 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/19 21:06:26 by tbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Done, for now
// need to check var size, 200 is just for test
// Maybe fancy variable name validator bling bling.

static int	parse_unset_cmd(char *cmd)
{
	int	i;

	i = 5;
	while (ft_isspace(cmd[i]) > 0)
		i++;
	if (!(ft_isalpha(cmd[i]) || cmd[i] == '_'))
		return (-1);
	while (cmd[i] != '\0')
	{
		if (!(ft_isalnum(cmd[i]) || cmd[i] == '_'))
			return (-1);
		i++;
	}
	return (0);
}

int	msh_builtin_unset(t_job *job, char *cmd)
{
	char	var[200];
	int		i;
	int		j;

	i = 5;
	j = 0;
	if (parse_unset_cmd(cmd) == -1)
		return (-1);
	while (ft_isspace(cmd[i]) > 0)
		i++;
	if (cmd[i] == '\0')
		return (-1);
	while (cmd[i])
	{
		var[j++] = cmd[i++];
		if (cmd[i] == '=')
			return (0);
		if (j == 200)
			return (-1);
	}
	var[j] = '\0';
	if (msh_envp_remove_entry(job->msh, var) < 0)
		return (-1);
	return (0);
}
