# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iamongeo <iamongeo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/16 01:26:25 by iamongeo          #+#    #+#              #
#    Updated: 2022/12/05 06:30:38 by iamongeo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_DIR		= src/
SRC_B_DIR		= src_bonus/

_SRC		=	msh_main.c			\
			job_manager_main.c		\
			meta_chars_utils.c		\
			substitute_env_vars.c		\
			envp_utils.c			\
			error_handling.c		\

SRC_M		= $(addprefix $(SRC_DIR), $(_SRC))
#SRC_B		= $(addprefix $(SRC_B_DIR), $(_SRC_B))
#SRC_RAND	= $(addprefix $(SRC_DIR), randint_gen.c)

OBJ_M		= $(SRC_M:.c=.o)
#OBJ_B		= $(SRC_B:.c=.o)

INCL		= inc/

LIBFT		= libft/libft.a

CC		= gcc
CFLAGS		= -Wall -Wextra -Werror -lreadline -ltermcap

NAME		= minishell

%.o:		%.c
		-$(CC) $(CFLAGS) -I$(INCL) -c $< -o $@

$(NAME):	$(LIBFT) $(OBJ_M)
		$(CC) $(CFLAGS) -o $(NAME) $(OBJ_M) $(LIBFT)

$(LIBFT):
		make -C libft/

all:		$(NAME) $(NAME_BONUS)

clean:
		rm -f $(OBJ_M) $(OBJ_B) $(LIBFT)

fclean:		clean
		rm -f $(NAME) $(NAME_BONUS)

re:		fclean all
