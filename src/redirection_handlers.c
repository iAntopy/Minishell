/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handlers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 20:33:23 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/01 00:23:18 by iamongeo         ###   ########.fr       */
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

//	(void)add_mode;
//	printf("Redirecting to outfile\n");
//	printf("redirect outfile cmds list: \n");
//	printf("redir out in append mode ? %d\n", (add_mode & O_APPEND) != 0);
//	print_all_cmds(cmd->job);
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

int	find_heredoc_env_subst_size(t_job *job, char *rl, char *var_buff, char **vals)
{
	int		i;
	int		j;
	int		extra_size;

	extra_size = 0;
	i = 0;
	while (rl[i])
	{
		if (rl[i] == '$' && !ft_isspace(rl[i + 1]))
		{
			j = i;
			while (rl[j] && rl[++j] && (ft_isalnum(rl[j]) || rl[j] == '_'))
				var_buff[j - i - 1] = rl[j];
			var_buff[j - i - 1] = '\0';
			*vals = msh_getenv(job->msh, var_buff);
			extra_size += ft_strlen(*vals);
			vals++;
		}
		else
			i++;
	}
	return (i + extra_size + 2);
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
				np += ft_strlcpy(np, vals[v++], SIZE_MAX);
			}
		}
		else
			rp++;
	}
	i = ft_strlcpy(np, rp, SIZE_MAX);
	ft_strlcpy(np + i, "\n\0", 2);
	return (0);
}

int	heredoc_readline_env_var_convert(t_job *job, char **ret_line)
{
	char	*rl;
	char	var_buff[4096];
	int		ret_status;

	rl = readline("> ");
	if (!rl)
		return (-1);
	ret_status = substitute_heredoc_env_vars(job, rl, ret_line, var_buff);
	ft_free_p((void **)&rl);
	return (ret_status);
}

int	get_heredoc_input(t_cmd *cmd, char **tks_p, int *id_p)
{
	char	*limiter;
	int		fd;
	char	*rl;
	char	tempname[PATH_MAX];
	int		cur_len;

	fd = open(gen_tempname(tempname, (*id_p)++), O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		return (report_file_error(tempname, cmd));//&cmd->doa));
	limiter = *tks_p + 2;
	rl = NULL;
	while (ft_free_p((void **)&rl) && heredoc_readline_env_var_convert(cmd->job, &rl) >= 0 &&
			rl && rl[0] && ft_strncmp(rl, limiter, ft_strlen(limiter)) != 0)
//	{
		if (rl)
			write(fd, rl, ft_strlen(rl));
//		write(fd, "\n", 1);
//	}
	ft_free_p((void **)&rl);
	close_pipe(&fd, &cmd->redir_in);
//	close_fd(&cmd->redir_in);
	cmd->redir_in = open(tempname, O_RDONLY);
	cur_len = strtab_len(tks_p);
	ft_free_p((void **)tks_p);
	ft_memmove(tks_p, tks_p + 1, (cur_len - 1) * sizeof(char *));
	tks_p[cur_len - 1] = NULL;
	return (1);
}
