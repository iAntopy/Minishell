/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 01:39:11 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/05 01:09:40 by iamongeo         ###   ########.fr       */
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

# define READLINE_PROMPT "minishell> "
# define _META_CHARS "<>|&*"
# define MAX_CMDS 1024
# define HDOC_SIZE 0xffffff
//# define E_REDIRECT_FAILED SIZE_MAX

typedef struct s_job			t_job;
typedef struct s_minishell_data	t_msh;
typedef struct s_command		t_cmd;
typedef int						(*t_bltin)(t_job *job, t_cmd *cmd);

typedef struct s_command
{
	t_job	*job;
	pid_t	pid;
	char	**tokens;
	t_bltin	builtin;
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
	char	*parsed2;
	char	**pipe_split;
	t_cmd	cmds[MAX_CMDS];
	int		nb_cmds;
	int		pp[2];
	int		rd_pipe;
	int		tmp_fd;
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
	int		is_hd_child;
	int		hd_id;
	int		hd_pid;
	char	nbr_buff[24];
	int		nbr_buff_len;
	int		stdin_fd;
	int		stdout_fd;
}	t_msh;

enum	e_exec_status
{
	INTERAC_MODE = 0,
	EXEC_MODE = 1,
	HEREDOC_MODE = 2
};

enum	e_err_codes
{
	E_MSH_INIT = 1,
	E_MALLOC,
	E_RAWLINE_CLR_ERR,
	E_REDIRECT_FAILED = -2
};

enum	e_builtin_status
{
	BUILTIN_FOUND = 0,
	BUILTIN_FAILED = -1,
	BUILTIN_NOT_FOUND = -2
};

// DEBUG FUNCTIONS (DELETE)
void	print_all_cmds(t_job *job);// in tokenizer_redirector.c

// JOB MANAGER
t_msh	*get_msh(void);
int		job_manager(t_msh *msh);
int		job_executor(t_job *job);
int		validate_syntax(char *line, int *exit_status);
//int		validate_meta_char_syntax(char *line);
//int		validate_syntax_beggining_or_ending_pipes(char *line);
int		intercept_builtin_call(t_cmd *cmd);
int		init_pipe(int pp[2], int *rd_pipe, int i, int nb_cmds);
int		close_pipe(int *rd_pipe, int *wr_pipe);
int		close_fd(int *fd);
int		setup_cmds(t_job *job);

// REDIRECTION HANDLERS
int		redirect_infile(t_cmd *cmd, char **tks_p);
int		redirect_outfile(t_cmd *cmd, char **tks_p, int add_mode);
int		get_heredoc_input(t_msh *msh, t_cmd *cmd, char **token_p);
char	*gen_tempname(char *tempfile, int heredoc_id);

// PARSING UTILS
int		is_meta_char(char *c, int *mlen);
int		contains_meta_char(char *str);
int		spaceout_meta_chars(t_job *job);
int		is_valid_env_char(char c, int is_first);
int		substitute_env_vars(t_msh *msh, char *str, char **ret);
int		substitute_env_vars_heredoc(t_msh *msh, char *str, char **ret);
int		split_on_pipes(t_job *job);
//char	**tokenize(t_job *job, char *cmd);
char	*skip_valid_envp_var_chars(char *var);
int		strip_quotes(char *str);

// BUILTINS
int		msh_builtin_echo(t_job *job, t_cmd *cmd);
int		msh_builtin_cd(t_job *job, t_cmd *cmd);
int		msh_builtin_env(t_job *job, t_cmd *cmd);
int		msh_builtin_pwd(t_job *job, t_cmd *cmd);
int		msh_builtin_export(t_job *job, t_cmd *cmd);
int		msh_builtin_unset(t_job *job, t_cmd *cmd);
int		msh_builtin_exit(t_job *job, t_cmd *cmd);

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
int		report_file_error(char *filename, t_cmd *cmd);
int		report_malloc_err(const char *fn);
int		report_fork_err(const char *fn);
int		report_jm_mlc_err(const char *fn);
int		report_pipe_err(const char *fn);
int		report_parsing_error(const char *fn, char *meta_c, int len);
int		report_builtin_failure(const char *fn);
int		report_max_nb_cmds_exceeded(t_job *job);
int		report_cmd_not_found(char *cmdname, t_cmd *cmd, int exit_code);
int		report_unclosed_quotes(void);

// SIGNALS
void	handlers_control(t_msh *msh, int mode);
void	sig_handler(int signum, siginfo_t *info, void *context);

#endif
