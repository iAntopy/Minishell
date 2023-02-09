/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handlers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 20:01:38 by tbeaudoi          #+#    #+#             */
/*   Updated: 2023/02/09 14:49:15 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Waiting for exec to continue
// void	sig_handler(int signum, siginfo_t *info, void *context)
// {
// 	(void)context;
// 	(void)info;
// 	if (signum == SIGINT)
// 	{
// 		write(1, "\n", 1);
// 		rl_on_new_line();
// 		rl_replace_line("", 0);
// 		rl_redisplay();
// 	}
// }

// void	sig_handler_child(int signum, siginfo_t *info, void *context)
// {
// 	(void)context;
// 	(void)info;
// 	printf("signal pid : %d\n", getpid());
// 	if (signum == SIGQUIT)
// 		(void) SIGQUIT;
// 	if (signum == SIGINT)
// 		(void) SIGINT;
// }

// void	sig_handler_heredoc(int signum, siginfo_t *info, void *context)
// {
// 	t_msh	*msh;

// 	(void)info;
// 	(void)context;
// 	msh = get_msh();
// 	if (signum == SIGINT)
// 	{
// 		close_fd(&msh->job.tmp_fd);
// 		kill(msh->hd_pid, SIGQUIT);
// 	}
// 	if (signum == SIGQUIT)
// 		(void) SIGQUIT;
// }

// void	handlers_control(t_msh *msh, int mode)
// {
// 	struct sigaction	sa;

// 	(void)msh;
// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = SA_RESTART;
// 	if (mode == INTERAC_MODE)
// 	{
// 		sa.sa_sigaction = &sig_handler;
// 		sigaction(SIGINT, &sa, NULL);
// 	}
// 	if (mode == EXEC_MODE)
// 	{
// 		sa.sa_sigaction = &sig_handler_child;
// 		sigaction(SIGINT, &sa, NULL);
// 		sigaction(SIGQUIT, &sa, NULL);
// 	}
// 	if (mode == HEREDOC_MODE)
// 	{
// 		sa.sa_sigaction = &sig_handler_heredoc;
// 		sigaction(SIGINT, &sa, NULL);
// 		sigaction(SIGQUIT, &sa, NULL);
// 	}
// }

void	sig_heredoc(int signum)
{
	t_msh	*msh;

	if (signum == SIGINT || signum == SIGQUIT)
	{
		msh = get_msh();
		job_clear(&msh->job, 0);
		msh_clear(msh, 0);
		if (signum == SIGINT)
			exit(EXIT_SIGINT);
		else
			exit(EXIT_SIGQUIT);
	}
}

void	sig_int(int signum)
{
	t_msh	*msh;

	(void)signum;
	msh = get_msh();
	close_fd(&msh->job.tmp_fd);
	kill(msh->hd_pid, 0);
	msh->sigint_flag = 1;
	write(1, "\n", 1);
	rl_on_new_line();
}

void	sig_interac(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sig_quit(int signum)
{
	t_msh	*msh;

	(void)signum;
	msh = get_msh();
	close_fd(&msh->job.tmp_fd);
	kill(msh->hd_pid, 0);
	msh->sigquit_flag = 1;
	write(1, "\n", 1);
	rl_on_new_line();
}

void	handlers_control(t_msh *msh, int mode)
{
	(void)msh;
	if (mode == INTERAC_MODE)
	{
		signal(SIGINT, sig_interac);
		signal(SIGQUIT, SIG_IGN);
	}
	if (mode == EXEC_MODE)
	{
		signal(SIGINT, sig_int);
		signal(SIGQUIT, sig_quit);
	}
	if (mode == HEREDOC_MODE)
	{
		signal(SIGINT, sig_heredoc);
		signal(SIGQUIT, sig_heredoc);
	}
}
