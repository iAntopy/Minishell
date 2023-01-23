/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 06:02:27 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/19 19:53:12 by tbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO : Remove fancy strtab print header and border.
int	msh_builtin_env(t_job *job)
{
	if (!job || !job->msh->envp)
		return (-1);
	strtab_print(job->msh->envp);
	return (0);
}
