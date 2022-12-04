/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 01:39:11 by iamongeo          #+#    #+#             */
/*   Updated: 2022/12/04 07:32:17 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
//# include <term.h>
//# include <curses.h>

# include "libft.h"

# ifndef rl_clear_history
#  define rl_clear_history clear_history
# endif

# define READLINE_PROMPT "minishell> "

typedef struct	s_minishell_data
{
	char 	**envp;
	char	**paths;
	char	*rawline;
	int		pipe_exit_status;

}	t_msh;

enum	e_err_codes
{
	E_MSH_INIT = 1,
	E_MALLOC,
	E_RAWLINE_CLR
};

// builtins
int	msh_builtin_echo(t_msh *msh, char **argv);
//int	msh_builtin_cd(t_msh *msh, char *path);
int	msh_builtin_env(char **envp);
int	msh_builtin_pwd(void);

// ERROR HANDLING
int	repport_missing_input(const char *fn);
int	repport_malloc_err(const char *fn);

#endif
