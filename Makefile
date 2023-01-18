# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/16 01:26:25 by iamongeo          #+#    #+#              #
#    Updated: 2023/01/18 02:41:51 by iamongeo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_DIR		= src/
SRC_B_DIR	= src_bonus/
BLT_DIR		= builtins/

_SRC		=	msh_main.c			\
			job_manager_main.c		\
			meta_chars_utils.c		\
			substitute_env_vars.c		\
			envp_utils.c			\
			error_handling.c		\
			split_cmd_on_pipes.c		\
			job_executor.c			\
			parse_exec_command.c		\
			pipe_utils.c			\
			sig_handlers.c			\
			intercept_builtin_calls.c

_BLT		=	cd.c		\
			echo.c		\
			export.c	\
			unset.c		\
			exit.c		\
			pwd.c		\
			env.c

SRC_M		= $(addprefix $(SRC_DIR), $(_SRC)) $(addprefix $(BLT_DIR), $(_BLT))

OBJ_M		= $(SRC_M:.c=.o)

INCL		= inc/

LIBFT		= libft/libft.a

LIBRD_DIR		=	$(INCL)/librd
LIBRD_FILES		=	libreadline.a libhistory.a
LIBRD_MAKEFILE		=	$(LIBRD_DIR)/Makefile
LIBRD			=	$(addprefix $(LIBRD_DIR)/, $(LIBRD_FILES))

CC		= gcc
CFLAGS		= -Wall -Wextra -Werror

LIBS	= $(LIBFT) -lcurses $(LIBRD)

NAME	= minishell

%.o:		%.c
		-$(CC) $(CFLAGS) -I$(INCL) -c $< -o $@


$(NAME):	$(LIBFT) $(LIBRD_MAKEFILE) $(LIBRD) $(OBJ_M)
		$(CC) $(CFLAGS) -o $(NAME) $(OBJ_M) $(LIBS)

$(LIBRD): 	$(LIBRD_MAKEFILE)
		@$(MAKE) -s -C $(LIBRD_DIR)

$(LIBRD_MAKEFILE):
		@cd $(LIBRD_DIR) && ./configure --silent
$(LIBFT):
		make -C libft/

all:	$(NAME) $(NAME_BONUS) 

clean:
		rm -f $(OBJ_M) $(OBJ_B) $(LIBFT)
		@$(MAKE) -s clean -C $(LIBRD_DIR)

fclean:		clean
		rm -f $(NAME) $(NAME_BONUS)

re:		fclean all
