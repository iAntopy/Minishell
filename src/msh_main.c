/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 19:16:03 by iamongeo          #+#    #+#             */
/*   Updated: 2022/11/16 20:15:03 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	msh_clear(t_msh *msh, int exit_code)
{
	if (msh->rawline)
		ft_free_p((void **)&msh->rawline);
	rl_clear_history();
	return (exit_code);
}

int	msh_parse_rawline(t_msh *msh)
{
	char	*parsed_str;

	parsed_str = ft_strtrim_space(msh->rawline);
	if (!parsed_line)
		return (-1);
	ft_free_p((void **)&msh->rawline);
	msh->rawline = parsed_str;
	
}

int	msh_init(t_msh *msh)
{
	msh->path = 
}

int	main(int argc, char **argv, char **envp)
{
	t_msh	msh;
	int		i;
	
	ft_memclear(&msh, sizeof(t_msh));
	if (msh_init(&&msh) < 0)
		return (msh_clear(&msh, E_MSH_INIT));
	i = -1;
	while (++i < 5)
	{
		if (msh->rawline)
			msh_clear(&msh, E_RAWLINE_CLR);

		msh.rawline = readline(READLINE_PROMPT);
		if (msh->rawline && msh->rawline[0] && msh->rawline[0] != '\n')
			add_history(msh->rawline);
		// PARSE_RAWLINE
		//
		////////////
		ft_free_((void **)&msh->rawline);
	}
	return (msh_clear(&msh, EXIT_SUCCESS));
}
