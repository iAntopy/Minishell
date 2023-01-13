/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handlers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 20:01:38 by tbeaudoi          #+#    #+#             */
/*   Updated: 2023/01/12 21:01:17 by tbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler1(int signum, siginfo_t *info, void *context)
{
	(void)signum;
	(void)context;
	(void)info;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// void	sig_handler2(int sig)
// {
// 	printf("mange ma raie");
// }

void	handler_handler(void)
{
	struct sigaction	sa;

	sa.sa_sigaction = &sig_handler1;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	// sigaction(SIGUSR2, &sa, NULL);
}
