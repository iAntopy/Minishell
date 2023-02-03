/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 06:08:27 by iamongeo          #+#    #+#             */
/*   Updated: 2023/02/02 21:05:15 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	build_new_envp_entry(char *var, char *value, char **ret)
{
	char	*entry;
	char	*e;

	entry = NULL;
	if (!ft_malloc_p(sizeof(char) * (ft_strlen(var) + ft_strlen(value) + 2),
			(void **)&entry))
		return (-1);
	e = entry;
	while (*var)
		*(e++) = *(var++);
	*(e++) = '=';
	while (*value)
		*(e++) = *(value++);
	*e = '\0';
	*ret = entry;
	return (0);
}

int	msh_envp_add_entry(t_msh *msh, char *var, char *value)
{
	char	**envp;
	char	**new_envp;
	char	*entry;
	int		len;
	int		i;

	if (!msh || !msh->envp || !var || !value)
		return (report_missing_input(__FUNCTION__));
	envp = msh->envp;
	len = strtab_len(envp);
	new_envp = NULL;
	if (build_new_envp_entry(var, value, &entry) < 0)
		return (report_malloc_err(__FUNCTION__));
	if (!ft_malloc_p(sizeof(char *) * (len + 2), (void **)&new_envp))
		return (ft_free_p((void **)&entry)
			+ report_malloc_err(__FUNCTION__));
	i = -1;
	while (envp[++i])
		new_envp[i] = envp[i];
	new_envp[i] = entry;
	new_envp[i + 1] = NULL;
	ft_free_p((void **)&envp);
	msh->envp = new_envp;
	return (0);
}

int	msh_envp_remove_entry(t_msh *msh, char *var)
{
	char	**envp;
	size_t	len;
	int		i;

	if (!msh || !msh->envp || !var)
		return (report_missing_input(__FUNCTION__));
	if (!(*var))
		return (0);
	len = ft_strlen(var);
	i = 0;
	envp = msh->envp;
	while (envp[i] && ft_strncmp(var, envp[i], len) != 0)
		i++;
	if (!envp[i])
		return (0);
	ft_free_p((void **)envp + i);
	while (envp[++i])
		envp[i - 1] = envp[i];
	envp[i - 1] = NULL;
	return (0);
}

int	msh_envp_copy(char **envp, char ***ret)
{
	char	**envp_cpy;

	if (!envp || !ret)
		return (report_missing_input(__FUNCTION__));
	envp_cpy = NULL;
	envp_cpy = strtab_copy(envp);
	if (!envp_cpy)
		return (report_malloc_err(__FUNCTION__));
	*ret = envp_cpy;
	return (0);
}

char	*msh_getenv(t_msh *msh, char *var)
{
	char	**envp;
	int		i;

	if (!msh || !msh->envp || !var)
		return ("");
	if (!var[0])
		return ("");
	if (*var == '?')
		return (msh->nbr_buff);
	envp = msh->envp;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, ft_strlen(var)) == 0)
			return (ft_strchr(envp[i], '=') + 1);
		i++;
	}
	return ("");
}
