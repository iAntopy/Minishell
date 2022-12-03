# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iamongeo <marvin@42quebec.com>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/16 17:13:42 by iamongeo          #+#    #+#              #
#    Updated: 2022/11/16 17:15:39 by iamongeo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_DIR		= src/
LIBFT_DIR	= ../libft/

_SRC	=	pipex_main \
			pipe_utils	\
			validator_funcs \
			error_handling	\
			get_here_doc_input

SRC			= $(addprefix $(SRC_DIR), $(_SRC))

SRCS		= $(addsuffix .c, $(SRC))

OBJS		= $(SRCS:.c=.o)

INCLS		= includes/

LIBFT		= $(LIBFT_DIR)libft.a

$(OBJS):	DEF_CMD_MAX := -D CMD_MAX=256

CC			= gcc
CFLAGS		= -Wall -Wextra -Werror

NAME		= pipex

%.o:		%.c
		-$(CC) $(CFLAGS) $(DEF_CMD_MAX) -I$(INCLS) -c $< -o $@

$(NAME):	$(LIBFT) $(OBJ_M)
		$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT)

$(LIBFT):
		make -C $(LIBFT_DIR)

all:		$(NAME)

clean:
		rm -f $(OBJS)

fclean:		clean
		rm -f $(NAME)

re:			fclean all
