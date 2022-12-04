/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 08:22:03 by iamongeo          #+#    #+#             */
/*   Updated: 2022/12/04 08:33:15 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	msh_builtin_export(t_msh *msh, char *var, char *value)
{
	// TODO : maybe validate the variable name and its value if required.
	if (!var || !var[0])
		return (repport_invalid_var(__FUNCTION__, var));
	if (msh_envp_add_entry(msh, var, value) < 0)
		return (-1);
	return (0);
}
