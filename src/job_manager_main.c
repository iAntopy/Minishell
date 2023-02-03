/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_manager_main.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 00:26:12 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/02 21:21:30 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	job_clear(t_job *job, int return_status)
{
	int		i;
	char	tempname[PATH_MAX];

	if (!job)
		return (-1);
	if (job->parsed)
		ft_free_p((void **)&job->parsed);
	if (job->parsed2)
		ft_free_p((void **)&job->parsed2);
	if (job->pipe_split)
		strtab_clear(&job->pipe_split);
	i = -1;
	while (++i < MAX_CMDS)
	{
		strtab_clear(&job->cmds[i].tokens);
		close_pipe(&job->cmds[i].redir_in, &job->cmds[i].redir_out);
	}
	close_pipe(job->pp, job->pp + 1);
	close_pipe(&job->rd_pipe, NULL);
	while (job->heredoc_id > 0)
		unlink(gen_tempname(tempname, --job->heredoc_id));
	return (return_status);
}

static int	job_init(t_msh *msh)
{
	t_job	*job;

	strtab_clear(&msh->paths);
	msh->paths = get_env_paths(msh->envp);
	if (!msh->paths)
		return (report_jm_mlc_err(__FUNCTION__));
	job = &msh->job;
	msh->nbr_buff_len = ft_putnbr_buff(msh->nbr_buff, msh->exit_status);
	ft_memclear(job, sizeof(t_job));
	job->msh = msh;
	job->parsed = ft_strtrim(msh->rawline, " ");
	if (!job->parsed)
		return (report_jm_mlc_err(__FUNCTION__));
	return (0);
}

static int	free_swap_lines(t_job *job)
{
	if (!job->parsed2)
		return (0);
	ft_free_p((void **)&job->parsed);
	job->parsed = job->parsed2;
	job->parsed2 = NULL;
	return (0);
}

int	job_manager(t_msh *msh)
{
	t_job	*job;

	job = &msh->job;
	if (job_init(msh) < 0)
		return (job_clear(job, -1));
	if (validate_syntax(job->parsed, &msh->exit_status) < 0)
		return (job_clear(job, 258));
	if (ft_strchr(job->parsed, '$')
		&& (substitute_env_vars(msh, job) < 0 || free_swap_lines(job)))
		return (job_clear(job, report_jm_mlc_err(__FUNCTION__)));
	if (contains_meta_char(job->parsed)
		&& (spaceout_meta_chars(job) < 0 || free_swap_lines(job)))
		return (job_clear(job, -1));
	if (ft_strchr_set(job->parsed, "\'\""))
		job->sc = substring_substitution(job->parsed, &job->parsed2);
	if (job->sc < 0 || free_swap_lines(job))
		return (job_clear(job, report_jm_mlc_err(__FUNCTION__)));
	if (split_on_pipes(job) < 0 || setup_cmds(job) < 0 || job_executor(job) < 0)
		return (job_clear(job, -1));
	return (job_clear(job, job->msh->exit_status));
}
