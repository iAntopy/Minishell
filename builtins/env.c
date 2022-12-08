/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 06:02:27 by iamongeo          #+#    #+#             */
/*   Updated: 2022/12/08 03:43:38 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	msh_builtin_env(t_msh *msh)
{
	// TODO : Remove fancy strtab print header and border.
	if (!msh || !msh->envp)
		return (-1);
	strtab_print(msh->envp);
	return (0);	
}
