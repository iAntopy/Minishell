/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 00:48:45 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/12 14:38:06 by tbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	job_executor_force_exit(t_job *job, int *rd_pipe)
{
	pid_t	*pids;

	if (!job || !job->pids)
		return (repport_missing_input(__FUNCTION__));
	close_pipe(job->pp, job->pp + 1);
	close_pipe(rd_pipe, NULL);
	pids = job->pids;
	while (*pids)
		kill(*pids, SIGKILL);
	ft_free_p((void **)&job->pids);
	return (-1);
}

static int	fork_child_processes(t_job *job)
{
	pid_t	pid;
	int		i;

	printf("fork child procs : entered : job %p, nb cmds : %d\n", job, job->nb_cmds);
	i = -1;
	while (++i < job->nb_cmds)//(job->nb_cmds - 1))
	{
		if (init_pipe(job->pp, &job->rd_pipe, i, job->nb_cmds) < 0)
			return (job_executor_force_exit(job, &job->rd_pipe));
		pid = fork();
		if (pid < 0)
			return (job_executor_force_exit(job, &job->rd_pipe));
		else if (pid == 0)
		{
			printf("CHILD %d : enter the BEAST\n", getpid());
			close_pipe(job->pp, NULL);
			dup2(job->rd_pipe, 0);
			dup2(job->pp[1], 1);
			job->pids[i] = pid;
			if (parse_exec_cmd(job->msh, job->pipe_split[i]) < 0)
			{
				printf("CHILD %d : exiting with ERROR\n", getpid());
				exit(msh_clear(job->msh, errno) | job_clear(job, 0));
			}
			printf("CHILD %d : exiting SUCCESSFULLY\n", getpid());
		}
		close_pipe(&job->rd_pipe, job->pp + 1);
		job->rd_pipe = job->pp[0];
	}
	return (0);
}

int	job_executor(t_job *job)
{
	int	builtin_status;

	if (!job)
		return (repport_missing_input(__FUNCTION__));
	printf("job exec : pipe split : \n");
	strtab_print(job->pipe_split);

	job->nb_cmds = strtab_len(job->pipe_split);

	if (job->nb_cmds == 1)
	{
		intercept_builtin_call(job, job->pipe_split[0], &builtin_status);
		if (builtin_status == BUILTIN_FAILED)
			return (repport_builtin_failure(__FUNCTION__));
		else if (builtin_status == BUILTIN_FOUND)
			return (0);
	}
	printf("job exec : mallocing pids\n");
	if (!ft_malloc_p(sizeof(pid_t) * job->nb_cmds,
			(void **)&job->pids))
		return (repport_jm_mlc_err(__FUNCTION__));
	//printf("job exec : mallocing DONE\n");
	//printf("job exec : forking\n");
	(void)fork_child_processes;
//	if (fork_child_processes(job) < 0)
//		return (-1);
	printf("job exec : forking DONE\n");
	return (0);
}
