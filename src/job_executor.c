/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 00:48:45 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/01 07:06:54 by iamongeo         ###   ########.fr       */
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
		ft_eprintf("attempt closing rd_pipe %d\n", cmd->job->rd_pipe);
		close_fd(&cmd->job->rd_pipe);
		ft_eprintf("fd %d becomes new STDIN\n", cmd->redir_in);
		dup2(cmd->redir_in, STDIN_FILENO);
	}
	else if (cmd->job->rd_pipe != STDIN_FILENO)
		dup2(cmd->job->rd_pipe, STDIN_FILENO);
	if (cmd->redir_out)
	{
		ft_eprintf("attempt closing write pipe %d\n", cmd->job->pp[1]);
		close_fd(&cmd->job->pp[1]);
		ft_eprintf("fd %d becomes new STDOUT\n", cmd->redir_out);
		dup2(cmd->redir_out, STDOUT_FILENO);
	}
	else if (cmd->job->pp[1] != STDOUT_FILENO)
		dup2(cmd->job->pp[1], STDOUT_FILENO);
}

static void	reset_standard_io(t_cmd *cmd)
{
	if (cmd->redir_in >= 3)
		dup2(cmd->job->msh->stdin_fd, STDIN_FILENO);
	if (cmd->redir_out >= 3)
		dup2(cmd->job->msh->stdout_fd, STDOUT_FILENO);
}

static int	fork_child_processes(t_job *job)
{
	int	i;

//	printf("fork child procs : entered : job %p, nb cmds : %d\n", job, job->nb_cmds);
	i = -1;
	while (++i < job->nb_cmds)//(job->nb_cmds - 1))
	{
//		usleep(100000);
//		printf("PARENT %d : forking cmd %d, redir_in %d, redir_out %d \n", getpid(), i, job->cmds[i].redir_in, job->cmds[i].redir_out);
		if (init_pipe(job->pp, &job->rd_pipe, i, job->nb_cmds) < 0)
			return (job_executor_force_exit(job, &job->rd_pipe));
//		printf("PARENT %d : opened pipe fds : rd %d, wr %d \n", getpid(), job->pp[0], job->pp[1]);
		job->cmds[i].pid = fork();
		if (job->cmds[i].pid < 0)
			return (job_executor_force_exit(job, &job->rd_pipe));
		else if (job->cmds[i].pid == 0)
		{
//			ft_eprintf("CHILD %d : enter the BEAST\n", getpid());
			setup_child_redirections(job->cmds + i);
			if (job->cmds[i].bltin_func)
				job->cmds[i].bltin_func(job, &job->cmds[i]);
			else if (!job->cmds[i].doa)
				execve(job->cmds[i].tokens[0],
					job->cmds[i].tokens, job->msh->envp);
			exit(job_clear(job, 0) | msh_clear(job->msh, errno));
		}
		close_pipe(&job->rd_pipe, job->pp + 1);
		job->rd_pipe = job->pp[0];
	}
	close_pipe(job->pp, job->pp + 1);
	close_fd(&job->rd_pipe);
	return (0);
}

int	job_executor(t_job *job)
{
	int	i;

	if (!job)
		return (report_missing_input(__FUNCTION__));
	if (job->nb_cmds == 1 && job->cmds[0].bltin_func)
	{
		if (job->cmds[0].doa)
			return (0);
//		printf("Setting up builtin redirection \n");
		printf("\nJob output : \n---------------------\n");
		setup_child_redirections(&job->cmds[0]);
		if (job->cmds[0].bltin_func(job, &job->cmds[0]) == BUILTIN_FAILED)
			return (report_builtin_failure(__FUNCTION__));
		reset_standard_io(&job->cmds[0]);
		printf("---------------------\nCommand output over.\n");
		return (0);
	}
	printf("job exec : forking\n");
	printf("\nJob output : \n---------------------\n");
	if (fork_child_processes(job) < 0)
		return (-1);
	job->msh->exec_status = EXEC_MODE;
	i = -1;
//	printf("Waitpid child processes\n");
	while (++i < job->nb_cmds)
		waitpid(job->cmds[i].pid, &job->msh->exit_status, 0);
	printf("---------------------\nJob output over.\n");
	printf("\n\njob exec : forking DONE\n");
	return (0);
}
