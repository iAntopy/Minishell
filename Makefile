# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/16 01:26:25 by iamongeo          #+#    #+#              #
#    Updated: 2023/01/12 21:39:17 by tbeaudoi         ###   ########.fr        #
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
#SRC_B		= $(addprefix $(SRC_B_DIR), $(_SRC_B))
#SRC_RAND	= $(addprefix $(SRC_DIR), randint_gen.c)

OBJ_M		= $(SRC_M:.c=.o)
#OBJ_B		= $(SRC_B:.c=.o)

INCL		= inc/

LIBFT		= libft/libft.a

#READDIR		= readline-8.2/
#READLIB		= $(READDIR)libreadline.a
#READMAKE	= $(READDIR)Makefile

LIBRD_DIR		=	inc/librd
LIBRD_FILES		=	libreadline.a \
					libhistory.a
LIBRD_MAKEFILE	=	$(addprefix $(LIBRD_DIR)/, Makefile)
LIBRD			=	$(addprefix $(LIBRD_DIR)/, $(LIBRD_FILES))

CC		= gcc
CFLAGS		= -Wall -Wextra -Werror #-lreadline #-ltermcap

LIBS	= $(LIBFT) -lcurses $(LIBRD)#-lreadline

NAME	= minishell

%.o:		%.c
		-$(CC) $(CFLAGS) -I$(INCL) -c $< -o $@ #$(READDIR)


$(NAME):	$(LIBFT) $(LIBRD_MAKEFILE) $(LIBRD) $(OBJ_M) #$(READLIB)
		$(CC) $(CFLAGS) -o $(NAME) $(OBJ_M) $(LIBS) #$(READLIB)

$(LIBRD): 			$(LIBRD_MAKEFILE)
					@$(MAKE) -s -C $(LIBRD_DIR)

$(LIBRD_MAKEFILE):
					@cd $(LIBRD_DIR) && ./configure --silent

					
$(LIBFT):
		make -C libft/

#$(READDIR):
#		tar -xf --keep-newer-files readline-8.2.tar.gz

#$(READMAKE):	$(READDIR)
#		cd $(READDIR); ./configure

#$(READLIB):	$(READMAKE)
#		make -C $(READDIR)
#		cp $(READDIR)*.h $(INCL)

all:	$(NAME) $(NAME_BONUS) 

clean:
		rm -f $(OBJ_M) $(OBJ_B) $(LIBFT)
		@$(MAKE) -s clean -C $(LIBRD_DIR)

fclean:		clean
		rm -f $(NAME) $(NAME_BONUS)

re:		fclean all
