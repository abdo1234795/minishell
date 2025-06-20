NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes -I$(HOME)/.local/include
LDFLAGS = -L$(HOME)/.local/lib -lreadline

RM = rm -f

SRC = \
	minishell.c minishell_utils.c \
	execution/builtin_tools.c execution/execute.c execution/execute_1.c \
	execution/mini_errors0.c execution/mini_errors1.c \
	execution/pipes_errors.c execution/piping.c execution/piping_1.c \
	execution/redirection.c \
	execution/shell_builts0.c execution/shell_builts1.c execution/shell_builts2.c execution/shell_builts3.c \
	execution/var_tools_0.c execution/var_tools_1.c execution/var_tools_2.c \
	includes/allocs.c includes/cleaning.c includes/env_tools.c \
	includes/handle_signals.c includes/mini_atoi.c includes/mini_itoa.c \
	includes/mini_lib0.c includes/mini_lib1.c includes/mini_lib2.c includes/mini_lib3.c \
	includes/mini_split.c \
	parsing/add_helper1.c parsing/add_helper2.c parsing/add_helper3.c \
	parsing/add_utils1.c parsing/add_utils2.c parsing/add_utils3.c parsing/add_utils4.c parsing/add_utils5.c parsing/add_utils6.c \
	parsing/expand_var.c parsing/expand_var_utils1.c \
	parsing/handle_red_utils1.c parsing/handle_red_utils2.c parsing/handle_red_utils3.c \
	parsing/if_conditions.c parsing/parse.c parsing/parse_helper1.c parsing/parse_helper2.c parsing/parse_helper3.c \
	parsing/parsing.c \
	parsing/read_heredoc_line_util.c \
	parsing/tokenizer.c parsing/tokenizer_1utils.c parsing/tokenizer_2utils.c \
	parsing/tokenizer_3utils.c parsing/tokenizer_4utils.c  parsing/expand_var_utils2.c\
	parsing/tokenizer_fill.c parsing/tokenizer_fill2.c parsing/tokenizer_fill3.c parsing/ft_split.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@

%.o: %.c includes/minishell.h execution/execution.h parsing/parsing.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
