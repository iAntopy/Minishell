/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handlers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 20:33:23 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/06 19:14:29 by iamongeo         ###   ########.fr       */
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
//	cur_len = strtab_len(tks_p);
//	ft_free_p((void **)tks_p);
//	ft_memmove(tks_p, tks_p + 1, (cur_len - 1) * sizeof(char *));
//	tks_p[cur_len - 1] = NULL;
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
//	cur_len = strtab_len(tks_p);
//	ft_free_p((void **)tks_p);
//	ft_memmove(tks_p, tks_p + 1, (cur_len - 1) * sizeof(char *));
//	tks_p[cur_len - 1] = NULL;
	return (1);
}

char	*gen_tempname(char *tmpfile, int id)
{
	char	*base_end;
	char	*num_end;

	base_end = tmpfile + 12;
	ft_strlcpy(tmpfile, "tmp/.heredoc", PATH_MAX);
	num_end = base_end + ft_putnbr_buff(base_end, id);
	ft_strlcpy(num_end, ".tmp", PATH_MAX);
	return (tmpfile);
}

static void	sig_handler_heredoc_child(int signum)
{
	t_msh	*msh;

	if (signum == SIGINT)
	{
		msh = get_msh();
		printf("SIGINT handled by heredoc child\n");
		job_clear(&msh->job, 0);
		msh_clear(msh, 0);
		exit(EXIT_SIGINT);
	}
}

static void	hd_rl_env_sub(t_msh *msh, t_cmd *cmd, char *limiter, char *tmp)
{
	char	*rl;
	char	*nrl;

	printf("CHILD : entered \n");
	msh->is_hd_child = 1;
	signal(SIGINT, sig_handler_heredoc_child);
	cmd->job->tmp_fd = open(tmp, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (cmd->job->tmp_fd < 0)
	{
		ft_eprintf("minishell :: heredoc tmpfile critical failure\n");
		exit(job_clear(cmd->job, 0) | msh_clear(cmd->job->msh, -1));
	}
	rl = NULL;
	nrl = NULL;
	while (ft_free_p((void **)&rl) && ft_free_p((void **)&nrl))
	{
		rl = readline("> ");
		if (!rl)
		{
			printf("heredoc CHILD : rl == NULL. SIGQUIT received\n");
			exit(job_clear(cmd->job, 0) | msh_clear(msh, 0));
		}
		if (substitute_env_vars_heredoc(msh, rl, &nrl) < 0)
			exit(job_clear(cmd->job, 0) | msh_clear(msh, -1));
		if (ft_strncmp(nrl, limiter, ft_strlen(rl)) == 0)
			break ;
		write(cmd->job->tmp_fd, nrl, ft_strlen(nrl));
	}
	printf("CHILD : quit while\n");
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

//	cmd->job->msh->tmp_fd = open(gen_tempname(tmp, (*id_p)++),
//		O_CREAT | O_TRUNC | O_WRONLY, 0644);
	printf("Get heredoc input entered \n");
	gen_tempname(tmp, msh->hd_id++);
	if (cmd->job->tmp_fd < 0)
		return (report_file_error(tmp, cmd));
	limiter = *tks_p + 2;
	strip_quotes(limiter);
	handlers_control(msh, HEREDOC_MODE);
	msh->hd_pid = fork();
	if (msh->hd_pid < 0)
		return (report_fork_err(__FUNCTION__));
	if (msh->hd_pid == 0)
		hd_rl_env_sub(msh, cmd, limiter, tmp);

	waitpid(msh->hd_pid, &status, 0);
	handlers_control(msh, INTERAC_MODE);
	printf("waitpid status : %d\n", WEXITSTATUS(status));

//	if (WEXITSTATUS(status) == 1)
//		printf("SIGQUIT occured in heredoc\n");
	if (WEXITSTATUS(status) == EXIT_SIGINT)
	{
		printf("SIGINT occured in heredoc\n");
		msh->exit_status = 130;
		return (-1);
	}
	msh->hd_pid = 0;
	close_fd(&cmd->redir_in);
	cmd->redir_in = open(tmp, O_RDONLY);
	if (cmd->redir_in < 0)
	{
		perror("heredoc PARENT : failed to open tmpfile");
		return (-1);
	}
	remove_redir_token(tks_p);
	return (1);
}
