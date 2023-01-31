/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substitute_env_vars.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 05:04:41 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/30 23:42:44 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
static int	is_space_or_quote(char c)
{
	return (ft_isspace(c) || c == '\'' || c == '\"');
}
*/
int	is_valid_envp_var_char(char c, int is_first)
{
	if (c == '_')
		return (1);
	if (is_first)
		return (ft_isalpha(c));
	return (ft_isalnum(c));
}

// printf("find subst size : var : %s = %s, value strlen : %zu\n", var, *vals, ft_strlen(*vals));
// printf("find subst size : var null terminated with i %zu, j %zu, j - i - 1 = %zu\n", i, j, j - i - 1);
static size_t	find_substituted_str_size(t_msh *msh, char *line, char **vals)
{
	char	var[4096];
	size_t	size;
	size_t	i;
	size_t	j;

	printf("find subst size : entered\n");
	size = 0;
	i = -1;
	while (line[++i])
	{
		if (line[i] == '$')
		{
			j = i;
			while (line[++j] && is_valid_envp_var_char(line[j], line[j] == line [i + 1]))//!is_space_or_quote(line[j]))//!ft_isspace(line[j]) && line[j] != '\'' && line[j] != '\"')
				var[j - i - 1] = line[j];
			printf("find subst size : var space found\n");
			var[j - i - 1] = '\0';
			*vals = msh_getenv(msh, var);
			size += ft_strlen(*vals) - (j - i - 1);
			vals++;
		}
		size++;
	}
	printf("find subst size : exits with size : %zu\n", size);
	return (size);
}

// Starting at $ + 1
char	*skip_valid_envp_var_chars(char *var)
{
	char	*start;

	start = var;
	while (is_valid_envp_var_char(*var, start == var))
		var++;
	return (var);
}

// Assumes line has been checked for at least one $ (dollar char).
int	msh_substitute_env_vars(t_msh *msh, char *l, char **ret)
{
	char	*vals[256];
	size_t	size;
	char	*r;
	int		v;
	char	quote_switch;

	if (!msh || !msh->envp || !l || !ret)
		return (report_missing_input(__FUNCTION__));
	size = find_substituted_str_size(msh, l, (char **)vals);
	if (!ft_malloc_p(sizeof(char) * size, (void **)ret))
		return (report_malloc_err(__FUNCTION__));
	quote_switch = 0;
	v = 0;
	r = *ret;
	while (l && *l)
	{
		if (*l == '\'' || *l == '\"')
			quote_switch = (*l) * (quote_switch == 0);
		if (*l == '$' && quote_switch != '\'')
		{
			l = skip_valid_envp_var_chars(l + 1);
			r += ft_strlcpy(r, vals[v++], size);
		}
		else
			*(r++) = *(l++);
	}
	*r = '\0';
	return (0);
}
	// if (*ret)
	// 	ft_free_p((void **)ret);
	// *ret = out;
