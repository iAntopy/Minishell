/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 15:54:05 by iamongeo          #+#    #+#             */
/*   Updated: 2022/10/21 22:21:59 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
static char	*return_empty_str(void)
{
	char	*empty;

	empty = NULL;
	if (!ft_malloc_p(sizeof(char), (void **)&empty))
		return (NULL);
	empty[0] = '\0';
	return (empty);
}
*/
char	*ft_strtrim(char const *s1, char const *set)
{
	int	start;
	int	end;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	end = ft_strlen(s1) - 1;
	while ((start < end) && ft_strchr(set, s1[end]))
		end--;
	end++;
	return (ft_substr(s1, start, end - start));
}
/*
char	*ft_strtrim(char const *s1, char const *set)
{
	int	start;
	int	end;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
//	if (!s1[start])
//		return (return_empty_str());
	end = ft_strlen(s1) - 1;
//	end = start;
//	while (s1[end])
//		end++;
	while ((start < end) && ft_strchr(set, s1[end]))
		end--;
	end++;
//	if (start == end)
//		return (return_empty_str());
	return (ft_substr(s1, start, end - start));
}

int	main(int argc, char **argv)
{
	char *str;

	if (argc < 2)
		return (0);

	str = ft_strtrim(argv[1], "tulas");
	printf("res : %s\n", str);
	return (0);
}
*/
