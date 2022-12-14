/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 08:32:40 by iamongeo          #+#    #+#             */
/*   Updated: 2022/12/08 04:26:56 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_unset_cmd(char *cmd, char **var)
{
	(void)cmd;
	(void)var;
	// TODO :	find second word and put '\0' after it.
	//		then *var = start of second word.
	return (0);
}

int	msh_builtin_unset(t_msh *msh, char *cmd)//char *var)
{
	char	*var;

	// TODO : maybe validate the variable name if required.
	if (parse_unset_cmd(cmd, &var) < 0)
		return (-1);
	if (msh_envp_remove_entry(msh, var) < 0)
		return (-1);
	return (0);
}
