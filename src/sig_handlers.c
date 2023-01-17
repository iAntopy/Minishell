/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handlers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 20:01:38 by tbeaudoi          #+#    #+#             */
/*   Updated: 2023/01/16 19:58:53 by tbeaudoi         ###   ########.fr       */
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

void	handler_handler(t_msh *msh)
{
	struct sigaction	sa;

	sa.sa_flags = SA_SIGINFO;
	if (msh->exec_status == INTERRACTIVE_MODE)
	{
		sa.sa_sigaction = &sig_handler;
		sigaction(SIGINT, &sa, NULL);
	}
}
