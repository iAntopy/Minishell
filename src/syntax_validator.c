/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validator.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 18:45:09 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/20 20:09:43 by iamongeo         ###   ########.fr       */
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

int	validate_syntax(char *line)
{
	int	i;
	int	mlen;
	char	*l;

	if (!line)
		return (-1);
	l = line;
	if (line[0] == '|' || line[ft_strlen(line) - 1] == '|')
		return (report_syntax_error("|", 1));
	while (*l)
	{
		if (is_meta_char(l, &mlen) && skip_spaces(&l, mlen)
			&& (*l == '\0' || is_meta_char(l, &mlen)))
			return (report_syntax_error(l, mlen));
		l++;
	}
	return (0);
}
/*
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
