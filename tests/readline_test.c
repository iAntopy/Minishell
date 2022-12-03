/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_test.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 01:38:21 by iamongeo          #+#    #+#             */
/*   Updated: 2022/11/16 19:12:34 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main()
{
	char	*rd_line;
	int	i;

	i = -1;
	while (++i < 4)
	{
		rd_line = readline("minishell > ");
		if (rd_line && rd_line[0] && rd_line[0] != '\n')
			add_history(rd_line);
		ft_printf("prompt line received : %s\n", rd_line);
		ft_free_p((void **)&rd_line);
//		rl_on_new_line();
	}
	rl_clear_history();
	return (0);
}
