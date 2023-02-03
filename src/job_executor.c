/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 00:48:45 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/02 20:30:35 by iamongeo         ###   ########.fr       */
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
		printf("REDIR IN \n");
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

static void	execute_child_task(t_job *job, t_cmd *cmd)
{
	int	sts;

	if (cmd->doa)
		exit(job_clear(job, 0) | msh_clear(job->msh, job->msh->exit_status));
	setup_child_redirections(cmd);
	sts = EXIT_SUCCESS;
	if (cmd->builtin)
		sts = cmd->builtin(job, cmd);
	else
		execve(cmd->tokens[0], cmd->tokens, job->msh->envp);
	exit(sts | WEXITSTATUS(errno) | job_clear(job, 0) | msh_clear(job->msh, 0));
}

static int	fork_child_processes(t_job *job)
{
	int	i;

	i = -1;
	while (++i < job->nb_cmds)
	{
		if (init_pipe(job->pp, &job->rd_pipe, i, job->nb_cmds) < 0)
			return (job_executor_force_exit(job, &job->rd_pipe));
		job->cmds[i].pid = fork();
		if (job->cmds[i].pid < 0)
			return (job_executor_force_exit(job, &job->rd_pipe));
		else if (job->cmds[i].pid == 0)
			execute_child_task(job, job->cmds + i);
		close_pipe(&job->rd_pipe, job->pp + 1);
		job->rd_pipe = job->pp[0];
	}
	close_fd(&job->rd_pipe);
	return (0);
}

int	job_executor(t_job *job)
{
	int	i;

	if (!job)
		return (report_missing_input(__FUNCTION__));
	if (job->nb_cmds == 1 && job->cmds[0].builtin)
	{
		if (!job->cmds[0].doa)
			setup_child_redirections(&job->cmds[0]);
		if (!job->cmds[0].doa && job->cmds[0].builtin(job, &job->cmds[0]) != 0)
			return (report_builtin_failure(__FUNCTION__));
		if (!job->cmds[0].doa && job->cmds[0].redir_in >= 3)
			dup2(job->msh->stdin_fd, STDIN_FILENO);
		if (!job->cmds[0].doa && job->cmds[0].redir_out >= 3)
			dup2(job->msh->stdout_fd, STDOUT_FILENO);
		return (0);
	}
	if (fork_child_processes(job) < 0)
		return (-1);
	job->msh->exec_status = EXEC_MODE;
	i = -1;
	while (++i < job->nb_cmds)
		waitpid(job->cmds[i].pid, &job->msh->exit_status, 0);
	job->msh->exec_status = INTERAC_MODE;
	job->msh->exit_status = WEXITSTATUS(job->msh->exit_status);
	return (0);
}
