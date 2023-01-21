# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tbeaudoi <tbeaudoi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/16 01:26:25 by iamongeo          #+#    #+#              #
<<<<<<< HEAD
#    Updated: 2023/01/27 08:05:01 by iamongeo         ###   ########.fr        #
=======
#    Updated: 2023/01/20 23:36:08 by iamongeo         ###   ########.fr        #
>>>>>>> Added validate_syntax(), tokenize() and apply_all_redirections() functions. validate_syntax reports error when meta characters are missing the args they need to work. tokenize transforms the char **pipe_split member of the job struct by spliting it on spaces and restoring the quoted substrings substitution characters to spaces. apply_all_redirections scans the list of commands token's to find redirection meta chars and opens the files if valid and closes the previous ones while checking validity and permissions. All parsing and redirections happen before forking now.
#                                                                              #
# **************************************************************************** #

SRC_DIR		= src/
SRC_B_DIR	= src_bonus/
BLT_DIR		= builtins/

_SRC		=	msh_main.c			\
			job_manager_main.c		\
			meta_chars_utils.c		\
			substitute_env_vars.c		\
			syntax_validator.c		\
			envp_utils.c			\
			error_handling.c		\
			split_cmd_on_pipes.c		\
			job_executor.c			\
			parse_exec_command.c		\
			pipe_utils.c			\
			sig_handlers.c			\
			intercept_builtin_calls.c	\
			tokenizer_redirector.c

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
#		@$(MAKE) -s clean -C $(LIBRD_DIR)

fclean:		clean
		rm -f $(NAME) $(NAME_BONUS)

re:		fclean all
