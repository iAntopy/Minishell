/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 07:27:37 by iamongeo          #+#    #+#             */
/*   Updated: 2022/12/04 07:29:29 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	repport_missing_input(const char *fn)
{
	if (fn)
		ft_eprintf("%s : missing input\n", fn);
	return (-1);
}

int	repport_malloc_err(const char *fn)
{
	if (fn)
		ft_eprintf("%s : malloc error\n", fn);
	return (-1);
}
