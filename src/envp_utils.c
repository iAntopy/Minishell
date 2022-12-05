/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 06:08:27 by iamongeo          #+#    #+#             */
/*   Updated: 2022/12/05 07:37:42 by iamongeo         ###   ########.fr       */
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
	int	len;
	int	i;

	if (!msh || !msh->envp || !var || !value)
		return (repport_missing_input(__FUNCTION__));
	envp = msh->envp;
	len = strtab_len(envp);
	new_envp = NULL;
	if (build_new_envp_entry(var, value, &entry) < 0)
		return (repport_malloc_err(__FUNCTION__));
	if (!ft_malloc_p(sizeof(char *) * (len + 2), (void **)&new_envp))
		return (ft_free_p((void **)&entry)
			+ repport_malloc_err(__FUNCTION__));
	i = -1;
	while (envp[++i])
		new_envp[i] = envp[i];
	new_envp[len] = entry;
	new_envp[len + 1] = NULL;
	ft_free_p((void **)&envp);
	msh->envp = new_envp;
	return (0);
}

int	msh_envp_remove_entry(t_msh *msh, char *var)
{
	char	**envp;
	size_t	len;
	int	i;
	
	if (!msh || !msh->envp || !var)
		return (repport_missing_input(__FUNCTION__));
	len = ft_strlen(var);
	i = -1;
	envp = msh->envp;
	while (envp[++i] && ft_strncmp(var, envp[i], len) != 0)
		continue ;
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
		return (repport_missing_input(__FUNCTION__));
	envp_cpy = NULL;
	envp_cpy = strtab_copy(envp);
	if (!envp_cpy)
		return (repport_malloc_err(__FUNCTION__));
	*ret = envp_cpy;
	return (0);
}

char	*msh_getenv(t_msh *msh, char *var)
{
	char	**envp;

	if (!msh || !msh->envp || !var)
		return (NULL);
	printf("msh getenv : entered with var : %s\n", var);
	envp = msh->envp;
	while (*envp)
	{
		if (ft_strnstr(*envp, var, 64))
		{
			printf("msh getenv : match found \n");
			return (ft_strchr(*envp, '=') + 1);
		}
		envp++;
	}
	printf("msh getenv : no match for var %s\n", var);
	return (NULL);
}
/*
int	main(int argc, char **argv, char **envp)
{
	t_msh	msh;

	if (msh_envp_copy(envp, &msh.envp) < 0)
		return (1);
	strtab_print(msh.envp);
	msh_envp_add_entry(&msh, "JINGLE", "JANGLES");
	msh_envp_add_entry(&msh, "HOHOHO", "les cocos");
	strtab_print(msh.envp);
	msh_envp_remove_entry(&msh, "JINGLE");
	strtab_print(msh.envp);
	strtab_clear(&msh.envp);
	return (0);
}
*/
