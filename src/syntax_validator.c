/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validator.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 18:45:09 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/07 06:46:43 by iamongeo         ###   ########.fr       */
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

char	*skip_spaces(char **line, int init_offset, int chg_inplace)
{
	char	*l;

	l = *line;
	l += init_offset;
	while (ft_isspace(*l))
		l++;
	if (chg_inplace)
		*line = l;
	return (l);
}

static int	skip_open_quotes_syntax(char **line, int *error, int *exit_status)
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
		if (error)
			*error = -1;
		if (exit_status)
			report_syntax_error(l, 1, exit_status, quote);
		return (0);
	}
	*line = l;
	return (1);
}

static int	is_valid_meta_combo(char *m1, int mlen1, char *m2)
{
	if ((*m1 == '&' && mlen1 == 2) && (*m2 == '<' || *m2 == '>'))
		return (1);
	if (*m1 == '|' && (*m2 == '<' || *m2 == '>'))
		return (1);
	return (0);
}

int	validate_syntax(char *l, int *exit_status)
{
	int		mlen;
	int		mlen2;
	char	*k;
	int		error;

	if (!l || !(*l))
		return (-1);
	if (skip_spaces(&l, 0, 1) && is_meta_char(l, &mlen)
		&& !ft_strchr("<>", *l))
		return (report_syntax_error(l, mlen, exit_status, 0));
	error = 0;
	while (skip_open_quotes_syntax(&l, &error, exit_status) && *l)
	{
		if (is_meta_char(l, &mlen))
		{
			k = skip_spaces(&l, mlen, 0);
			if (!*k || (is_meta_char(k, &mlen2)
					&& !is_valid_meta_combo(l, mlen, k)))
				return (report_syntax_error(k, mlen2, exit_status, 0));
			l += mlen;
		}
		else
			l++;
	}
	return (error);
}
