/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 01:39:29 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/06 05:24:16 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define MAX_WILDCARD_EXP 1024

static int	test_fit_wildcard_token(const char *wc, const char *str)
{
	while (*wc && *str)
	{
		if (*wc == '*')
		{
			while (*(wc + 1) == '*')
				wc++;
			while (*str && *str != *(wc + 1))
				str++;
		}
		else if (*str == *wc)
			str++;
		else
			break ;
		wc++;
	}
	while (*wc == '*')
		wc++;
	return (!(*wc || *str));
}

static int	contains_wildcard(char *tk)
{
	while (*tk && skip_open_quotes(&tk))
	{
		if (*tk == '*')
			return (1);
		tk++;
	}
	return (0);
}

static int	wildcard_expansion(t_cmd *cmd, char *tk, char **mchs, int *nb_mch)
{
	DIR				*cur_dir;
	struct dirent	*dent;

	if (!tk)
		return (report_missing_input(__FUNCTION__));
	if (!contains_wildcard(tk))
		return (0);
	restore_substring(tk, cmd->job->sc, 1);
	cur_dir = opendir("./");
	dent = readdir(cur_dir);
	while (dent)
	{
		if (dent->d_name[0] != '.')
		{
			if (test_fit_wildcard_token(tk, dent->d_name))
			{
				mchs[(*nb_mch)++] = ft_strdup(dent->d_name);
				if (!mchs[*nb_mch - 1] || *nb_mch > MAX_WILDCARD_EXP)
					return (-1);
			}
		}
		dent = readdir(cur_dir);
	}
	closedir(cur_dir);
	return (0);
}

static int	insert_matches_in_tokens(t_cmd *cmd, int i, char **mchs, int nb_mch)
{
	char	**new_tks;
	int		tlen;

	tlen = strtab_len(cmd->tokens);
	new_tks = NULL;
	if (!ft_calloc_p(sizeof(char *) * (tlen + nb_mch), (void **)&new_tks))
	{
		while (nb_mch--)
			ft_free_p((void **)(mchs + nb_mch));
		return (-1);
	}
	ft_free_p((void **)&cmd->tokens[i]);
	ft_memcpy(new_tks, cmd->tokens, sizeof(char *) * i);
	ft_memcpy(new_tks + i, mchs, sizeof(char *) * nb_mch);
	ft_memcpy(new_tks + i + nb_mch, cmd->tokens + i + 1,
		sizeof(char *) * (tlen - i - 1));
	ft_free_p((void **)&cmd->tokens);
	cmd->tokens = new_tks;
	return (0);
}

int	wildcard_expand_all_tokens(t_cmd *cmd)
{
	int		i;
	int		nb_mch;
	char	*matches[MAX_WILDCARD_EXP];

	i = -1;
	while (cmd->tokens[++i])
	{
		ft_memclear(matches, sizeof(char *) * MAX_WILDCARD_EXP);
		nb_mch = 0;
		if (wildcard_expansion(cmd, cmd->tokens[i], matches, &nb_mch) < 0)
			return (-1);
		if (!nb_mch)
			continue ;
		if (insert_matches_in_tokens(cmd, i, matches, nb_mch) < 0)
			return (-1);
		i += nb_mch - 1;
	}
	return (0);
}
