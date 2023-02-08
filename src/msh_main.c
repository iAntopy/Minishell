/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 19:16:03 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/07 23:54:57 by iamongeo         ###   ########.fr       */
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
	close_fd(&msh->read_script_fd);
	return (msh_clear_pipelines(msh, exit_code));
}

int	read_script(t_msh *msh, char *filename)
{
	msh->read_script_fd = open(filename, O_RDONLY);
	msh->rawline = get_next_line(msh->read_script_fd);
	while (msh->rawline)
	{
		msh_pipelines_manager(msh);
		ft_free_p((void **)&msh->rawline);
		msh->rawline = get_next_line(msh->read_script_fd);
	}
	ft_free_p((void **)&msh->rawline);
	close_fd(&msh->read_script_fd);
	return (0);
}

static int	msh_init(t_msh *msh, int argc, char **argv, char **envp)
{
	if (!msh || !envp)
		return (-1);
	msh->paths = get_env_paths(envp);
	if (!msh->paths || msh_envp_copy(envp, &msh->envp) < 0)
		return (report_malloc_err());
	msh->exit_status = 0;
	msh->sigint_flag = 0;
	msh->stdin_fd = dup(STDIN_FILENO);
	msh->stdout_fd = dup(STDOUT_FILENO);
	if (argc == 2)
	{
		read_script(msh, argv[1]);
		exit(msh_clear(msh, msh->exit_status));
	}
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
	handlers_control(msh, INTERAC_MODE);
	if (msh_init(msh, argc, argv, envp) < 0)
		return (msh_clear(msh, E_MSH_INIT));
	while (!msh->request_exit)
	{
		if (msh->rawline)
			msh_clear(msh, E_RAWLINE_CLR_ERR);
		msh->rawline = readline(READLINE_PROMPT);
		if (msh->rawline == NULL)
			break ;
		if (msh->rawline[0] != '\0'
			&& *skip_spaces(&msh->rawline, 0, 0) != '\0')
		{
			add_history(msh->rawline);
			msh_pipelines_manager(msh);
		}
		ft_free_p((void **)&msh->rawline);
		handlers_control(msh, INTERAC_MODE);
	}
	return (msh_clear(msh, msh->shell_exit_status));
}
