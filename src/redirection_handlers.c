/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handlers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 20:33:23 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/02 23:47:38 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redirect_infile(t_cmd *cmd, char **tks_p)
{
	char	*filename;
	int		fd;
	int		cur_len;

	if (!tks_p)
		return (-1);
	close_fd(&cmd->redir_in);
	filename = *tks_p + 1;
	if (access(filename, F_OK | R_OK) < 0)
		return (report_file_error(filename, cmd));
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (report_file_error(filename, cmd));
	cmd->redir_in = fd;
	cur_len = strtab_len(tks_p);
	ft_free_p((void **)tks_p);
	ft_memmove(tks_p, tks_p + 1, (cur_len - 1) * sizeof(char *));
	tks_p[cur_len - 1] = NULL;
	return (1);
}

int	redirect_outfile(t_cmd *cmd, char **tks_p, int add_mode)
{
	char	*filename;
	int		fd;
	int		cur_len;

	if (!tks_p)
		return (-1);
	is_meta_char(*tks_p, &cur_len);
	filename = *tks_p + cur_len;
	close_fd(&cmd->redir_in);
	if (access(filename, F_OK) == 0 && access(filename, W_OK) < 0)
		return (report_file_error(filename, cmd));
	fd = open(filename, O_CREAT | O_WRONLY | add_mode, 0644);
	if (fd < 0)
		return (report_file_error(filename, cmd));
	cmd->redir_out = fd;
	cur_len = strtab_len(tks_p);
	ft_free_p((void **)tks_p);
	ft_memmove(tks_p, tks_p + 1, (cur_len - 1) * sizeof(char *));
	tks_p[cur_len - 1] = NULL;
	return (1);
}

char	*gen_tempname(char *tempfile, int id)
{
	char	*base_end;
	char	*num_end;

	base_end = tempfile + 12;
	ft_strlcpy(tempfile, "tmp/.heredoc", PATH_MAX);
	num_end = base_end + ft_putnbr_buff(base_end, id);
	ft_strlcpy(num_end, ".tmp", PATH_MAX);
	return (tempfile);
}

static int	hd_rl_env_sub(t_job *job, char **ret_line)
{
	char	*rl;

	rl = readline("> ");
	if (!rl)
		return (-1);
	if (substitute_env_vars_heredoc(job->msh, rl, ret_line) < 0)
		return (-1);
	ft_free_p((void **)&rl);
	return (0);
}

int	get_heredoc_input(t_cmd *cmd, char **tks_p, int *id_p)
{
	char	*limiter;
	int		fd;
	char	*rl;
	char	tmp[PATH_MAX];
	int		cur_len;

	fd = open(gen_tempname(tmp, (*id_p)++), O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		return (report_file_error(tmp, cmd));
	limiter = *tks_p + 2;
	rl = NULL;
	while (ft_free_p((void **)&rl) && hd_rl_env_sub(cmd->job, &rl) >= 0
		&& ft_strncmp(rl, limiter, ft_strlen(limiter)) != 0)
		write(fd, rl, ft_strlen(rl));
	ft_free_p((void **)&rl);
	close_pipe(&fd, &cmd->redir_in);
	cmd->redir_in = open(tmp, O_RDONLY);
	cur_len = strtab_len(tks_p);
	ft_free_p((void **)tks_p);
	ft_memmove(tks_p, tks_p + 1, (cur_len - 1) * sizeof(char *));
	tks_p[cur_len - 1] = NULL;
	return (1);
}
