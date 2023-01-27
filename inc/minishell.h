/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 01:39:11 by iamongeo          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/01/27 08:22:20 by iamongeo         ###   ########.fr       */
=======
/*   Updated: 2023/01/27 07:05:58 by iamongeo         ###   ########.fr       */
>>>>>>> now intercepting builtins by checking the equality of the first token in cmd->tokens with every builtin names, then if match found set cmd->bltin_func = to the correct function ptr msh_builtin_*. All commands are validated before executions and there name swaped with the full path to their location in environment. Else command not found err msg is printed before exec.
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
# include <string.h>
# include <errno.h>
# include "readline.h"
# include "history.h"
# include "libft.h"

//# include <curses.h>
//# include <term.h>

# define READLINE_PROMPT "minishell> "
# define _META_CHARS "<>|&*"
# define MAX_CMDS 256
# define HDOC_SIZE 0xffffff
//# define E_REDIRECT_FAILED SIZE_MAX

typedef struct s_job			t_job;
typedef struct s_minishell_data	t_msh;
typedef int (* t_bltin)(t_msh *msh, t_cmd *cmd);

typedef struct s_command
{
	t_job	*job;	// reference to the job struct the cmd spawns from. Init in setup_all_cmds()
	pid_t	pid;
	char	**tokens;
	t_bltin	bltin_func;
	int		redir_in;
	int		redir_out;
	int		doa;
}	t_cmd;

// sc = substitution char.
typedef struct s_job
{
	t_msh	*msh;
	char	sc;
	char	*parsed;
	char	**pipe_split;
	t_cmd	cmds[MAX_CMDS];
//	char	**cmds[MAX_CMDS];
//	pid_t	pids[MAX_CMDS];
//	int		redir_in[MAX_CMDS];
//	int		redir_out[MAX_CMDS];
//	int		doa;
	int		heredoc_id;
	int		nb_cmds;
	int		pp[2];
	int		rd_pipe;
}	t_job;

//exec_status = interactive mode/execution mode. Used for signals.
typedef struct s_minishell_data
{
	t_job	job;
	char	**envp;
	char	**paths;
	char	*rawline;
	int		exit_status;
	int		shell_exit_status;
	int		request_exit;
	int		exec_status;
	char	nbr_buff[24];
//	int		stdin_fd;
//	int		stdout_fd;
//	pid_t	job_pid;
}	t_msh;


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
//	E_JOB_MNG_FAILED,
	E_REDIRECT_FAILED = -2
};

enum	e_builtin_status
{
	BUILTIN_FOUND = 0,
	BUILTIN_FAILED = -1,
	BUILTIN_NOT_FOUND = -2
};

// JOB MANAGER
int		job_manager(t_msh *msh);
int		job_executor(t_job *job);
int		validate_synax(char *line);
int		intercept_builtin_call(t_job *job, t_cmd *cmd, int *builtin_status);
int		parse_exec_cmd(t_job *job, int idx);//t_msh *msh, char *cmd);
int		init_pipe(int pp[2], int *rd_pipe, int i, int nb_cmds);
int		close_pipe(int *rd_pipe, int *wr_pipe);
int		close_fd(int *fd);
int		setup_all_cmds(t_job *job);
//int		apply_all_redirections(t_job *job);

// REDIRECTION HANDLERS
int		redirect_infile(t_cmd *cmd, char **tks_p);
int		redirect_outfile(t_cmd *cmd, char **tks_p, int add_mode);
int		get_heredoc_input(t_cmd *cmd, char **token_p, int *heredoc_id);
char	*gen_tempname(char *tempfile, int heredoc_id);

// PARSING UTILS
int		validate_syntax(char *line);
int		is_meta_char(char *c, int *mlen);
int		contains_meta_char(char *str);
int		spaceout_meta_chars(char *str, char **ret);
int		msh_substitute_env_vars(t_msh *msh, char *line, char **ret);
int		split_cmd_on_pipes(char *cmd, char ***tab_p);
char	**tokenize(t_job *job, char *cmd);
char	*skip_valid_envp_var_chars(char *var);

// BUILTINS
int		msh_builtin_echo(t_msh *msh, t_cmd *cmd);//char *cmd);
int		msh_builtin_cd(t_msh *msh, t_cmd *cmd);
int		msh_builtin_env(t_msh *msh t_cmd *cmd);
int		msh_builtin_pwd(t_msh *msh, t_cmd *cmd);
int		msh_builtin_export(t_msh *msh, t_cmd *cmd);//, char *var, char *value);
int		msh_builtin_unset(t_msh *msh, t_cmd *cmd);
int		msh_builtin_exit(t_msh *msh, t_cmd *cmd);//char *var);

// ENVIRONMENT VARIABLES UTILS
int		msh_envp_add_entry(t_msh *msh, char *var, char *value);
int		msh_envp_remove_entry(t_msh *msh, char *var);
int		msh_envp_copy(char **envp, char ***ret);
char	*msh_getenv(t_msh *msh, char *var);

// CLEAR FUNCTIONS
int		msh_clear(t_msh *msh, int exit_code);
int		job_clear(t_job *job, int exit_code);

// ERROR HANDLING
int		report_missing_input(const char *fn);
int		report_malloc_err(const char *fn);
int		report_fork_err(const char *fn);
int		report_jm_mlc_err(const char *fn);
int		report_pipe_err(const char *fn);
int		report_parsing_error(const char *fn, char *meta_c, int len);
int		report_builtin_failure(const char *fn);
int		report_max_nb_cmds_exceeded(t_job *job);
int		report_cmd_not_found(char *cmd, int *doa_p);

// SIGNALS
void	handlers_control(t_msh *msh);
void	sig_handler(int signum, siginfo_t *info, void *context);

#endif
