/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 08:22:03 by iamongeo          #+#    #+#             */
/*   Updated: 2022/12/08 04:58:26 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_export_cmd(char *cmd, char **var, char **value)
{
	(void)cmd;
	(void)var;
	(void)value;
	// TODO : parse cmd and set var and value. If no value is given 
	// 	(if var[=value] has no [=value] (if no equal sign maybe ?))
	//	do nothing.
	return (0);
}

int	msh_builtin_export(t_msh *msh, char *cmd)//char **var, char **value)//char *var, char *value)
{
	char	*var;
	char	*value;
	char	*existing_value;

	if (parse_export_cmd(cmd, &var, &value) < 0)
		return (-1);
	// TODO : maybe validate the variable name and its value if required.
	existing_value = msh_getenv(msh, var);
	if (existing_value)
		msh_envp_remove_entry(msh, var);
	if (msh_envp_add_entry(msh, var, value) < 0)
		return (-1);
	return (0);
}
