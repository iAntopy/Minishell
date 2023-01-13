/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 06:02:27 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/12 14:41:19 by tbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO : Remove fancy strtab print header and border.
int	msh_builtin_env(t_msh *msh)
{
	if (!msh || !msh->envp)
		return (-1);
	strtab_print(msh->envp);
	return (0);
}
