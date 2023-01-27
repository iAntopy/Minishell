/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 00:48:45 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/27 07:55:28 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static int	job_executor_force_exit(t_job *job, int *rd_pipe)
{
	int	i;

	if (!job)
		return (report_missing_input(__FUNCTION__));
	close_pipe(job->pp, job->pp + 1);
	close_pipe(rd_pipe, NULL);
	i = -1;
	while (++i < job->nb_cmds)
		if (job->cmds[i].pid)
			kill(job->cmds[i].pid, SIGKILL);
	return (-1);
}

static void	setup_child_redirections(t_cmd *cmd)
{
	close_fd(&cmd->job->pp[0]);
	if (cmd->redir_in)
	{
		close_fd(&cmd->job->rd_pipe);
		dup2(cmd->redir_in, STDIN_FILENO);
	}
	else if (cmd->job->rd_pipe != STDIN_FILENO)
		dup2(cmd->job->rd_pipe, STDIN_FILENO);
	if (cmd->redir_out)
	{
		close_fd(&cmd->job->pp[1]);
		dup2(cmd->redir_out, STDOUT_FILENO);
	}
	else if (cmd->job->pp[1] != STDOUT_FILENO)
		dup2(cmd->job->pp[1], STDOUT_FILENO);
}

static int	fork_child_processes(t_job *job)
{
//	pid_t	pid;
	int	i;
	int	bltin_status;

	printf("fork child procs : entered : job %p, nb cmds : %d\n", job, job->nb_cmds);
	i = -1;
	while (++i < job->nb_cmds)//(job->nb_cmds - 1))
	{
//		usleep(100000);
//		printf("PARENT %d : forking cmd %d \n", getpid(), i);
		if (init_pipe(job->pp, &job->rd_pipe, i, job->nb_cmds) < 0)
			return (job_executor_force_exit(job, &job->rd_pipe));
//		printf("PARENT %d : opened pipe fds : rd %d, wr %d \n", getpid(), job->pp[0], job->pp[1]);
		job->cmds[i].pid = fork();
		if (job->cmds[i].pid < 0)
			return (job_executor_force_exit(job, &job->rd_pipe));
		else if (job->cmds[i].pid == 0)
		{
//			ft_eprintf("CHILD %d : enter the BEAST\n", getpid());
			setup_child_redirections(job->cmd + i);
			if (job->bltin_func)
				job->cmd[i].bltin_func(job, job->cmds[i]);
			else if (!job->cmds[i].doa)
				execve(job->cmds[i].tokens[0],
					job->cmds[i].tokens, job->msh->envp);
			exit(job_clear(job, 0) | msh_clear(job->msh, errno));
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
		return (report_missing_input(__FUNCTION__));

	if (job->nb_cmds == 1 && job->bltin_func)
	{
		if (job->bltin_func(job, job->cmds[i]) == BUILTIN_FAILED)
			return (report_builtin_failure(__FUNCTION__));
		return (0);
	}

	printf("job exec : forking\n");
//	(void)fork_child_processes;
	if (fork_child_processes(job) < 0)
		return (-1);
	job->msh->exec_status = EXEC_MODE;
	i = -1;
	while (++i < job->nb_cmds)
		waitpid(job->cmds[i].pid, &job->msh->exit_status, 0);
	printf("\n\njob exec : forking DONE\n");
	return (0);
}
