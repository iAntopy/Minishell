/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 03:43:47 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/07 03:44:23 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	sig_handler_heredoc_child(int signum)
{
	t_msh	*msh;

	if (signum == SIGINT)
	{
		msh = get_msh();
		job_clear(&msh->job, 0);
		msh_clear(msh, 0);
		exit(EXIT_SIGINT);
	}
}
