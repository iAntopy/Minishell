/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substitute_env_vars.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 05:04:41 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/03 01:15:19 by iamongeo         ###   ########.fr       */
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

//	printf("var start : %s\n", var);
	start = var;
	while (is_valid_env_char(*var, start == var))
		var++;
	if (start == var && (*var != '\'' || *var != '\"'))
		return (var + 1);
//	printf("var end : %s\n", var);
	return (var);
}

static int	skip_open_quotes(char **line, char **cpy, size_t *size)
{
	char	*quote;
	size_t	len;
	

//	l = *line;
//	printf("skip quote line start : %s\n", *line);
	if (!**line)
		return (0);
	if (**line == '\'')
	{
		quote = ft_strchr(*line + 1, '\'');
		len = quote - *line;
	//	printf("skip : line before : %s\n", *line);
		if (cpy)
		{
	//		printf("quote - *line : %ld\n", quote - *line);
			ft_strncpy(*cpy, *line, len);
	//		printf("len : %zu\n", len);
			*line += len;
			*cpy += len;
		}
		else
		{
			*line = quote;
			if (size)
				*size += len;
		}
		
//		while (*(++l) && *l != '\'')
//			continue ;
		//printf("skip : line after : %s\n", *line);
	}
//	*line = l;
	//printf("skip quote line after: %s\n", *line);
	return (1);
}

static size_t	find_subst_str_size(t_msh *msh, char *l, char **vals)
{
	char	var[4096];
	size_t	size;
	char	*p;

	size = 0;
	while (skip_open_quotes(&l, NULL, &size))
	{
		if (*l == '$' && !(*(l + 1) == '\'' || *(l + 1) == '\"'))
		{
			p = l;
			p = skip_valid_envp_var_chars(l + 1);
			ft_strncpy(var, l + 1, p - (l + 1));
			*vals = msh_getenv(msh, var);
//			printf("val found (%s) at l : %s\n", *vals, l);
			size += ft_strlen(*vals);
			l = p - 1 + (**vals == '\0');
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
//	char	qte;

	//printf("sub entered\n");
	ft_memclear(vals, sizeof(char *) * 1024);
	if (!ft_malloc_p(find_subst_str_size(msh, s, (char **)vals), (void **)ret))
		return (report_malloc_err(__FUNCTION__));
//	qte = 0;
	v = 0;
	r = *ret;
//	printf("WOWWY!\n");
	while (*s)
	{
//		printf("whiling s : %s\n", s);
	//	if (*s == '\'' || *s == '\"')
	//		qte = (*s) * (qte == 0);
		if (*s == '\'')
			skip_open_quotes(&s, &r, NULL);
		//if (*s == '$' && qte != '\'' && *(s + 1) && !ft_strchr(" \'\"", *(s + 1)))
		if (*s == '$' && *(s + 1) && !ft_strchr(" \'\"", *(s + 1)))
		{
//			printf("s env char before skip : %s\n", s);
			s = skip_valid_envp_var_chars(s + 1);
//			printf("s env char after skip : %s\n", s);
//			printf("r val cpy before skip : %s\n", r);
			r += ft_strlcpy(r, vals[v], ft_strlen(vals[v]) + 1);
//			printf("r - 5 val cpy after skip : %s\n", r - 5);
			v++;
		}
		else
			*(r++) = *(s++);
	}
	*r = '\0';
	//printf("sub exit : %s\n", *ret);
	return (0);
}
