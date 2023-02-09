/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 07:27:37 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/07 06:15:13 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	report_malloc_err(void)
{
	ft_eprintf("Minishell : malloc error\n");
	return (-1);
}

int	report_fork_err(void)
{
	ft_eprintf("Minishell : fork failed\n");
	return (-1);
}

int	report_pipe_err(void)
{
	ft_eprintf("Minishell : pipe failed\n");
	return (-1);
}

int	report_parsing_error(const char *fn, char *meta_c, int len)
{
	char	meta_buff[8];

	if (!meta_c || !len)
		return (-1);
	ft_memcpy(meta_buff, meta_c, len);
	meta_buff[len] = '\0';
	if (fn)
		ft_eprintf("Minishell %s: parsing error near \'%s\'\n", fn, meta_buff);
	return (-1);
}

int	report_unclosed_quotes(void)
{
	ft_eprintf("Minishell: syntax error : unclosed quotes\n");
	return (-1);
}
