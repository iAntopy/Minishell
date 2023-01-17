/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substitute_env_vars.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 05:04:41 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/16 14:16:17 by tbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_space_or_quote(char c)
{
	return (ft_isspace(c) || c == '\'' || c == '\"');
}

static size_t	find_substituted_str_size(t_msh *msh, char *line, char **vals)
{
	char	var[256];
	size_t	size;
	size_t	i;
	size_t	j;

	printf("find subst size : entered\n");
	size = 0;
	i = -1;
	while (line[++i])
	{
//		printf("find subst size : line[i] : %c\n", line[i]);
		if (line[i] == '$')
		{
//			printf("find subst size : $ found at i = %zu\n", i);
			
			j = i;
			while (line[++j] && !is_space_or_quote(line[j]))//!ft_isspace(line[j]) && line[j] != '\'' && line[j] != '\"')
				var[j - i - 1] = line[j];
//			printf("find subst size : var space found\n");
			var[j - i - 1] = '\0';
//			printf("find subst size : var null terminated with i %zu, j %zu, j - i - 1 = %zu\n", i, j, j - i - 1);
			*vals = msh_getenv(msh, var);
//			printf("find subst size : var : %s = %s, value strlen : %zu\n", var, *vals, ft_strlen(*vals));
			size += ft_strlen(*vals) - (j - i - 1);
			vals++;
		}
		size++;
	}
	printf("find subst size : exits with size : %zu\n", size);
	return (size);
}

// Assumes line has been checked for at least one $ (dollar char).
int	msh_substitute_env_vars(t_msh *msh, char *line, char **ret)
{
	char	*vals[256];
	char	*out;
	size_t	size;
	char	*l;
	char	*r;
	int	v;
	char	quote_switch;

	printf("subst env vars : entered : msh %p, envp %p, line %p, ret %p\n", msh, msh->envp, line, ret);
	if (!msh || !msh->envp || !line || !ret)
		return (repport_missing_input(__FUNCTION__));
//	printf("subst env vars : checked \n");
	size = find_substituted_str_size(msh, line, (char **)vals);
//	printf("subst env vars : size = %zu \n", size);
	if (!ft_malloc_p(sizeof(char) * size, (void **)&out))
		return (repport_malloc_err(__FUNCTION__));
//	printf("subst env vars : malloced \n");
	quote_switch = 0;
	l = line;
	r = out;
	v = 0;
	while (l && *l)
	{
		if (*l == '\'' || *l == '\"')
			quote_switch = (*l) * (quote_switch == 0);
//		printf("subst env vars : quote switch at *l = %c : %c\n", *l, quote_switch);
		if (*l == '$' && quote_switch != '\'')
		{
//			printf("subst var : pre while is space or quote\n");
			while (*l && !is_space_or_quote(*l))
				l++;
//			printf("subst var : post while is space or quote\n");
//			l = ft_strchr_space(l);
			r += ft_strlcpy(r, vals[v++], size);			
		}
		else
			*(r++) = *(l++);
	}
	*r = '\0';
	if (*ret)
		ft_free_p((void **)ret);
	*ret = out;
	printf("subst env vars : ret at end : %s\n", *ret);
	return (0);
}
