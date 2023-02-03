/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substitute_env_vars.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 05:04:41 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/02 21:28:25 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
static int	is_space_or_quote(char c)
{
	return (ft_isspace(c) || c == '\'' || c == '\"');
}
*/

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

static size_t	find_subst_str_size(t_msh *msh, char *l, char **vals)
{
	char	var[4096];
	size_t	size;
	char	*p;

//	printf("find subst size : entered. eval sizeof : %s\n", l);
	size = 0;
	l--;
	while (*(++l))
	{
		if (*l == '$')
		{
		//	printf("$ found : %s\n", l);
			p = l;
			p = skip_valid_envp_var_chars(l + 1);
			ft_strncpy(var, l + 1, p - (l + 1));
			*vals = msh_getenv(msh, var);
		//	printf("vals : %s\n", *vals);
			size += ft_strlen(*vals);
		//	printf("val : %p, *val : %p\n", vals, *vals);
			l = p - 1 + (**vals == '\0');
			vals++;
		}
		size++;
	}
//	printf("find subst size : exits with size : %zu\n", size + 1);
	return (size + 1);
}
/*
// printf("find subst size : var : %s = %s, value strlen : %zu\n", var, *vals, ft_strlen(*vals));
// printf("find subst size : var null terminated with i %zu, j %zu, j - i - 1 = %zu\n", i, j, j - i - 1);
static size_t	find_subst_str_size(t_msh *msh, char *l, char **vals)
{
	char	var[4096];
	size_t	size;
	size_t	i;
	size_t	j;
	char	*p;

	printf("find subst size : entered. eval sizeof : %s\n", l);
	size = 0;
	i = -1;
	while (l[++i])
	{
		if (l[i] == '$')
		{
			j = i;
			if (l[j + 1] == '?')
				j += ft_strlcpy(var, "?", 4096);
			else
			{
//				p = skip_valid_envp_var_chars(l + j);
//				ft_strncpy(var, l + i + 1, p - i - 1);
				while (l[++j] && is_valid_env_char(l[j], l[j] == l[i + 1]))
					var[j - i - 1] = l[j];
				var[j - i - 1] = '\0';
			}
			*vals = msh_getenv(msh, var);
			size += ft_strlen(*(vals++));
			i = j;
		}
		size++;
	}
	printf("find subst size : exits with size : %zu\n", size + 1);
	return (size + 1);
}
*/
// Assumes line has been checked for at least one $ (dollar char).
int	substitute_env_vars(t_msh *msh, t_job *job)//char *l, char **ret)
{
	char	*vals[1024];
	size_t	size;
	char	*r;
	int		v;
	char	*l;
	char	quote_switch;

	printf("original len : %zu\n", ft_strlen(job->parsed));
	if (!msh || !msh->envp || !job->parsed)
		return (report_missing_input(__FUNCTION__));
	size = find_subst_str_size(msh, job->parsed, (char **)vals);
	if (!ft_malloc_p(sizeof(char) * size, (void **)&job->parsed2))
		return (report_malloc_err(__FUNCTION__));
	quote_switch = 0;
	v = 0;
	r = job->parsed2;
	l = job->parsed;
	while (l && *l)
	{
		if (*l == '\'' || *l == '\"')
			quote_switch = (*l) * (quote_switch == 0);
		if (*l == '$' && quote_switch != '\'')
		{
			l = skip_valid_envp_var_chars(l + 1);
			//printf("r before : %p, v : %d, val : %s\n", r, v, vals[v]);
			r += ft_strlcpy(r, vals[v++], size);
			//printf("r after : %p, v: %d, val : %s\n", r, v, vals[v]);
//			printf("parsed2 after substitution : %s\n", job->parsed2);
		}
		else
			*(r++) = *(l++);
	}
	printf("subst final result (len %zu) : %s\n", ft_strlen(job->parsed2), job->parsed2);
	*r = '\0';
	return (0);
}
