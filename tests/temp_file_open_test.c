/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temp_file_open_test.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 19:55:23 by iamongeo          #+#    #+#             */
/*   Updated: 2022/12/05 00:07:35 by iamongeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#define SPILL_SIZE 4096

void	spill_the_beans(int fdin, int fdout)
{
	char	buff[SPILL_SIZE + 1];
	ssize_t	nc;

//	printf("starting spill from %d to %d\n", fdin, fdout);
	nc = 1;
	while (nc)
	{
		nc = read(fdin, buff, SPILL_SIZE);
		buff[nc] = '\0';
//		printf("nc = %d : %s", nc, buff);
		write(fdout, buff, nc);
	}
}

static int	gen_tmp_filename(char *buff)
{
	static int	tmp_count;
	size_t		id;
	size_t		len;

	if (!buff)
		return (-1);
	while (1)
	{
		id = getpid();
		id = (id * 115249 * (tmp_count + 1)) % (27361 + 19 * id);
		len = ft_strlcpy(buff, ".tmp", PATH_MAX);
		len += ft_putnbr_buff(buff + len, id);
		buff[len] = '\0';
		if (access(buff, F_OK) != 0)
			break ;
		tmp_count++;
	}
	return (0);
}

int	reset_file(char *filename, int *fd, int mode)
{
	if (!fd)
		return (-1);
	close(*fd);
	*fd = open(filename, mode);
	if (*fd < 0)
		return (-1);
	return (0);
}

int	main(int argc, char **argv)
{
	int	fdin;
	int	temp_fd;
	int	fdout;
	char	tmp_name[PATH_MAX];
	ssize_t	delta_time;

	if (argc < 3)
		return (1);
	fdin = open(argv[1], O_RDONLY);
	if (fdin < 0)
	{
		printf("infile open failed\n");
		return (2);
	}
	if (gen_tmp_filename(tmp_name) < 0)
	{
		printf("gen_tmp_filename failed \n");
		return (3);
	}
	printf("tmmp filename : %s\n", tmp_name);
	temp_fd = open(tmp_name, O_RDWR | O_CREAT, 0644);
	if (temp_fd < 0)
	{
		printf("temp file open failed\n");
//		printf("%s\n", strerror(errno));
		perror("Oh non ...");
		return (4);
	}
	fdout = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fdout < 0)
	{
		printf("outfile open failed\n");
		return (5);
	}
	ft_deltatime_usec();
	spill_the_beans(fdin, temp_fd);
	close(fdin);
	reset_file(tmp_name, &temp_fd, O_RDWR);
	spill_the_beans(temp_fd, fdout);
	close(temp_fd);
	if (unlink(tmp_name) < 0)
		printf("unlink(%s) returned -1\n", tmp_name);
	close(fdout);	
	delta_time = ft_deltatime_usec();
	printf("delta time : %zd\n", delta_time);
	return (0);
}
