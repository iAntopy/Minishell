/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handlers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 20:33:23 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/20 23:36:00 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

static int	repport_file_error(char *filename, int *fd_ptr)
{
	ft_eprintf("minishell error :: %s : %s\n", strerror(errno), filename);
	*fd_ptr = E_REDIRECT_FAILED;
	return (errno);
}

int	redirect_infile(char **token_p, int *in_fd)
{
	char	*filename;
	int		fd;

	if (!token_p)
		return (-1);
	close_fd(in_fd);
	filename = *token + 1;
	if (access(filename, F_OK | R_OK) < 0)
		return (repport_file_error(filename, in_fd));
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (repport_file_error(filename, in_fd));
	*in_fd = fd;
	ft_free_p((void **)token_p);
	ft_memmove(token_p, token_p + 1, strtab_len(token + 1) * sizeof(char *));
	return (1);
}

int	redirect_outfile(char **token_p, int *out_fd, int add_mode)
{
	char	*filename;
	int		fd;

	if (!token_p)
		return (-1);
	filename = *token + 1;
	close_fd(out_fd);
	if (access(filename, F_OK | W_OK) < 0)
		return (repport_file_error(filename, out_fd));
	fd = open(filename, O_CREAT | O_WRONLY | add_mode, 0644);
	if (fd < 0)
		return (repport_file_error(filename, out_fd));
	*out_fd = fd
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

int	get_heredoc_input(char **token_p, int *in_fd, int *id)
{
	char	*limiter;
	int		fd;
	char	buff[HDOC_SIZE];
	char	tempname[PATH_MAX];
	ssize_t	nchrs;

	close_fd(*in_fd);
	fd = open(generate_tempname(tempname, id), O_CREAT | O_TRUNC | O_RDWR, 0644);
	if (fd < 0)
		return (repport_file_error(tempfile, in_fd));
	limiter = *token_p + 2;
	while (1)
	{
		write(job->msh->stdout_fd, "> ", 2);
		nchrs = read(job->msh->stdin_fd, buff, HDOC_SIZE);
		if (nchrs < 0)
			return (read_exception());
		else if (nchrs == 0
			|| (nchrs > 0 && ft_strncmp(buff, limiter, nchrs - 1) == 0))
			break ;
		write(fd, buff, nchrs);
	}
	ft_free_p((void **)token_p);
	ft_memmove(token_p, token_p + 1, strtab_len(token + 1) * sizeof(char *));
	return (1);
}
