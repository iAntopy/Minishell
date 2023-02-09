/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substitute_env_vars.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 05:04:41 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/07 06:55:32 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_env_char(char c, int is_first)
{
	if (c == '_')
		return (1);
	if (is_first)
		return (ft_isalpha(c) || c == '?');
	return (ft_isalnum(c));
}

// Starting at $ + 1
char	*skip_valid_envp_var_chars(char *var)
{
	char	*start;

	start = var;
	while (is_valid_env_char(*var, start == var))
		var++;
	if (start == var && (*var != '\'' || *var != '\"'))
		return (var + 1);
	return (var);
}

static int	skip_cpy_open_qts(char **line, char **cpy, size_t *size, char *qte)
{
	char	*quote;
	size_t	len;

	if (line && *line && !**line)
		return (0);
	if (**line == '\"')
		*qte = **line * (*qte == '\0');
	if (!*qte && **line == '\'')
	{
		quote = ft_strchr(*line + 1, '\'');
		len = quote - *line;
		if (cpy)
		{
			ft_strncpy(*cpy, *line, len);
			*line += len;
			*cpy += len;
		}
		else
		{
			*line = quote;
			if (size)
				*size += len;
		}
	}
	return (1);
}

static size_t	find_subst_size(t_msh *msh, char *l, char **vals)
{
	char	var[4096];
	size_t	size;
	char	*p;
	char	qte;

	size = 0;
	qte = '\0';
	while (skip_cpy_open_qts(&l, NULL, &size, &qte))
	{
		if (*l == '$' && *(l + 1) && !ft_strchr(" \'\"", *(l + 1)))
		{
			p = skip_valid_envp_var_chars(l + 1);
			ft_strncpy(var, l + 1, p - (l + 1));
			*vals = msh_getenv(msh, var);
			size += ft_strlen(*vals);
			l = p - 1 + (**vals == '\0' && *p != '\0');
			vals++;
		}
		size++;
		l++;
	}
	return (size + 1);
}

// Assumes line has been checked for at least one $ (dollar char).
int	substitute_env_vars(t_msh *msh, char *s, char **ret)
{
	char	*vals[1024];
	char	*r;
	int		v;
	char	qte;

	ft_memclear(vals, sizeof(char *) * 1024);
	if (!ft_malloc_p(find_subst_size(msh, s, (char **)vals), (void **)ret))
		return (report_malloc_err());
	qte = 0;
	v = 0;
	r = *ret;
	while (skip_cpy_open_qts(&s, &r, NULL, &qte))
	{
		if (*s == '$' && *(s + 1) && !ft_strchr(" \'\"", *(s + 1)))
		{
			s = skip_valid_envp_var_chars(s + 1);
			r += ft_strlcpy(r, vals[v], ft_strlen(vals[v]) + 1);
			v++;
		}
		else
			*(r++) = *(s++);
	}
	*r = '\0';
	return (0);
}
