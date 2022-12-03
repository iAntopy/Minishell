/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spaceout_meta_chars.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 16:47:43 by iamongeo          #+#    #+#             */
/*   Updated: 2022/12/02 21:11:49 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static const char	*meta_chars[8] = {"<<", ">>", "<", ">", "||", "|", "&&", NULL};
static const int	meta_lens[7] = {2, 2, 1, 1, 2, 1, 2};

int	is_meta_char(char *c, int *len)
{
	int	i;

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

static size_t	spaced_meta_chars_size_counter(char *str)
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
			while (str[i] && str[i] != quote_switch)
				i++;
		}
		else if (!quote_switch && is_meta_char(str + i, NULL))
			extra_spaces += 2;
	}
	return (i + extra_spaces);
}

char	*spaceout_meta_chars(char *str)
{
	char	*ret;
	char	*r;
	size_t	size;
	char	quote_switch;
	size_t	i;
	int		meta_len;

	if (!str)
		return (NULL);
	size = spaced_meta_chars_size_counter(str);
	if (!ft_malloc_p(sizeof(char) * (size + 1), (void **)&ret))
		return (NULL);
	i = -1;
	quote_switch = 0;
	r = ret;
	while (str[++i])
	{
		if (str[i] == '\"' || str[i] == '\'')
			quote_switch = str[i] * (quote_switch == 0);
		if (!quote_switch && is_meta_char(str + i, &meta_len))
		{
			*(r++) = ' ';
			ft_memcpy(r, str + i, meta_len);
			r += meta_len;
			str += meta_len - 1;
			*(r++) = ' ';
		}
		else
			*(r++) = str[i];
	}	
	return (ret);
}

int	main(int argc, char **argv)
{
	char	*ret;

	if (argc < 2)
		return (1);
	ret = NULL;
	ret = spaceout_meta_chars(argv[1]);
	printf("input str : %s\n", argv[1]);
	printf("spaced out  str : %s\n", ret);
	return (0);
}
