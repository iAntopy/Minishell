/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 08:22:03 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/27 16:49:19 by tbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_synthax_export_cmd(char *cmd, char *cmd_name)
{
	int	i;

	i = 0;
	if (!(ft_isalpha(cmd[0]) || cmd[0] == '_'))
	{
		ft_eprintf("%s: `%s': not a valid identifier\n", cmd_name, cmd);
		return (-1);
	}
	while (cmd[i] != '\0' && cmd[i] != '=')
	{
		if (!(ft_isalnum(cmd[i]) || cmd[i] == '_'))
		{
			ft_eprintf("%s: `%s': not a valid identifier\n", cmd_name, cmd);
			return (-1);
		}
		i++;
	}
	return (0);
}

static int	parse_export_cmd(char *cmd, char **var, char **value)
{
	char	**split;

	if (!ft_strchr(cmd, '='))
		return (-1);
	if (check_synthax_export_cmd(cmd, "export") == -1)
		return (-1);
	split = ft_split(cmd, '=');
	if (!split || !split[0])
		return (report_malloc_err(__FUNCTION__));
	*var = split[0];
	if (!split[1])
		*value = "\0";
	else
		*value = split[1];
	free(split);
	return (0);
}

static int	pre_parse(t_job *job, char **token_tab)
{
	char	*var;
	char	*value;
	char	*existing_value;
	int		i;

	i = 0;
	while (token_tab[++i])
	{
		if (parse_export_cmd(token_tab[i], &var, &value) == 0)
		{
			existing_value = msh_getenv(job->msh, var);
			if (existing_value)
				msh_envp_remove_entry(job->msh, var);
			if (msh_envp_add_entry(job->msh, var, value) < 0)
				return (-1);
			ft_free_p((void **)&var);
			if (value[0] != '\0')
				ft_free_p((void **)&value);
		}
	}
	return (0);
}

static void	no_arg_export_cmd(t_job *job)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	j = 0;
	while (job->msh->envp[i])
	{
		printf("declare -x ");
		while (job->msh->envp[i][j] != '=' && job->msh->envp[i][j] != '\0')
		{
			printf("%c", job->msh->envp[i][j]);
			j++;
		}
		str = ft_strchr(job->msh->envp[i], '=');
		printf("=\"%s\"\n", ++str);
		j = 0;
		i++;
	}
}

int	msh_builtin_export(t_job *job, t_cmd *cmd)
{
	// char	**token_tab;

	// token_tab = tokenize(job, cmd);
	// strtab_print(token_tab);
	if (cmd->tokens == NULL)
		return (-1);
	if (cmd->tokens[1] == NULL)
		no_arg_export_cmd(job);
	if (pre_parse(job, cmd->tokens) < 0)
		return (-1);
	// strtab_clear(&token_tab);
	return (0);
}
