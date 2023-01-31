/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validator.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 18:45:09 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/30 23:31:42 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	report_syntax_error(char *near, int mlen)
{
	if (*near == '\0')
		ft_eprintf("minishell: syntax error near unexpected token 'newline'\n");
	else
	{
		ft_eprintf("minishell: syntax error near unexpected token '");
		write(STDERR_FILENO, near, mlen);
		write(STDERR_FILENO, "'\n", 2);
	}
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

static int	skip_open_quotes(char **line)
{
	char	*l;

	l = *line;
	if (!l[0])
		return (0);
	if (!(*l == '\'' || *l == '\"'))
		return (1);
	else
		l++;
	while (*l && *l != '\'' && *l != '\"')
		l++;
	if (*l == '\0')
		return (0);
	*line = l + 1;
	return (1);
}

int	validate_syntax_beggining_or_ending_pipes(char *line)
{
	if (!line)
		return (report_missing_input(__FUNCTION__));
	if (line[0] == '|' || line[ft_strlen(line) - 1] == '|')
		return (report_syntax_error("|", 1));
	return (0);
}

int	validate_meta_char_syntax(char *line)
{
	int		mlen;
	int		mlen2;
	char	*l;
	char	*k;

	if (!line || !(*line))
		return (-1);
	l = line;
	while (skip_open_quotes(&l) && *l)
	{
		if (is_meta_char(l, &mlen))
		{
			k = l + mlen;
			skip_spaces(&k, 0);
			if ((*l == '<' || *l == '>') && is_meta_char(k, &mlen2))
				return (report_syntax_error(k, mlen2));
			l += mlen;
		}
		else
			l++;
	}
	return (0);
}
/*
int	validate_syntax(char *line)
{
//	int	i;
	int	mlen;
	char	*l;

	if (!line)
		return (-1);
	l = line;
	while (*l)
	{
		if (is_meta_char(l, &mlen) && skip_spaces(&l, mlen)
			&& (*l == '\0' || is_meta_char(l, &mlen)))
			return (report_syntax_error(l, mlen));
		l++;
	}
	return (0);
}

int	main()
{
	int		is_valid;
	char	line[] = "<< grep allo";
	
	if (validate_syntax(line) < 0)
		printf("the line '%s' is not valid syntax.\n", line);
	else	
		printf("the line '%s' is valid syntax.\n", line);

	return (0);
}
*/
