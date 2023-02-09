/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipelines_manager.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 07:01:40 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/09 03:25:19 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	msh_clear_pipelines(t_msh *msh, int return_value)
{
	int	i;

	i = 0;
	while (i < msh->nb_plns)
	{
		ft_free_p((void **)&msh->pipelines[i]);
		msh->pl_meta_bools[i] = 0;
		i++;
	}
	msh->nb_plns = 0;
	return (return_value);
}

int	msh_pipelines_manager(t_msh *msh)
{
	int	i;
	int	bl;

	if (validate_syntax(msh->rawline, &msh->exit_status) < 0
		|| split_on_bools(msh) < 0)
		return (msh_clear_pipelines(msh, -1));
	printf("pipelines manager : nb_plns : %d\n", msh->nb_plns);
	i = -1;
	while (++i < msh->nb_plns && !msh->sigint_flag)
	{
		bl = msh->pl_meta_bools[i];
		if (job_manager(msh, msh->pipelines[i]) < 0)
			break ;
		if (bl && ((bl == BOOL_AND && msh->exit_status != EXIT_SUCCESS)
				|| (bl == BOOL_OR && msh->exit_status == EXIT_SUCCESS)))
			break ;
	}
	if (msh->sigint_flag)
		msh->exit_status = 130;
	msh->sigint_flag = 0;
	return (msh_clear_pipelines(msh, 0));
}
