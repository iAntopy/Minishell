/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 15:23:20 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/09 15:23:34 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	msh_getenv_export(t_msh *msh, char *var, char **ret_val)
{
	char	**envp;
	int		i;
	char	var_buff[1024];

	if (!msh || !msh->envp || !var || !var[0])
		return (0);
	*ret_val = "";
	if (*var == '?')
	{
		*ret_val = msh->nbr_buff;
		return (1);
	}
	envp = msh->envp;
	i = -1;
	while (envp[++i])
	{
		ft_strlcpy(var_buff, envp[i], ft_strchr(envp[i], '=') - envp[i] + 1);
		if (ft_strcmp(var, var_buff) == 0)
		{
			*ret_val = (ft_strchr(envp[i], '=') + 1);
			return (1);
		}
	}
	return (0);
}
