/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meta_chars_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 16:47:43 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/01 07:07:15 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_meta_char(char *c, int *len)
{
	static const int	meta_lens[8] = {2, 2, 1, 1, 2, 1, 2, 1};
	static const char	*meta_chars[9] = {
		"<<", ">>", "<", ">", "||", "|", "&&", "*", NULL};
	int					i;

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


	quote_switch = '\0';
	extra_spaces = 0;
	i = -1;
	while (str[++i])
	{
		if (str[i] == '\"' || str[i] == '\'')
		{
			quote_switch = str[i++];
			extra_spaces++;
			while (str[i] && str[i] != quote_switch)
				i++;
			if (!str[i])
				return (0);
		}
		else if (!quote_switch && is_meta_char(str + i, NULL))
			extra_spaces += 2;
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
//	if (is_meta_char(str, meta_len))
//		return (report_parsing_error(__FUNCTION__, str, *meta_len));
	*r_p = r;
	*str_p = str;
	return (0);
}

// Assumes that str contains meta chars as validated by contains_meta_char(str).
int	spaceout_meta_chars(char *str, char **ret)
{
	size_t	size;
	char	*r;
	char	quote_switch;
	int		meta_len;

	*ret = NULL;
	size = spaced_size(str);
	if (!size)
		return (report_unclosed_quotes());
	if (!str || !ret || !ft_malloc_p(size, (void **)ret))
		return (report_jm_mlc_err(__FUNCTION__));
	quote_switch = 0;
	r = *ret;
	while (*str)
	{
		if (*str == '\"' || *str == '\'')
			quote_switch = (*str) * (quote_switch == '\0');
		if (!quote_switch && is_meta_char(str, &meta_len)
			&& parse_single_meta_char(&r, &str, &meta_len) < 0)
			return (ft_free_p((void **)ret) - 2);
		else
			*(r++) = *(str++);
	}
	*r = '\0';
	return (0);
}
