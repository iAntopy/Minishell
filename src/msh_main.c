/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 19:16:03 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/05 01:12:24 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	msh_clear(t_msh *msh, int exit_code)
{
	if (msh->paths)
		strtab_clear(&msh->paths);
	if (msh->envp)
		strtab_clear(&msh->envp);
	if (msh->rawline)
		ft_free_p((void **)&msh->rawline);
	rl_clear_history();
	close_fd(&msh->stdin_fd);
	close_fd(&msh->stdout_fd);
	return (exit_code);
}

int	msh_init(t_msh *msh, char **envp)
{
	if (!msh || !envp)
		return (report_missing_input(__FUNCTION__));
	msh->paths = get_env_paths(envp);
	if (!msh->paths || msh_envp_copy(envp, &msh->envp) < 0)
		return (report_malloc_err(__FUNCTION__));
	msh->exit_status = 0;
	msh->stdin_fd = dup(STDIN_FILENO);
	msh->stdout_fd = dup(STDOUT_FILENO);
	return (0);
}

t_msh	*get_msh(void)
{
	static t_msh	msh;

	return (&msh);
}

int	main(int argc, char **argv, char **envp)
{
	t_msh	*msh;

	(void)argc;
	(void)argv;
	msh = get_msh();
//	ft_memclear(&msh, sizeof(t_msh));
	if (msh_init(msh, envp) < 0)
		return (msh_clear(msh, E_MSH_INIT));
	while (!msh->request_exit)
	{
		handlers_control(msh, INTERAC_MODE);
		if (msh->rawline)
			msh_clear(msh, E_RAWLINE_CLR_ERR);
		msh->rawline = readline(READLINE_PROMPT);
		if (msh->rawline == NULL)
			break ;
		if (msh->rawline[0] != '\0')
		{
			add_history(msh->rawline);
			job_manager(msh);
		}
		ft_free_p((void **)&msh->rawline);
	}
	return (msh_clear(msh, msh->shell_exit_status));
}
