/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 09:50:34 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/31 11:00:22 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/ioctl.h>
# include "../includes/minishell.h"

typedef struct s_env		t_env;
typedef struct s_cmd		t_cmd;
typedef struct s_malloc		t_malloc;

/**
 * Token types enumeration
 */
typedef enum e_types
{
	pip,
	red,
	file,
	text
}	t_types;

/**
 * Quote state enumeration for parsing
 */
typedef enum e_quote_state
{
	UNQUOTED,
	SINGLE_QUOTED,
	DOUBLE_QUOTED
}	t_quote_state;

/**
 * Token structure for lexical analysis
 */
typedef struct s_token
{
	char			*value;
	t_types			type;
	bool			syn_err;
	bool			heredoc;
	bool			need_expand;
	bool			newline;
	bool			wait_more_args;
	bool			ambiguous;
	struct s_token	*next;
}	t_token;

typedef struct s_var
{
	t_token			*tokens;
	char			*buffer;
	char			*tmp_err;
	int				i;
	int				j;
	char			c;
	int				a;
	int				*p;
	t_quote_state	state;
	bool			has_heredoc;
	bool			wait_more_args;
}	t_var;

typedef struct s_v
{
	t_token			*new_token;
	t_token			*current;
	t_token			*prev_token;
	char			*buff;
	char			*joined;
	bool			quote;
	char			*new_buff;
	char			first;
	char			last;
	char			*tmp;
	char			*expanded_value;
	char			**bib;
	int				i;
	int				s;
	int				t;
	bool			ambiguous;
}	t_v;

/**
 * Redirection types
 */
# define REDIR_IN 1
# define REDIR_OUT 2
# define REDIR_APPEND 3
# define REDIR_HEREDOC 4

typedef struct s_redirection
{
	char					*file;
	int						type;
	int						fd;
	struct s_redirection	*next;
}	t_red;

typedef struct s_cmd
{
	char			*name;
	char			**args;
	int				arg_count;
	int				arg_capacity;
	bool			am;
	t_red			*in;
	t_red			*out;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_pt
{
	t_cmd	*commands;
	t_cmd	*current_cmd;
	t_token	*current;
	t_token	*prev;
	int		s;
	int		tmp_err;
	int		err;
	bool	syn_err;
	char	*err_file;
	int		result;
	int		a;
}	t_pt;

typedef struct s_h_red_v
{
	int				i;
	int				redir_type;
	int				in;
	int				capacity;
	char			**bib;
	int				n;
	char			*line;
	char			*tmp;
	char			**new_bib;
	int				j;
	int				fd;
	int				ss;
	int				result;
	t_token			*file_token;
	t_red			*redir;
	t_red			*current;
}	t_h_red_v;

typedef struct s_sp_var
{
	char		*line;
	t_cmd		*cmds;
	t_env		*env;
	t_malloc	*allocs;
	int			status;
	t_var		*var;
	t_pt		*vpt;
	t_h_red_v	*hrv;
	t_v			*v;
	bool		ambig;
	bool		st_quote;
	bool		st_ambiguous_red;
	bool		st_ambiguous;
	int			tmp;
	int			a;
}	t_sp_var;

typedef struct s_expand
{
	int		i;
	int		j;
	char	*tmp;
	char	*result;
	int		m;
	int		k;
	char	*var_name;
	char	*value;
}	t_expand;

typedef struct s_readline
{
	char	c;
	char	**buf;
	int		*len;
	int		*cap;
}	t_readline;

/* Tokenization functions */
t_token		*tokenize_input(t_sp_var *v);
void		detect_file_tokens(t_token **tokens);
int			validate_syntax(t_token **tokens);
t_cmd		*parse_tokens(t_token *tokens, t_sp_var *v);
t_cmd		*parse(t_sp_var *v);
int			is_token_separator(char c);
int			add_token_with_type(t_sp_var *va);
int			check_operator_validity(char *buffer, t_quote_state *state);
char		*remove_character(t_sp_var *va, char c);
void		add_error_token(t_token **token, t_sp_var *va);
void		check_if_needs_expansion(t_token **tokens, t_quote_state state);
char		*expand_env_vars(char *str, t_sp_var *va);

/* Tokenizer helper functions */
void		first_condtion(t_sp_var *va);
int			second_condition(t_sp_var *va);
int			third_condition(t_sp_var *va);
int			fourth_condition(t_sp_var *va);
int			fill1_fourth(t_sp_var *va);
int			fill2_fourth(t_sp_var *va);
int			fill3_fourth(t_sp_var *va);
int			fill_fill3_fourth(t_sp_var *va);
int			fifth_condition(t_sp_var *va);
int			sixth_condition(t_sp_var *va);

/* Condition functions */
int			first_if(t_sp_var *va);
int			second_if(t_sp_var *va);
int			third_if(t_sp_var *va);
int			fourth_if(t_sp_var *va);
int			while_part1(t_sp_var *va);

/* Tokenizer utils */
void		init_var(t_sp_var *va);
bool		fill_tken_part1(t_var *v);
t_token		*fill_tken_part2(t_var *v);
t_token		*fill_tken_part3(t_var *v);
t_token		*create_new_token(t_token **new_token, char *new_buff,
				t_sp_var *va);
void		init_variable(t_v *v, t_token *tokens);
int			ft_fill_2(t_v *v, t_sp_var *va);
int			ft_fill_1(t_sp_var *va);
int			need_expandd(char *str, t_quote_state *state);

/* Redirection and pipe handling */
int			fill1_red_pip_txt(t_v *v, t_quote_state *state);
void		fill2_red_pip_txt(t_v *v, t_quote_state *state);
void		red_pip_txt(t_v *v, t_quote_state *state);
void		add_expanded_token(t_v *v, t_token **tokens, char *expanded,
				t_sp_var *va);
void		add_token(t_v *v, t_token **tokens);

/* Parsing functions */
t_cmd		*create_new_command(t_sp_var *va);
void		add_argument(t_cmd *cmd, char *arg, t_sp_var *va);
t_red		*create_redirection(char *file, int type, t_sp_var *va);
void		init_vpt(t_token *tokens, t_pt *vpt);
int			ft_detect_syn_err(t_token *tokens, t_sp_var *va);

/* While part functions */
int			while_part_if_1(t_sp_var *va, int *x);
int			while_part_if_2_fill(t_sp_var *va);
int			while_part_if_2(t_sp_var *va, int *x);
void		while_part_print_err(t_sp_var *va, int *x);
void		while_print_err(t_sp_var *va, int *x);
int			while_part(t_sp_var *va);
void		while_part_if_2_fi(t_sp_var *va, int *x);

/* Redirection handling */
int			handle_redirection(t_cmd *cmd, t_token *token, t_sp_var *va,
				int ss);

int			process_heredoc(t_sp_var *va);
int			init_heredoc_buffer(t_sp_var *va);
int			read_heredoc_line(t_sp_var *va);
void		expand_heredoc_line(t_sp_var *va, char **tmp);
int			resize_heredoc_buffer(t_sp_var *va);
int			heredoc_to_temp_file(t_sp_var *va);
int			handle_red_if1(t_cmd *cmd, t_sp_var *va);
int			handle_red_if2(t_cmd *cmd, t_sp_var *va);
int			open_outfile(t_sp_var *va);
void		add_red_file(t_cmd *cmd, t_sp_var *va);
int			red_type(char *value);

/* Token list utilities */
t_token		*last_token(t_token *token);
bool		should_expand_token(t_v *v, t_sp_var *va);
void		init_first_last(t_v *v, t_sp_var *va, char **static_buffer);
void		p_ex_with_buffer(t_v *v, t_sp_var *va, char **static_buffer);
int			p_with_buffer(t_v *v, t_sp_var *va, char **static_buffer);
void		update_quote_state(t_sp_var *va, bool *quote);
t_v			*init_token_vars(t_sp_var *va, bool *quote);
int			prepare_token(t_sp_var *va, t_v *v);

/* Ambiguous token handling */
void		add_anbiguous_token(t_v *v, t_token **tokens);
void		init_last_ambig(t_v *v, t_sp_var *va, int a);
int			ambiguous_nob_1(t_v *v, t_sp_var *va, char **static_buffer);
int			ambiguous_nob_2(t_v *v, t_sp_var *va, char **static_buffer);
void		wait_or_add(t_v *v, t_sp_var *va, char **static_buffer);
void		p_without_buffer_last_part(t_v *v, t_sp_var *va,
				char **static_buffer);
void		p_ex_without_buffer_fill(t_v *v, t_sp_var *va,
				char **static_buffer);
void		p_ex_without_buffer(t_v *v, t_sp_var *va, char **static_buffer);
void		p_without_buffer(t_v *v, t_sp_var *va, char **static_buffer);
bool		ft_ambiguous_red(t_v *v);

/* Additional ambiguous token functions */
void		init_first_last_ambig(t_v *v, t_sp_var *va, char **static_buffer);
void		p_ex_with_buffer_1(t_v *v, t_sp_var *va, char **static_buffer);
int			ambiguous_1(t_v *v, t_sp_var *va, char **static_buffer);
int			ambiguous_2(t_v *v, t_sp_var *va, char **static_buffer);
int			ambiguous_3(t_v *v, t_sp_var *va, char **static_buffer);
int			ambiguous_4(t_v *v, t_sp_var *va, char **static_buffer);
int			ambiguous_0(t_v *v, t_sp_var *va, char **static_buffer);
int			p_ex_with_buffer_2(t_v *v, t_sp_var *va, char **static_buffer);
void		p_no_ex_with_buffer(t_v *v, t_sp_var *va, char **static_buffer);

/* While part utilities */
int			while_ambiguous_part(t_sp_var *va, int *x);
int			while_part_fiill(t_sp_var *va, int *x);
void		p_without_buffer_f(t_v *v, t_sp_var *va, char **static_buffer);

/* Expansion utilities */
int			expand_part1(char *str, t_sp_var *va, t_expand *e);
int			expand_part2(char *str, t_sp_var *va, t_expand *e);
char		*get_var_value(char *var_name, t_env *env, t_sp_var *va);

void		heredoc_signal(int a);
int			return_tesult(int result, int fd);
int			process_heredoc_line(t_sp_var *va, char **tmp, int fd);
int			sig_return(int fd);
int			setup_heredoc_reading(int *fd);

#endif