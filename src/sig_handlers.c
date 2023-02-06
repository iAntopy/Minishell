/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handlers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 20:01:38 by tbeaudoi          #+#    #+#             */
/*   Updated: 2023/02/06 12:28:19 by tbeaudoi         ###   ########.fr       */
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

	if (signum == SIGQUIT)
		(void) SIGQUIT;
	if (signum == SIGINT)
		(void) SIGINT;
}

void	sig_handler_heredoc(int signum, siginfo_t *info, void *context)
{
	t_msh	*msh;

	msh = get_msh();
	if (signum == SIGINT)
	{
		close_fd(&msh->job.tmp_fd);
		kill(msh->hd_pid, SIGQUIT);
	}
	if (signum == SIGQUIT)
		(void) SIGQUIT;
}

void	handlers_control(t_msh *msh, int mode)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	(void)msh;
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
