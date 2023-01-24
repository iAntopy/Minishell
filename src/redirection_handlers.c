/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handlers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 20:33:23 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/24 04:21:39 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

static int	repport_file_error(char *filename, char **redir_fn)
{
	ft_eprintf("minishell error :: %s : %s\n", strerror(errno), filename);
	*redir_fn = E_REDIRECT_FAILED;
	return (errno);
}

int	redirect_infile(char **token_p, char **redir_fn)
{
	char	*filename;
	int		fd;

	if (!token_p)
		return (-1);
	close_fd(redir_fn);
	filename = *token + 1;
	if (access(filename, F_OK | R_OK) < 0)
		return (repport_file_error(filename, redir_fn));
//	fd = open(filename, O_RDONLY);
//	if (fd < 0)
//		return (repport_file_error(filename, redir_fn));
//	*redir_fn = fd;
	ft_free_p((void **)token_p);
	ft_memmove(token_p, token_p + 1, strtab_len(token + 1) * sizeof(char *));
	return (1);
}

int	redirect_outfile(char **token_p, int *redir_fn, int add_mode)
{
	char	*filename;
	int		fd;

	if (!token_p)
		return (-1);
	filename = *token + 1;
	close_fd(redir_fn);
	if (access(filename, F_OK | W_OK) < 0)
		return (repport_file_error(filename, redir_fn));
//	fd = open(filename, O_CREAT | O_WRONLY | add_mode, 0644);
//	if (fd < 0)
//		return (repport_file_error(filename, redir_fn));
//	*redir_fn = fd
	ft_free_p((void **)token_p);
	ft_memmove(token_p, token_p + 1, strtab_len(token + 1) * sizeof(char *));
	return (1);
}

static int	read_exception(void)
{
	ft_eprintf("minishell :: heredoc tmp file open failed.");
	return (-1);
}

char	*generate_tempname(char *tempfile, int *id)
{
	int			nb_len;

	ft_strlcpy(tempfile, "tmp/.heredoc", PATH_MAX);
	ft_putnbr_buff(tempfile + 12, *id);
	ft_strlcat(".tmp", PATH_MAX);
	*id += 1;
	return (tempfile);
}

int	find_heredoc_env_subst_size(t_msh *msh, char *rl, char *var_buff)
{
	int		i;
	int		extra_size;
	char	*env_val;

	extra_size = 0;
	i = -1;
	while (rl[++i])
	{
		if (rl[i] == '$' && !ft_isspace(rl[i + 1]))
		{
			j = i;
			while (rl[++j] && (ft_isalnum(rl[j]) || rl[j] == '_'))
				*var_buff[j - i - 1] = rl[j];
			*var_buff[j - i - 1] = '\0';
			env_val = msh_getenv(msh, *var_buff);
			extra_size += ft_strlen(env_val);
			i = j;
		}
	}
	return (i + extra_size);
}

char	*ft_strchr_p(char *str, char c, char **ptr)
{
	*ptr = ft_strchr(str, c);
	if (*ptr)
		return (ptr)
	return (NULL);
}

int	substitute_heredoc_env_vars(t_msh *msh, char *rl, char **ret, char *var_buff)
{
	int		i;
	char	*rp;
	char	*np;
	char	*env_val;
	char	*var_p;

	if (!ft_malloc_p(find_heredoc_env_subst_size(msh, rl, var_buff), (void **)ret))
		return (report_malloc_err(__FUNCTION__));
	rp = rl;
	np = *ret;
	while (ft_strchr_p(rp, '$', &var_p) && ft_memcpy(np, rp, var_p - rp))
	{
		np += var_p - rp;
		rp = var_p;
		if (*(rp + 1) == '?')
		{
			np += ft_putnbr_buff(np, msh->exit_code);
			rp += 2;
		}
		else if (!ft_isspace(*(rp + 1)))
		{
			if (!(*(rp + 1) == '_' || ft_isalpha(*(rp + 1))))
				rp += 2;
			else
			{
				i = 0;
				while (*(++rp) && (ft_isalnum(*rp) || *rp == '_'))
					var_buff[i++] = *rp;
				var_buff[i] = '\0';
				env_val = msh_getenv(msh, var_buff);
				np = ft_strcpy(np, env_val) + ft_strlen(env_val);
			}
		}
	}
	ft_strcpy(np, rp);
	return (0);
}

int	heredoc_readline_env_var_convert(t_msh *msh, char **ret_line)
{
	char	*rl;
	char	var_buff[4096];
	int		ret_status;

	rl = readline("> ");
	if (!rl)
		return (read_exception());
	ret_status = substitute_heredoc_env_vars(msh, rl, ret_line, var_buff);
	ft_free_p((void **)&rl);
	return (ret_status);
}

int	get_heredoc_input(t_msh *msh, char **token_p, int *redir_fn, int *id)
{
	char	*limiter;
	int		fd;
//	char	buff[HDOC_SIZE];
	char	*rl;
	char	tempname[PATH_MAX];
	ssize_t	nchrs;

	close_fd(*redir_fn);
	fd = open(generate_tempname(tempname, id), O_CREAT | O_TRUNC | O_RDWR, 0644);
	if (fd < 0)
		return (repport_file_error(tempfile, redir_fn));
	limiter = *token_p + 2;
	rl = NULL;
	while (heredoc_readline_env_var_convert(msh, &rl) >= 0 &&
			rl && rl[0] && ft_strcmp(rl, limiter) != 0)
		write(fd, rl, ft_strlen(rl));
//	{
//		rl = readline("> ");
//		if (nchrs < 0)
//			return (read_exception());
//		else if (nchrs == 0
//			|| (nchrs > 0 && ft_strncmp(buff, limiter, nchrs - 1) == 0))
//			break ;
			
//		write(fd, buff, nchrs);
//	}
	ft_free_p((void **)&rl);
	ft_free_p((void **)token_p);
	ft_memmove(token_p, token_p + 1, strtab_len(token + 1) * sizeof(char *));
	return (1);
}
