/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handlers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 20:01:38 by tbeaudoi          #+#    #+#             */
/*   Updated: 2023/02/05 01:17:12 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Waiting for exec to continue

void	sig_handler(int signum, siginfo_t *info, void *context)
{
	(void)context;
	(void)info;
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	sig_handler_child(int signum, siginfo_t *info, void *context)
{
	(void)context;
	(void)info;
	printf("signal pid : %d\n", getpid());
	if (signum == SIGQUIT)
		(void) SIGQUIT;
	if (signum == SIGINT)
	{
		(void) SIGINT;
		// kill(getpid(), SIGQUIT);
	}
}

void	sig_handler_heredoc(int signum, siginfo_t *info, void *context)
{
	t_msh	*msh;

	(void)info;
	(void)context;
	msh = get_msh();
	if (signum == SIGINT)
	{
		close_fd(&msh->job.tmp_fd);
		//write(1, "\n", 1);
		//rl_on_new_line();
		kill(msh->hd_pid, SIGQUIT);
//		exit(job_clear(&msh->job, 0) | msh_clear(msh, 3));
	}
	if (signum == SIGQUIT)
		(void) SIGQUIT;
}

void	handlers_control(t_msh *msh, int mode)
{
	struct sigaction	sa;

	//sa.sa_mask = SA_SIGINFO;
	(void)msh;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (mode == INTERAC_MODE)
	{
		sa.sa_sigaction = &sig_handler;
		sigaction(SIGINT, &sa, NULL);
	}
	if (mode == EXEC_MODE)
	{
		sa.sa_sigaction = &sig_handler_child;
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGQUIT, &sa, NULL);
	}
	if (mode == HEREDOC_MODE)
	{
		sa.sa_sigaction = &sig_handler_heredoc;
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGQUIT, &sa, NULL);
	}
}
