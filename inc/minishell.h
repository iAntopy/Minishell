/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 01:39:11 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/17 12:37:57 by tbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <sys/wait.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <errno.h>
# include "readline.h"
# include "history.h"
# include "libft.h"

//# include <curses.h>
//# include <term.h>


# define READLINE_PROMPT "minishell> "
# define _META_CHARS "<>|&*"
# define MAX_CMDS 256

//exec_status = interactive mode/execution mode. Used for signals.
typedef struct s_minishell_data
{
	char	**envp;
	char	**paths;
	char	*rawline;
	int		exit_status;
	int		shell_exit_status;
	int		request_exit;
	int		exec_status;
//	pid_t	job_pid;
}	t_msh;


// sc = substitution char.
typedef struct s_job
{
	t_msh	*msh;
	char	sc;
	char	*parsed;
	char	**pipe_split;
	int	nb_cmds;
	pid_t	pids[MAX_CMDS];
	int	pp[2];
	int	rd_pipe;

}	t_job;

enum	e_exec_status
{
	INTERAC_MODE = 0,
	EXEC_MODE = 1
};

enum	e_err_codes
{
	E_MSH_INIT = 1,
	E_MALLOC,
	E_RAWLINE_CLR_ERR,
	E_JOB_MNG_FAILED
};

enum	e_builtin_status
{
	BUILTIN_FOUND = 0,
	BUILTIN_FAILED = -1,
	BUILTIN_NOT_FOUND = -2
};

// JOB MANAGER
int	job_manager(t_msh *msh);
int	job_executor(t_job *job);
int	intercept_builtin_call(t_job *job, char *cmd, int *builtin_status);
int	parse_exec_cmd(t_job *job, int idx);//t_msh *msh, char *cmd);
int	init_pipe(int pp[2], int *rd_pipe, int i, int nb_cmds);
int	close_pipe(int *rd_pipe, int *wr_pipe);


// PARSING UTILS
int		contains_meta_char(char *str);
int		spaceout_meta_chars(char *str, char **ret);
int		msh_substitute_env_vars(t_msh *msh, char *line, char **ret);
int		split_cmd_on_pipes(char *cmd, char ***tab_p);

// BUILTINS
int		msh_builtin_echo(t_msh *msh, char *cmd);
int		msh_builtin_cd(t_msh *msh, char *cmd);
int		msh_builtin_env(t_msh *msh);
int		msh_builtin_pwd(t_msh *msh);
int		msh_builtin_export(t_msh *msh, char *cmd);//, char *var, char *value);
int		msh_builtin_unset(t_msh *msh, char *cmd);
int		msh_builtin_exit(t_msh *msh, char *cmd);//char *var);

// ENVIRONMENT VARIABLES UTILS
int		msh_envp_add_entry(t_msh *msh, char *var, char *value);
int		msh_envp_remove_entry(t_msh *msh, char *var);
int		msh_envp_copy(char **envp, char ***ret);
char	*msh_getenv(t_msh *msh, char *var);

// CLEAR FUNCTIONS
int		msh_clear(t_msh *msh, int exit_code);
int		job_clear(t_job *job, int exit_code);

// ERROR HANDLING
int		repport_missing_input(const char *fn);
int		repport_malloc_err(const char *fn);
int		repport_fork_err(const char *fn);
int		repport_jm_mlc_err(const char *fn);
int		repport_pipe_err(const char *fn);
int		repport_parsing_error(const char *fn, char *meta_c, int len);
int		repport_builtin_failure(const char *fn);

// SIGNALS
void	handlers_control(t_msh *msh);
void	sig_handler(int signum, siginfo_t *info, void *context);

#endif
