/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 19:52:25 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/26 21:19:54 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	report_bad_inputs(int argc)
{
	errno = EINVAL;
	ft_eprintf(RED_BC"<[ PIPEX ERROR :: wrong nb of args received (%d) ]>\n",
		argc - 1);
	ft_eprintf(WHITE_C"\n\n pipex arguments format : \n");
	ft_eprintf("\teither\t{infile} {cmd1} {cmd2} {...} {cmdn} {outfile}\n");
	ft_eprintf("\tor\there_doc {LIMITER} {cmd1} {cmd2} {...} {cmnd} {outfile}\n");
	return (errno);
}

int	report_file_error(char *filename)
{
	ft_eprintf(RED_BC"<[ PIPEX ERROR :: %s : %s ]>\n"WHITE_C,
		strerror(errno), filename);
	return (errno);
}

int	report_error(char *err)
{
	ft_eprintf(RED_BC"<[ PIPEX ERROR :: %s ]>\n"WHITE_C, err);
	return (errno);
}

int	report_bad_cmd(char ***argv, char **filename)
{
	int	status;

	if (filename && *filename)
	{
		status = EACCES;
		ft_eprintf(RED_BC"<[ PIPEX ERROR :: Permission denied %s ]>\n"WHITE_C,
			(*argv)[0]);
	}
	else
	{
		status = ENOCMD;
		ft_eprintf(RED_BC"<[ PIPEX ERROR :: command not found : %s ]>\n"WHITE_C,
			(*argv)[0]);
	}
	malloc_free_p(0, (void **)filename);
	strtab_clear(argv);
	errno = status;
	return (status);
}

int	report_execve_failed(char *cmd)
{
	ft_eprintf(RED_BC"<[ PIPEX ERROR :: %s cmd execution failed ]>\n"WHITE_C,
		cmd);
	return (errno);
}
