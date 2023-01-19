/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 08:22:03 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/17 12:32:26 by tbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_export_cmd(char *cmd, char **var, char **value)
{
	char	**split;

	if (!ft_strchr(cmd, '='))
		return (0);
	while (*cmd && !ft_isspace(*cmd))
		cmd++;
	while (*cmd && ft_isspace(*cmd))
		cmd++;
	split = ft_split(cmd, '=');
	if (!split || !split[0])
		return (repport_malloc_err(__FUNCTION__));
	*var = split[0];
	*value = split[1];
	free(split);
	return (0);
}

int	msh_builtin_export(t_msh *msh, char *cmd)
{
	char	*var;
	char	*value;
	char	*existing_value;
	int		ret_val;

	ret_val = 0;
	if (!ft_strchr(cmd, '='))
		return (0);
	if (parse_export_cmd(cmd, &var, &value) < 0)
		return (-1);
	existing_value = msh_getenv(msh, var);
	if (existing_value)
		msh_envp_remove_entry(msh, var);
	ret_val = msh_envp_add_entry(msh, var, value);
	ft_free_p((void **)&var);
	ft_free_p((void **)&value);
	return (ret_val);
}
