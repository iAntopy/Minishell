/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 19:16:03 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/27 08:51:49 by iamongeo         ###   ########.fr       */
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
	return (exit_code);
}

int	msh_init(t_msh *msh, char **envp)
{
	if (!msh || !envp)
		return (report_missing_input(__FUNCTION__));
	msh->paths = get_env_paths(envp);
	if (!msh->paths || msh_envp_copy(envp, &msh->envp) < 0)
		return (report_malloc_err(__FUNCTION__));
	printf("msh_init : envp cpy ptr : %p\n", msh->envp);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_msh	msh;

	(void)argc;
	(void)argv;
	ft_memclear(&msh, sizeof(t_msh));
	if (msh_init(&msh, envp) < 0)
		return (msh_clear(&msh, E_MSH_INIT));
	msh.exec_status = INTERAC_MODE;
	while (!msh.request_exit)
	{
		msh.exec_status = INTERAC_MODE;
		handlers_control(&msh);
		if (msh.rawline)
			msh_clear(&msh, E_RAWLINE_CLR_ERR);
		msh.rawline = readline(READLINE_PROMPT);
		if (msh.rawline == NULL)
			break ;
		if (!msh.rawline || msh.rawline[0] == '\0' || msh.rawline[0] == '\n')
			continue ;
		if (msh.rawline[0])
			add_history(msh.rawline);
		printf("msh : entering job manager\n");
		job_manager(&msh);
		ft_free_p((void **)&msh.rawline);
	}
	return (msh_clear(&msh, msh.shell_exit_status));
}

		// printf("Return to main from job manager()\n");
		// printf("main : post manage job call\n");
		// printf("main : post execution exit status : %d\n", WEXITSTATUS(msh.exit_status));
		// printf("main : rawline received %p : %s\n", msh.rawline, msh.rawline);
