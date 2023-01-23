/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 08:22:03 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/19 21:01:15 by tbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_export_cmd_the_return_of_the_parse(char *cmd)
{
	int	i;

	i = 0;
	if (!(ft_isalpha(cmd[0]) || cmd[0] == '_'))
		return (-1);
	while (cmd[i] != '\0' && cmd[i] != '=')
	{
		if (!(ft_isalnum(cmd[i]) || cmd[i] == '_'))
			return (-1);
		i++;
	}
	return (0);
}

static int	parse_export_cmd(char *cmd, char **var, char **value)
{
	char	**split;

	if (!ft_strchr(cmd, '='))
		return (0);
	if (parse_export_cmd_the_return_of_the_parse(cmd) == -1)
		return (-1);
	split = ft_split(cmd, '=');
	if (!split || !split[0])
		return (repport_malloc_err(__FUNCTION__));
	*var = split[0];
	if (!split[1])
		*value = "\0";
	else
		*value = split[1];
	free(split);
	return (0);
}

int	msh_builtin_export(t_job *job, char *cmd)
{
	char	*var;
	char	*value;
	char	*existing_value;
	char	**token_tab;
	int		ret_val;

	ret_val = 0;
	if (!ft_strchr(cmd, '='))
		return (0);
	token_tab = tokenize(job, cmd);
	strtab_print(token_tab);
	if (token_tab == NULL)
		return (-1);
	if (!ft_strchr(token_tab[1], '=') || parse_export_cmd(token_tab[1], &var, &value) < 0)
	{
		strtab_clear(&token_tab);
		return (-1);
	}
	existing_value = msh_getenv(job->msh, var);
	if (existing_value)
		msh_envp_remove_entry(job->msh, var);
	ret_val = msh_envp_add_entry(job->msh, var, value);
	ft_free_p((void **)&var);
	if (value[0] != '\0')
		ft_free_p((void **)&value);
	strtab_clear(&token_tab);
	return (ret_val);
}
