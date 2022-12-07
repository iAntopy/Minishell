/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 01:39:11 by iamongeo          #+#    #+#             */
/*   Updated: 2022/12/05 19:26:05 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <sys/wait.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "libft.h"

//# include <curses.h>
//# include <term.h>

# ifndef rl_clear_history
#  define rl_clear_history clear_history
# endif

# define READLINE_PROMPT "minishell> "
#define _META_CHARS "<>|&*"

typedef struct	s_minishell_data
{
	char 	**envp;
	char	**paths;
	char	*rawline;
	int		exit_status;
	pid_t	job_pid;

}	t_msh;

enum	e_err_codes
{
	E_MSH_INIT = 1,
	E_MALLOC,
	E_RAWLINE_CLR_ERR,
	E_JOB_MNG_FAILED
};

// JOB MANAGER
int	jm_manage_job(t_msh *msh);

// PARSING UTILS
int	contains_meta_char(char *str);
int	spaceout_meta_chars(char *str, char **ret);
int	msh_substitute_env_vars(t_msh *msh, char *line, char **ret);

// BUILTINS
int	msh_builtin_echo(t_msh *msh, char **argv);
//int	msh_builtin_cd(t_msh *msh, char *path);
int	msh_builtin_env(char **envp);
int	msh_builtin_pwd(void);
int	msh_builtin_export(t_msh *msh, char *var, char *value);
int	msh_builtin_unset(t_msh *msh, char *var);

// ENVIRONMENT VARIABLES UTILS
int	msh_envp_add_entry(t_msh *msh, char *var, char *value);
int	msh_envp_remove_entry(t_msh *msh, char *var);
int	msh_envp_copy(char **envp, char ***ret);
char	*msh_getenv(t_msh *msh, char *var);

// ERROR HANDLING
int	repport_missing_input(const char *fn);
int	repport_malloc_err(const char *fn);
int	repport_fork_err(const char *fn);
int	repport_jm_mlc_err(const char *fn);

#endif
