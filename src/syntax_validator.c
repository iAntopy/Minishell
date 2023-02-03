/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validator.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 18:45:09 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/02 21:17:06 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	report_syntax_error(char *near, int mlen, int *exit_status, char quote)
{
	if (*near == '\0')
	{
		if (quote)
			report_unclosed_quotes();
		else
			ft_eprintf("minishell: syntax error near \
				unexpected token 'newline'\n");
	}
	else
	{
		ft_eprintf("minishell: syntax error near unexpected token '");
		write(STDERR_FILENO, near, mlen);
		write(STDERR_FILENO, "'\n", 2);
	}
	*exit_status = 258;
	return (-1);
}

static char	*skip_spaces(char **line, int init_offset)
{
	char	*l;

	l = *line;
	l += init_offset;
	while (ft_isspace(*l))
		l++;
	*line = l;
	return (l);
}

static int	skip_open_quotes(char **line, int *error, int *exit_status)
{
	char	quote;
	char	*l;

	l = *line;
	if (!l[0])
		return (0);
	if (!(*l == '\'' || *l == '\"'))
		return (1);
	quote = *(l++);
	while (*l && *l != quote)
		l++;
	if (*l == '\0')
	{
		*error = -1;
		report_syntax_error(l, 1, exit_status, quote);
		return (0);
	}
	*line = l;
	return (1);
}

int	validate_syntax(char *line, int *exit_status)
{
	int		mlen;
	int		mlen2;
	char	*l;
	char	*k;
	int		error;

	if (!line || !(*line))
		return (-1);
	if (line[0] == '|' || line[ft_strlen(line) - 1] == '|')
		return (report_syntax_error("|", 1, exit_status, 0));
	l = line;
	error = 0;
	while (skip_open_quotes(&l, &error, exit_status) && *l)
	{
		if (is_meta_char(l, &mlen))
		{
			k = l + mlen;
			skip_spaces(&k, 0);
			if ((*l == '<' || *l == '>') && (!*k || is_meta_char(k, &mlen2)))
				return (report_syntax_error(k, mlen2, exit_status, 0));
			l += mlen;
		}
		else
			l++;
	}
	return (error);
}
