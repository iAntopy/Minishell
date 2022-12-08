/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 07:27:37 by iamongeo          #+#    #+#             */
/*   Updated: 2022/12/08 04:51:52 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	repport_missing_input(const char *fn)
{
	if (fn)
		ft_eprintf("%s : missing input\n", fn);
	return (-1);
}

int	repport_malloc_err(const char *fn)
{
	if (fn)
		ft_eprintf("%s : malloc error\n", fn);
	return (-1);
}

int	repport_fork_err(const char *fn)
{
	if (fn)
		ft_eprintf("%s : fork failed\n", fn);
	return (-1);
}

int	repport_jm_mlc_err(const char *fn)
{
	if (fn)
		ft_eprintf("%s : malloc error\n", fn);
//	errno = ENOMEM;
//	return (errno);
	return (-1);
}

int	repport_pipe_err(const char *fn)
{
	if (fn)
		ft_eprintf("%s : pipe init failed\n", fn);
	return (-1);
//	return (errno);
}

int	repport_parsing_error(const char *fn, char *meta_c, int len)
{
	char	meta_buff[8];
	if (!meta_c || !len)
		return (-1);
	ft_memcpy(meta_buff, meta_c, len);
	meta_buff[len] = '\0';
	if (fn)
		ft_eprintf("%s : parsing error near \'%s\'\n", fn, meta_buff);
	return (-1);
}

int	repport_builtin_failure(const char *fn)
{
	if (fn)
		ft_eprintf("%s : a builtin failed to execute\n", fn);
	return (-1);
//	return (errno);
}

