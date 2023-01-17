/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exec_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 02:36:44 by iamongeo          #+#    #+#             */
/*   Updated: 2023/01/16 14:16:17 by tbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_exec_cmd(t_msh *msh, char *cmd)
{
	(void)msh;
	printf("CHILD %d : executing cmd : %s\n", getpid(), cmd);
	return (0);
}
