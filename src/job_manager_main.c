/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_manager_main.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 00:26:12 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/20 23:34:45 by iamongeo         ###   ########.fr       */
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
	if (job->pipe_split)
		strtab_clear(&job->pipe_split);
	i = -1;
	while (++i < MAX_CMDS)
	{
		if (job->cmds[i])
			strtab_clear(job->cmds + i);
		if (job->redir_in[i] || job->redir_out[i])
			close_pipe(job->redir_in[i], job->redir_out[i]);
	}
	close_pipe(job->pp, job->pp + 1);
	close_pipe(&job->rd_pipe, NULL);
	while (job->heredoc_id > 0)
		unlink(generate_tempname(temp_name, --job->heredoc_id));
	return (return_status);
}

static void	free_swap_lines(char **old, char **new)
{
	if (!(*new))
		return ;
	ft_free_p((void **)old);
	*old = *new;
	*new = NULL;
}

int	job_parser(t_msh *msh)
{
	t_job	job;
	char	*cur_line;
//	char	**pipe_split;

	ft_memclear(&job, sizeof(t_job));
	job.msh = msh;
	cur_line = NULL;
	if (!msh)
		return (report_missing_input(__FUNCTION__));
	ft_printf("job manager main : line received : %s\n", msh->rawline);


	/// TRIMING RAWLINE ////
	job.parsed = ft_strtrim(msh->rawline, " ");

	/// SUBSTITUTE $ ENV VARS RESPECTING QUOTES ///
	if (ft_strchr(job.parsed, '$')
		&& msh_substitute_env_vars(msh, job.parsed, &cur_line) < 0)
		return (job_clear(&job, report_jm_mlc_err(__FUNCTION__)));
	printf("jm : cur line after env var substitutions : %s\n", cur_line);
	free_swap_lines(&job.parsed, &cur_line);
	printf("jm : parsed line after env var substitutions : %s\n", job.parsed);

	/// SPACING META CHARACTERS TO MAKE SPLITTING EASIER AND VALIDATE SYNTAX ///
	if (contains_meta_char(job.parsed)
		&& (spaceout_meta_chars(job.parsed, &cur_line) < 0
		|| validate_syntax(cur_line) < 0)
		return (job_clear(&job, -1));
	free_swap_lines(&job.parsed, &cur_line);
	ft_printf("jm : post meta char spacing : %s\n", job.parsed);


	/// SUBSTITUTING SPACES IN QUOTED SUBSTRINGS BY SUBSTITUTION CHARACTER ///
	if (ft_strchr_set(job.parsed, "\'\""))
		job.sc = substring_substitution(job.parsed, &cur_line);
	if (job.sc < 0)
		return (job_clear(&job, report_jm_mlc_err(__FUNCTION__)));
	free_swap_lines(&job.parsed, &cur_line);

	/// SPLITING RAWLINE ON PIPES ///
	if (split_cmd_on_pipes(job.parsed, &job.pipe_split) < 0)//ft_split(cur_line, '|');
		return (job_clear(&job, report_jm_mlc_err(__FUNCTION__)));
	printf("jm : cur_line after substitution and spacing : %s\n", job.parsed);
	
	job->nb_cmds = strtab_len(job->pipe_split);
	if (job->nb_cmds > MAX_CMDS)
		return (report_max_nb_cmds_exceeded(job));

	/// TOKENIZE ALL CMDS IN PIPELINE AND VALIDATE/OPEN REDIRECTIONS ///
	if (tokenize_all_cmds(&job) < 0)
		return (job_clear(&job, report_jm_mlc_err(__FUNCTION__)));
	if (apply_all_redirections(&job) < 0)
		return (job_clear(&job, 1));

	if (job_executor(&job) < 0)
		return (job_clear(&job, -1));
	printf("Return to job manager \n");

	return (job_clear(&job, job->msh->exit_status));
}
