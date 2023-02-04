/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handlers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 20:33:23 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/03 23:01:28 by iamongeo         ###   ########.fr       */
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
	strip_quotes(filename);
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
	strip_quotes(filename);
	close_fd(&cmd->redir_out);
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


static void	hd_rl_env_sub(t_cmd *cmd, char *limiter, int fd)//, char **ret_line)
{
	char	*rl;
	char	*nrl;

	printf("CHILD : entered \n");
	rl = NULL;
	nrl = NULL;
	while (1)
	{
		printf("CHILD : readline \n");
		rl = readline("> ");
		printf("CHILD : readline done\n");

		if (!rl)
			exit(close_pipe(&fd, &cmd->redir_in) + 1);
		if (substitute_env_vars_heredoc(cmd->job->msh, rl, &nrl) < 0)
			exit(-1);
		if (ft_strncmp(rl, limiter, ft_strlen(limiter)) == 0)
			break ;
		write(fd, nrl, ft_strlen(nrl));
		ft_free_p((void **)&rl);
		ft_free_p((void **)&nrl);
	}
	printf("CHILD : quit while\n");
	ft_free_p((void **)&rl);
	close_pipe(&fd, &cmd->redir_in);
	exit(job_clear(cmd->job, 0) | msh_clear(cmd->job->msh, 0));
}

int	get_heredoc_input(t_cmd *cmd, char **tks_p, int *id_p)
{
	char	*limiter;
//	int		fd;
//	char	*rl;
	char	tmp[PATH_MAX];
	int		cur_len;
	int		status;

	cmd->job->msh->tmp_fd = open(gen_tempname(tmp, (*id_p)++),
		O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (cmd->job->msh->tmp_fd < 0)
		return (report_file_error(tmp, cmd));
	limiter = *tks_p + 2;
	strip_quotes(limiter);

	handlers_control(cmd->job->msh, HEREDOC_MODE);
	cmd->job->msh->hd_pid = fork();
	if (cmd->job->msh->hd_pid < 0)
		return (report_fork_err(__FUNCTION__));
	if (cmd->job->msh->hd_pid == 0)
		hd_rl_env_sub(cmd, limiter, cmd->job->msh->tmp_fd);

	waitpid(cmd->job->msh->hd_pid, &status, 0);

	printf("waitpid status : %d\n", WEXITSTATUS(status));
	if (status == 1)
		printf("SIGQUIT occured in heredoc\n");
	else if (status == 3)
		printf("SIGINT occured in heredoc\n");

	cmd->job->msh->hd_pid = 0;
	handlers_control(cmd->job->msh, INTERAC_MODE);
	close_pipe(&cmd->redir_in, &cmd->job->msh->tmp_fd);
	cmd->redir_in = open(tmp, O_RDONLY);//cmd->job->msh->tmp_fd;
	cur_len = strtab_len(tks_p);
	ft_free_p((void **)tks_p);
	ft_memmove(tks_p, tks_p + 1, (cur_len - 1) * sizeof(char *));
	tks_p[cur_len - 1] = NULL;
	return (1);
}
