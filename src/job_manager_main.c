/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_manager_main.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 00:26:12 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/12 12:56:57 by tbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	job_clear(t_job *job, int return_status)
{
	if (!job)
		return (-1);
	if (job->parsed)
		ft_free_p((void **)&job->parsed);
	if (job->pids)
		ft_free_p((void **)&job->pids);
	if (job->pipe_split)
		strtab_clear(&job->pipe_split);
	close_pipe(job->pp, job->pp + 1);
	close_pipe(&job->rd_pipe, NULL);
	return (return_status);
}
/*
int	job_executor(t_job *job)
{
	if (!job || !job->msh)
		return (repport_missing_input(__FUNCTION__));
	printf("job exec : pipe split : \n");
	strtab_print(job->pipe_split);
	job->nb_cmds = strtab_len(job->pipe_split);
	if (ft_malloc_p(sizeof(pid_t) * job->nb_cmds,
		(void **)&job->pids))
		return (repport_jm_mlc_err(__FUNCTION__));
	
	return (0);
}
*/

static void	free_swap_lines(char **old, char **new)
{
	if (!(*new))
		return ;
	ft_free_p((void **)old);
	*old = *new;
	*new = NULL;
}

int	job_manager(t_msh *msh)
{
	t_job	job;
	char	*cur_line;
//	char	**pipe_split;

	ft_memclear(&job, sizeof(t_job));
	job.msh = msh;
	cur_line = NULL;
	if (!msh)
		return (repport_missing_input(__FUNCTION__));
	ft_printf("job manager main : line received : %s\n", msh->rawline);
	job.parsed = ft_strtrim(msh->rawline, " ");

	if (ft_strchr(job.parsed, '$')
		&& msh_substitute_env_vars(msh, job.parsed, &cur_line) < 0)
		return (job_clear(&job, repport_jm_mlc_err(__FUNCTION__)));
	printf("jm : cur line after env var substitutions : %s\n", cur_line);
	free_swap_lines(&job.parsed, &cur_line);

	printf("jm : parsed line after env var substitutions : %s\n", job.parsed);

	if (contains_meta_char(job.parsed)
		&& spaceout_meta_chars(job.parsed, &cur_line) < 0)
		return (job_clear(&job, -1));
	free_swap_lines(&job.parsed, &cur_line);
	ft_printf("jm : post meta char spacing : %s\n", job.parsed);

	if (ft_strchr_set(job.parsed, "\'\""))
		job.sc = substring_substitution(job.parsed, &cur_line);
	if (job.sc < 0)
		return (job_clear(&job, repport_jm_mlc_err(__FUNCTION__)));
	free_swap_lines(&job.parsed, &cur_line);


	if (split_cmd_on_pipes(job.parsed, &job.pipe_split) < 0)//ft_split(cur_line, '|');
		return (job_clear(&job, repport_jm_mlc_err(__FUNCTION__)));
	printf("jm : cur_line after substitution and spacing : %s\n", job.parsed);

	if (job_executor(&job) < 0)
		return (job_clear(&job, -1));

	return (job_clear(&job, EXIT_SUCCESS));
}
