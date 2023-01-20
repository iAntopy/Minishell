/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 00:48:45 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/18 20:10:46 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	job_executor_force_exit(t_job *job, int *rd_pipe)
{
	pid_t	*pids;

	if (!job)
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
//	pid_t	pid;
	int	i;

	printf("fork child procs : entered : job %p, nb cmds : %d\n", job, job->nb_cmds);
	i = -1;
	while (++i < job->nb_cmds)//(job->nb_cmds - 1))
	{
//		usleep(100000);
//		printf("PARENT %d : forking cmd %d \n", getpid(), i);
		if (init_pipe(job->pp, &job->rd_pipe, i, job->nb_cmds) < 0)
			return (job_executor_force_exit(job, &job->rd_pipe));
//		printf("PARENT %d : opened pipe fds : rd %d, wr %d \n", getpid(), job->pp[0], job->pp[1]);
		job->pids[i] = fork();
		if (job->pids[i] < 0)
			return (job_executor_force_exit(job, &job->rd_pipe));
		else if (job->pids[i] == 0)
		{
//			ft_eprintf("CHILD %d : enter the BEAST\n", getpid());
			close_pipe(job->pp, NULL);
//			ft_eprintf("CHILD %d : closed pipe read side\n", getpid());
			if (job->rd_pipe != 0 && dup2(job->rd_pipe, 0)  < 0)
			{
				ft_eprintf("CHILD %d : first dup2 failed trying to plug rd_pipe (%d) to stdin\n", getpid(), job->rd_pipe);
				perror("CHILD perror");
			}
//			ft_eprintf("CHILD %d : dup prev pipe (%d) to stdin\n", getpid(), job->rd_pipe);
			if (job->pp[1] != 1 && dup2(job->pp[1], 1) < 0)
			{
				ft_eprintf("CHILD %d : second dup2 failed trying to plug write pipe (%d) to stdout\n", getpid(), job->pp[1]);
				perror("CHILD perror");
			}
//			ft_eprintf("CHILD %d : dup write pipe job->pp[1] (%d) to stdout \n", getpid(), job->pp[1]);
			if (parse_exec_cmd(job, i) < 0)
			{
				ft_eprintf("CHILD %d : exiting with ERROR\n", getpid());
				exit(job_clear(job, 0) | msh_clear(job->msh, errno));
			}
		}
		close_pipe(&job->rd_pipe, job->pp + 1);
		job->rd_pipe = job->pp[0];
	}
	return (0);
}

int	job_executor(t_job *job)
{
	int	builtin_status;
	int	i;
	
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
	printf("job exec : forking\n");
//	(void)fork_child_processes;
	if (fork_child_processes(job) < 0)
		return (-1);
	msh->exec_status = EXEC_MODE;
	i = -1;
	while (++i < job->nb_cmds)
		waitpid(job->pids[i], &job->msh->exit_status, 0);
	printf("\n\njob exec : forking DONE\n");
	return (0);
}
