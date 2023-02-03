/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meta_chars_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 16:47:43 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/02 23:46:55 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_meta_char(char *c, int *len)
{
	const int	meta_lens[8] = {2, 2, 1, 1, 2, 1, 2, 1};
	const char	*meta_chars[9] = {
		"<<", ">>", "<", ">", "||", "|", "&&", "*", NULL};
	int			i;

	if (len)
		*len = 0;
	i = -1;
	while (meta_chars[++i])
	{
		if (ft_strncmp(c, meta_chars[i], meta_lens[i]) == 0)
		{
			if (len)
				*len = meta_lens[i];
			return (meta_lens[i]);
		}
	}
	return (0);
}

int	contains_meta_char(char *str)
{
	while (*str)
		if (ft_strchr(_META_CHARS, *(str++)))
			return (1);
	return (0);
}

static size_t	spaced_size(char *str)
{
	char	quote_switch;
	size_t	i;
	size_t	extra_spaces;
	int		mlen;

	quote_switch = '\0';
	extra_spaces = 0;
	i = -1;
	while (str[++i])
	{
		if (str[i] == '\"' || str[i] == '\'')
		{
			quote_switch = str[i++];
			extra_spaces += 2;
			while (str[i] && str[i] != quote_switch)
				i++;
			if (!str[i])
				return (0);
		}
		else if (!quote_switch && is_meta_char(str + i, &mlen))
			extra_spaces += 2 + mlen;
	}
	return (sizeof(char) * (i + extra_spaces + 1));
}

static int	parse_single_meta_char(char **r_p, char **str_p, int *meta_len)
{
	char	*r;
	char	*str;

	r = *r_p;
	str = *str_p;
	*(r++) = ' ';
	r = ft_memcpy(r, str, *meta_len) + *meta_len;
	if (*str == '|' || *str == '&')
		*(r++) = ' ';
	str += *meta_len;
	while (*str && ft_isspace(*str))
		str++;
	*r_p = r;
	*str_p = str;
	return (0);
}

// Assumes that str contains meta chars as validated by contains_meta_char(str).
int	spaceout_meta_chars(t_job *job)
{
	char	*r;
	char	*p;
	char	quote_switch;
	int		mlen;

	job->parsed2 = NULL;
	if (!job->parsed
		|| !ft_malloc_p(spaced_size(job->parsed), (void **)&job->parsed2))
		return (report_jm_mlc_err(__FUNCTION__));
	quote_switch = 0;
	p = job->parsed;
	r = job->parsed2;
	while (*p)
	{
		if (*p == '\"' || *p == '\'')
			quote_switch = (*p) * (quote_switch == '\0');
		if (!quote_switch && is_meta_char(p, &mlen))
			parse_single_meta_char(&r, &p, &mlen);
		else
			*(r++) = *(p++);
	}
	*r = '\0';
	return (0);
}
