/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substitute_env_vars_heredoc.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 05:04:41 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/07 06:54:49 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Starting at $ + 1
static char	*skip_valid_envp_var_chars_hd(char *var)
{
	char	*start;

	start = var;
	while (is_valid_env_char(*var, start == var))
		var++;
	if (start == var && !ft_strchr(" \'\"", *var))
		return (var + 1);
	return (var);
}

static size_t	find_subst_size(t_msh *msh, char *l, char **vals)
{
	char	var[4096];
	size_t	size;
	char	*p;

	size = 0;
	l--;
	while (*(++l))
	{
		if (*l == '$' && *(l + 1) && !ft_strchr(" \'\"", *(l + 1)))
		{
			p = l;
			p = skip_valid_envp_var_chars_hd(l + 1);
			ft_strncpy(var, l + 1, p - (l + 1));
			*vals = msh_getenv(msh, var);
			size += ft_strlen(*vals);
			l = p - 1 + (**vals == '\0');
			vals++;
		}
		size++;
	}
	return (size + 2);
}

// Assumes line has been checked for at least one $ (dollar char).
int	substitute_env_vars_heredoc(t_msh *msh, char *s, char **ret)
{
	char	*vals[1024];
	char	*r;
	int		v;

	if (!msh || !msh->envp || !s)
		return (-1);
	if (!ft_malloc_p(sizeof(char) * find_subst_size(msh, s, vals),
			(void **)ret))
		return (report_malloc_err());
	v = 0;
	r = *ret;
	while (s && *s)
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
	ft_strncpy(r, "\n", 1);
	return (0);
}
