/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_manager_main.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 00:26:12 by iamongeo          #+#    #+#             */
/*   Updated: 2022/12/06 20:02:19 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct	s_job
{
	char	sc;		// substitution character
	char	*var_subst;	// 
	char	*spaced;	// spaced meta char from spaceout_meta_chars()
	char	*substr_subst;		// substituted substrings
	char	**pipe_split;
}	t_job;

int	jm_clear(t_job *job, int return_status)
{
	if (!job)
		return (-1);
	if (job->var_subst)
		ft_free_p((void **)&job->var_subst);
	if (job->substr_subst)
		ft_free_p((void **)&job->substr_subst);
	if (job->spaced)
		ft_free_p((void **)&job->spaced);
	return (return_status);
}

int	jm_manage_job(t_msh *msh)
{
	t_job	job;
	char	*cur_line;
//	char	**pipe_split;

	ft_memclear(&job, sizeof(t_job));
	cur_line = NULL;
	if (!msh)
		return (repport_missing_input(__FUNCTION__));
	ft_printf("job manager main : line received : %s\n", msh->rawline);
	cur_line = msh->rawline;

	if (ft_strchr(cur_line, '$') && msh_substitute_env_vars(msh, cur_line, &job.var_subst) < 0)
		return (jm_clear(&job, repport_jm_mlc_err(__FUNCTION__)));
	printf("WOWSERS !\n");
	if (job.var_subst)
		cur_line = job.var_subst;
	printf("jm : cur line after env var substitutions : %s\n", job.var_subst);

	if (contains_meta_char(cur_line)
		&& spaceout_meta_chars(cur_line, &job.spaced) < 0)
		return (jm_clear(&job, repport_jm_mlc_err(__FUNCTION__)));
	if (job.spaced)
		cur_line = job.spaced;
	ft_printf("jm : post meta char spacing : %s\n", cur_line);

	job.sc = substring_substitution(cur_line, &job.substr_subst);
	if (job.sc < 0)
		return (jm_clear(&job, repport_jm_mlc_err(__FUNCTION__)));
	if (job.substr_subst)
		cur_line = job.substr_subst;

	job.pipe_split = ft_split(cur_line, '|');
	strtab_print(job.pipe_split);
	printf("jm : cur_line after substitution and spacing : %s\n", cur_line);
//	printf("jm : msh_getenv TERM : %s\n", msh_getenv(msh, "TERM"));
	return (jm_clear(&job, EXIT_SUCCESS));
}
