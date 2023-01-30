/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handlers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 20:33:23 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/30 07:33:29 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	report_file_error(char *filename, int *doa_flag)
{
	ft_eprintf("minishell error :: %s : %s\n", strerror(errno), filename);
	*doa_flag = 1;
	return (errno);
}

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
		return (report_file_error(filename, &cmd->doa));
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (report_file_error(filename, &cmd->doa));
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

//	(void)add_mode;
	printf("Redirecting to outfile\n");
	printf("redirect outfile cmds list: \n");
	print_all_cmds(cmd->job);
	filename = *tks_p + 1;
	close_fd(&cmd->redir_in);
	if (access(filename, F_OK) < 0 && !access(filename, W_OK))
		return (report_file_error(filename, &cmd->doa));
	fd = open(filename, O_CREAT | O_WRONLY | add_mode, 0644);
	printf("fd after open : %d\n", fd);
	if (fd < 0)
		return (report_file_error(filename, &cmd->doa));
	cmd->redir_out = fd;
	cur_len = strtab_len(tks_p);
	ft_free_p((void **)tks_p);
	printf("redir out : cur len : %d\n", cur_len);
	ft_memmove(tks_p, tks_p + 1, (cur_len - 1) * sizeof(char *));
	tks_p[cur_len - 1] = NULL;
	return (1);
}

static int	read_exception(void)
{
	ft_eprintf("minishell :: heredoc tmp file open failed.");
	return (-1);
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

int	find_heredoc_env_subst_size(t_job *job, char *rl, char *var_buff, char **vals)
{
	int		i;
	int		j;
	int		extra_size;

	extra_size = 0;
	i = -1;
	while (rl[++i])
	{
		if (rl[i] == '$' && !ft_isspace(rl[i + 1]))
		{
			j = i;
			while (rl[++j] && (ft_isalnum(rl[j]) || rl[j] == '_'))
				var_buff[j - i - 1] = rl[j];
			var_buff[j - i - 1] = '\0';
			*vals = msh_getenv(job->msh, var_buff);
			extra_size += ft_strlen(*vals);
			vals++;
			i = j;
		}
	}
	return (i + extra_size);
}

char	*ft_strchr_p(char *str, char c, char **ptr)
{
	*ptr = ft_strchr(str, c);
	if (*ptr)
		return (*ptr);
	return (NULL);
}

int	substitute_heredoc_env_vars(t_job *job, char *rl, char **ret, char *var_buff)
{
	int		i;
	char	*rp;
	char	*np;
	char	*vals[256];
	char	*var_p;
	int		v;

	*ret = NULL;
	if (!ft_malloc_p(find_heredoc_env_subst_size(job, rl, var_buff, vals), (void **)ret))
		return (report_malloc_err(__FUNCTION__));
	rp = rl;
	np = *ret;
	v = 0;
	while (ft_strchr_p(rp, '$', &var_p) && ft_memcpy(np, rp, var_p - rp))
	{
		np += var_p - rp;
		rp = var_p;
		if (*(rp + 1) == '?')
		{
			np += ft_putnbr_buff(np, job->msh->exit_status);
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
//				var_buff[i] = '\0';
				//vals = msh_getenv(job->msh, var_buff);
				np += ft_strlcpy(np, vals[v++], SIZE_MAX);
			}
		}
		else
			rp++;
	}
	ft_strlcpy(np, rp, SIZE_MAX);
	return (0);
}

int	heredoc_readline_env_var_convert(t_job *job, char **ret_line)
{
	char	*rl;
	char	var_buff[4096];
	int		ret_status;

	rl = readline("> ");
	if (!rl)
		return (read_exception());
	ret_status = substitute_heredoc_env_vars(job, rl, ret_line, var_buff);
	ft_free_p((void **)&rl);
	return (ret_status);
}

int	get_heredoc_input(t_cmd *cmd, char **tks_p, int *id_p)
{
	char	*limiter;
	int		fd;
//	char	buff[HDOC_SIZE];
	char	*rl;
	char	tempname[PATH_MAX];
	int	cur_len;

	fd = open(gen_tempname(tempname, (*id_p)++), O_CREAT | O_TRUNC | O_RDWR, 0644);
	if (fd < 0)
		return (report_file_error(tempname, &cmd->doa));
	limiter = *tks_p + 2;
	rl = NULL;
	while (heredoc_readline_env_var_convert(cmd->job, &rl) >= 0 &&
			rl && rl[0] && ft_strcmp(rl, limiter) != 0)
	{
		printf("heredoc DEGUG : readline received : '%s'. Writing to fd %d\n", rl, fd);
		write(fd, rl, ft_strlen(rl));
	}
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
	
//	int i = -1;
//	while (cmd->tokens[i])
//		printf("heredoc debug : cmd %i ptr : %p\n", i, cmd->tokens[i]);
	close_fd(&cmd->redir_in);
	cmd->redir_in = fd;
	cur_len = strtab_len(tks_p);
	ft_free_p((void **)tks_p);
	ft_memmove(tks_p, tks_p + 1, (cur_len - 1) * sizeof(char *));
	tks_p[cur_len - 1] = NULL;
	return (1);
}
