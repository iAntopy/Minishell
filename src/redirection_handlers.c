/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handlers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 20:33:23 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/07 23:55:11 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_redir_token(char **tks_p)
{
	int	cur_len;

	cur_len = strtab_len(tks_p);
	ft_free_p((void **)tks_p);
	ft_memmove(tks_p, tks_p + 1, (cur_len - 1) * sizeof(char *));
	tks_p[cur_len - 1] = NULL;
}

int	redirect_infile(t_cmd *cmd, char **tks_p)
{
	char	*filename;
	int		fd;

	if (!tks_p)
		return (-1);
	close_fd(&cmd->redir_in);
	filename = *tks_p + 1;
	strip_quotes(filename);
	if (access(filename, F_OK | R_OK) < 0)
		return (report_file_error(filename, cmd));
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (report_file_error(filename, cmd));
	cmd->redir_in = fd;
	remove_redir_token(tks_p);
	return (1);
}

int	redirect_outfile(t_cmd *cmd, char **tks_p, int add_mode)
{
	char	*filename;
	int		fd;
	int		mlen;

	if (!tks_p)
		return (-1);
	is_meta_char(*tks_p, &mlen);
	filename = *tks_p + mlen;
	strip_quotes(filename);
	close_fd(&cmd->redir_out);
	if (access(filename, F_OK) == 0 && access(filename, W_OK) < 0)
		return (report_file_error(filename, cmd));
	fd = open(filename, O_CREAT | O_WRONLY | add_mode, 0644);
	if (fd < 0)
		return (report_file_error(filename, cmd));
	cmd->redir_out = fd;
	remove_redir_token(tks_p);
	return (1);
}

static void	hd_rl_env_sub(t_msh *msh, t_cmd *cmd, char *limiter, char *tmp)
{
	char	*rl;
	char	*nrl;

	msh->is_hd_child = 1;
	signal(SIGINT, sig_handler_heredoc_child);
	cmd->job->tmp_fd = open(tmp, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (cmd->job->tmp_fd < 0)
		exit(job_clear(cmd->job, 0) | msh_clear(cmd->job->msh, -1));
	rl = NULL;
	nrl = NULL;
	while (ft_free_p((void **)&rl) && ft_free_p((void **)&nrl))
	{
		rl = readline("> ");
		if (!rl)
			exit(job_clear(cmd->job, 0) | msh_clear(msh, 0));
		if (ft_strcmp(rl, limiter) == 0)
			break ;
		if (substitute_env_vars_heredoc(msh, rl, &nrl) < 0)
			exit(job_clear(cmd->job, 0) | msh_clear(msh, -1));
		write(cmd->job->tmp_fd, nrl, ft_strlen(nrl));
	}
	ft_free_p((void **)&rl);
	ft_free_p((void **)&nrl);
	close_pipe(&cmd->job->tmp_fd, &cmd->redir_in);
	exit(job_clear(cmd->job, 0) | msh_clear(msh, 0));
}

int	get_heredoc_input(t_msh *msh, t_cmd *cmd, char **tks_p)
{
	char	*limiter;
	char	tmp[PATH_MAX];
	int		status;

	gen_tempname(tmp, msh->hd_id++);
	limiter = *tks_p + 2;
	strip_quotes(limiter);
	handlers_control(msh, HEREDOC_MODE);
	msh->hd_pid = fork();
	if (msh->hd_pid < 0)
		return (report_fork_err());
	if (msh->hd_pid == 0)
		hd_rl_env_sub(msh, cmd, limiter, tmp);
	waitpid(msh->hd_pid, &status, 0);
	handlers_control(msh, INTERAC_MODE);
	msh->exit_status = 130 * (WEXITSTATUS(status) == EXIT_SIGINT);
	if (WEXITSTATUS(status) == EXIT_SIGINT)
		return (-1);
	msh->hd_pid = 0;
	close_fd(&cmd->redir_in);
	cmd->redir_in = open(tmp, O_RDONLY);
	if (cmd->redir_in < 0)
		return (-1);
	remove_redir_token(tks_p);
	return (1);
}
