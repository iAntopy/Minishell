/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 08:32:40 by iamongeo          #+#    #+#             */
/*   Updated: 2022/12/04 08:33:01 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	msh_builtin_unset(t_msh *msh, char *var)
{
	// TODO : maybe validate the variable name if required.
	if (!var || !var[0])
		return (repport_invalid_var(__FUNCTION__, var));
	if (msh_envp_remove_entry(msh, var) < 0)
		return (-1);
	return (0);
}
