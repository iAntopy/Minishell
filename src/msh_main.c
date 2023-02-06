/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 19:16:03 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/06 02:46:36 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	msh_clear_pipelines(t_msh *msh, int return_value)
{
	int	i;

	i = 0;
	while (i < msh->nb_plns)
	{
		ft_free_p((void **)&msh->pipelines[i]);
		msh->pl_meta_bools[i] = 0;
		i++;
	}
	msh->nb_plns = 0;
	return (return_value);
}

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
	return (msh_clear_pipelines(msh, exit_code));
}

static int	msh_init(t_msh *msh, char **envp)
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

static int	msh_pipelines_manager(t_msh *msh)
{
	int	i;
	int	bl;

	if (validate_syntax(msh->rawline, &msh->exit_status) < 0
		|| split_on_bools(msh) < 0)
		return (msh_clear_pipelines(msh, -1));
	i = -1;
	while (++i < msh->nb_plns && msh->exit_status != EXIT_SIGINT)
	{
		bl = msh->pl_meta_bools[i];
		if (job_manager(msh, msh->pipelines[i]) < 0)
			break ;
		if (bl && ((bl == BOOL_AND && msh->exit_status != EXIT_SUCCESS)
			|| (bl == BOOL_OR && msh->exit_status == EXIT_SUCCESS)))
			break ;
	}
	return (msh_clear_pipelines(msh, 0));
}

int	main(int argc, char **argv, char **envp)
{
	t_msh	*msh;

	(void)argc;
	(void)argv;
	msh = get_msh();
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
		if (msh->rawline[0] != '\0' && *skip_spaces(&msh->rawline, 0, 0) != '\0')
		{
			add_history(msh->rawline);
			msh_pipelines_manager(msh);
		}
		ft_free_p((void **)&msh->rawline);
	}
	return (msh_clear(msh, msh->shell_exit_status));
}
