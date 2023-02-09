/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 02:19:04 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/07 05:08:02 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	close_fd(int *fd)
{
	if (fd && *fd >= 3)
	{
		close(*fd);
		*fd = -1;
	}
	return (1);
}

int	close_pipe(int *rd, int *wr)
{
	if (rd && *rd >= 3)
	{
		close(*rd);
		*rd = -1;
	}
	if (wr && *wr >= 3)
	{
		close(*wr);
		*wr = -1;
	}
	return (0);
}

int	init_pipe(int pp[2], int *rd_pipe, int i, int nb_cmds)
{
	int	isfirst;
	int	islast;

	isfirst = (i == 0);
	islast = (i == (nb_cmds - 1));
	if (!islast && pipe(pp) < 0)
		return (report_pipe_err());
	if (isfirst)
		*rd_pipe = STDIN_FILENO;
	if (islast)
	{
		pp[0] = -1;
		pp[1] = STDOUT_FILENO;
	}
	return (0);
}
