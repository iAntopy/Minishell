/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 19:42:59 by tbeaudoi          #+#    #+#             */
/*   Updated: 2023/01/19 20:01:02 by tbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**tokenize(t_job *job, char *cmd)
{
	char	**tokens;

	tokens = NULL;
	tokens = ft_split(cmd, ' ');
	if (!tokens)
		return (NULL);
	restore_substrings_in_tab(tokens, job->sc);
	return (tokens);
}