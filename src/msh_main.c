/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 19:16:03 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/16 18:50:25 by tbeaudoi         ###   ########.fr       */
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
/*
int	msh_parse_rawline(t_msh *msh)
{
	char	*parsed_str;

	parsed_str = ft_strtrim_space(msh->rawline);
	if (!parsed_line)
		return (-1);
	ft_free_p((void **)&msh->rawline);
	msh->rawline = parsed_str;
	
}
*/

int	msh_init(t_msh *msh, char **envp)
{
	if (!msh || !envp)
		return (repport_missing_input(__FUNCTION__));
	msh->paths = get_env_paths(envp);
	if (!msh->paths || msh_envp_copy(envp, &msh->envp) < 0)
		return (repport_malloc_err(__FUNCTION__));
	printf("msh_init : envp cpy ptr : %p\n", msh->envp);
	return (0);
}

/*
int	msh_start_job_manager(t_msh *msh)
{
	int	pid;

	if (!msh)
		return (repport_missing_input(__FUNCTION__));
	pid = fork();
	if (pid < 0)
		return (repport_fork_err(__FUNCTION__));
	else if (pid == 0)
	{
		if (jm_manage_job(msh) < 0)
			exit(msh_clear(msh, E_JOB_MNG_FAILED));
		exit(msh_clear(msh, EXIT_SUCCESS));
	}
	else
		msh->job_pid = pid;
	return (0);
}
*/
/*
void	terminal_infos_tests(void)
{
	int	slot;
	char	buff[2048];
	int	getent_ret;
	int	tnum;
	char	*pc_str;

	slot = ttyslot();
	printf("ttyslot : %d\n", slot);
	printf("isatty : %d\n", isatty(slot));
	printf("ttyname : %s\n", ttyname(slot));
	getent_ret = tgetent(NULL, getenv("TERM"));
	printf("getent ret : %d, buff : %s\n", getent_ret, buff);
	tnum = tgetnum("co");
	printf("nb columns : %d\n", tnum);
	pc_str = NULL;
	tgetstr("pc", &pc_str);
	if (pc_str)
		printf("padding char : %d\n", *pc_str);
	ft_free_p((void **)&pc_str);
	tnum = tgetnum("dC");
//	printf("baud rate where padding needed : %d\n", tnum);
	printf("msec for carridge return : %d\n", tnum);
}
*/

int	main(int argc, char **argv, char **envp)
{
	t_msh	msh;

	(void)argc;
	(void)argv;
	ft_memclear(&msh, sizeof(t_msh));
	if (msh_init(&msh, envp) < 0)
		return (msh_clear(&msh, E_MSH_INIT));
	msh.exec_status = INTERRACTIVE_MODE;

//	terminal_infos_tests();
	while (!msh.request_exit)
	{
		msh.exec_status = INTERRACTIVE_MODE;
		// reset exec_status to interractive affect program exec
		handler_handler(&msh);
		if (msh.rawline)
			msh_clear(&msh, E_RAWLINE_CLR_ERR);
		msh.rawline = readline(READLINE_PROMPT);
		if (msh.rawline == NULL)
			break ;
//		printf("main : rawline received %p : %s\n", msh.rawline, msh.rawline);
		if (!msh.rawline || msh.rawline[0] == '\0' || msh.rawline[0] == '\n')
			continue ;
		if (msh.rawline[0])// && msh.rawline[0] != '\n')
			add_history(msh.rawline);
		job_manager(&msh);
		printf("main : post manage job call\n");
//		msh_start_job_manager(&msh);
		///
		/// set sigactions 
		/// and waitpid msh.job_pid
		/// then set msh.job_pid to 0
		///
		
		printf("main : post waitpid exit status : %d\n", WEXITSTATUS(msh.exit_status));
		printf("main : rawline received %p : %s\n", msh.rawline, msh.rawline);
		ft_free_p((void **)&msh.rawline);
	}
	return (msh_clear(&msh, msh.shell_exit_status));
}
