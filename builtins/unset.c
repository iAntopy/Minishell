/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 08:32:40 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/18 03:19:40 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Done, for now
// need to check var size, 200 is just for test
// Maybe fancy variable name validator bling bling.

int	msh_builtin_unset(t_msh *msh, char *cmd)
{
	char	var[200];
	int		i;
	int		j;

	i = 5;
	j = 0;
	while (ft_isspace(cmd[i]) > 0)
		i++;
	if (cmd[i] == '\0')
		return (-1);
	while (cmd[i])
	{
		var[j++] = cmd[i++];
		if (cmd[i] == '=' || i == 199)
			return (-1);
	}
	var[j] = '\0';
	if (msh_envp_remove_entry(msh, var) < 0)
		return (-1);
	return (0);
}
