/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bnespoli <bnespoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 17:27:17 by vcarrara          #+#    #+#             */
/*   Updated: 2025/01/16 17:19:40 by bnespoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/libft.h"
# include "../lib/printf/ft_printf.h"
# include <curses.h>
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>
# include <limits.h>

# define FT_VECTOR_INITIAL_CAPACITY	16
# define TEMP "/tmp/heredoc"
# define HERE_DOC 0
# define FORK	1
# define PROMPT	2
# define PATH_MAX 4096

typedef enum e_token
{
	WORD = 0,
	REDIN = 1 << 0,
	REDOUT = 1 << 1,
	REDIRECTS = 3,
	HEREDOC = 1 << 2,
	APPEND = 1 << 3,
	REDALL = 15,
	OR = 1 << 4,
	AND = 1 << 5,
	OR_AND = 48,
	PIPE = 1 << 6,
	ALLEXRED = 112,
	ALLEXHEREDOC = 121,
	ALL = 127,
	OPEN_BRACKET = 1 << 7,
	CLOSE_BRACKET = 1 << 8,
	ALL_W_BRACKET = 1023,
	EXEC	= 1 << 9
}					t_token;

typedef struct s_status
{
	int	_heredoc;
	int	_fork;
	int	_prompt;
}	t_status;

typedef struct s_vector
{
	void			**values;
	unsigned long	size;
	unsigned long	capacity;
}					t_vector;

typedef enum e_side
{
	LEFT = 0,
	RIGHT = 1
}					t_size;

typedef struct s_index
{
	int				i;
	int				j;
	int				m;
}					t_index;

typedef struct s_parse
{
	char			*prompt;
	char			*prompt_arranged;
	t_index			*idx;
	t_vector		*phrase;
	t_vector		*phrase_grammar;
	char			**prompt_splitted;
}					t_parse;

typedef struct s_node
{
	char			*str;
	char			*str_not_expanded;
	int				type;
	struct s_node	*left;
	struct s_node	*right;
	t_vector		*phrase;
	int				fd;
}					t_node;

typedef struct s_shell
{
	t_parse			*parse;
	char			**envp;
	t_vector		*envp_dict;
	t_node			*root;
	char			*path;
	char			**path_splitted;
	int				status;
	char			*str;
}					t_shell;

// File: Read_inputs
char				*ft_readline(t_shell *shell);
void				ft_return_terminal(int ret);
void				ft_to_execute(char *str, t_shell *shell);

// File Parse
char				**ft_parser(t_parse *parse);
int					ft_aux_parse_char(char *ch, t_parse *parse, char *prompt);
int					ft_parse_char(t_parse *parse, char *prompt);
int					ft_valid_brackets_str(char *str);
void				ft_arranging_prompt(t_parse *parse);
void				ft_parse_brackets(t_parse *parse, char *prompt, int count);
void				ft_parse_quotes(t_parse *parse, char *prompt, char *signal);
int					ft_error_brackets(int st);
int					ft_error_brackets(int st);

// File Tokenizer
char				*ft_expand(char *str, t_shell *shell);
char				*ft_parse_expand(char *str, t_shell *shell);
char				*ft_parse_expand_heredoc(char *s, t_shell *shell);
int					ft_set_token(char *str);
t_vector			*ft_construct_phrase(char **split, t_shell *shell);
void				ft_free_phrase(t_vector *phrase);
void				ft_split_expand(char *str, t_vector *split);
char				*ft_eliminate_signal(char *s, t_shell *shell);
char				ft_signal(t_vector *vector, size_t *i, char signal);
char				*ft_signal_str(t_vector *vector, size_t *i);
unsigned long int	ft_index(unsigned long int st);
int					ft_empty_str(t_vector *vector);

// File Grammar
int					ft_grammar_rules(t_vector *phrase);
void				ft_order_redall(t_vector *vector);
int					ft_bracks_inside_empty(t_vector *phrase);
int					ft_before_brackets(t_vector *phrase, char *str, size_t pos);

// File Utils
int					ft_count_chr(char *str, char ch);
int					ft_count_matrix(char **str);
int					ft_status_here(int where, int st);
void				ft_change_sub_space(char **split);
void				ft_free_matrix(char **matrix);
void				ft_replace_char_between_signal(char *str,
						char signal, char old, char new_ch);
char				ft_is_space(char c);
// File grammar utils
int					error(int status);
int					ft_pos_token_back(t_vector *phrase, int token);

//File AST
t_node				*ft_ast(t_vector *phrase);
void				ft_eliminate_ch_corner(char *str);

// File Vector
bool				ft_vector_empty(const t_vector *vector);
bool				ft_vector_resize(t_vector *vector,
						unsigned long new_capacity);
int					ft_value_int(const t_vector *vector,
						unsigned long i, unsigned long j);
t_vector			*ft_vector_create(void);
t_vector			*ft_vector_slice_right(t_vector *vector, unsigned long pos);
unsigned long		ft_vector_insert(t_vector *vector, unsigned long position,
						void *value);
unsigned long		ft_vector_size(const t_vector *vector);
void				*ft_realloc(void *ptr,
						size_t original_size, size_t new_size);
void				*ft_value(const t_vector *vector,
						unsigned long i, unsigned long j);
void				*ft_vector_at(const t_vector *vector, unsigned long index);
void				ft_vector_erase(t_vector *vector, unsigned long position);
void				ft_vector_free(t_vector *vector);
void				ft_vector_init(t_vector *vector);
void				ft_vector_pop_back(t_vector *vector);
void				ft_vector_push_back(t_vector *vector, void *value);

//File Envp
t_vector			*ft_build_line_envp(char *str);
t_vector			*ft_envp_dict(char **envp);
void				ft_envp(t_shell *shell);
void				ft_free_envp_dict(t_vector *envp_dict);
void				ft_free_line_envp(t_vector *line_dict);

//File Pathname
char				*ft_get_pathname(char **matrix, char *str);

//File execution
extern volatile sig_atomic_t	g_status;
int					ft_status(int st);
void				ft_exec_brackets(t_node *root, t_shell *shell);
void				ft_exec_redirects(t_node *root, t_shell *shell);
void				ft_execution(t_node *root, t_shell *shell);
void				ft_pipe(t_node *root, t_shell *shell);

//File exec_utils.c
void				ft_close_fd(int *fd);
void				ft_change_fds(int fd, int new);
void				ft_pid_status(pid_t pid);
void				ft_pid_status_without_flobal(pid_t pid);
void				ft_expand_before_exec(t_node *root, t_shell *shell);
char				**ft_build_argv_exec(t_vector *phrase);
int					ft_builtins(t_node *root, t_shell *shell);
void				ft_clean_vector(t_vector *vector);

//File Heredoc
int					ft_open_heredoc(t_node *root, t_shell *shell);
long				ft_count(void);
void				ft_end_heredoc(int infile, char *gnl);
char				*ft_heredoc(t_node *root, t_shell *shell);
char				*ft_limiter(t_node *root, t_shell *shell);

//File Signal
void				ft_init_signal(void);
void				ft_handle_sigint(int signal);
void				ft_sigquit(void);
int					ft_get_return_value(int status);

// File Error
void				ft_clear(t_shell *shell);
void				ft_error(char *cmd, char *flag, char *msg, int status);
void				ft_close_open_fds(void);
void				ft_free_shell(t_shell *shell);
void				ft_free_constructed_phrase(t_vector *phrase);
void				ft_clear_vector(t_vector *vector);

// Builtins
char				**ft_env_export(const t_vector *vars);
void				ft_builtin_cd(t_shell *shell, t_vector *cmd);
void				ft_builtin_echo(t_shell *shell, t_vector *cmd);
void				ft_builtin_env(t_shell *shell, t_vector *phrase);
void				ft_builtin_exit(t_shell *shell, t_vector *cmd);
void				ft_builtin_export(t_shell *shell, t_vector *cmd);
void				ft_builtin_pwd(t_shell *shell, t_vector *cmd);
void				ft_builtin_unset(t_shell *shell, t_vector *cmd);
void				ft_env_add(t_vector *vars, char *name, char *data);
void				ft_env_delete(t_vector *vars, char *name);
int					ft_atoll_check(char *str, bool *error);
bool				ft_check_limits(int signal,
						unsigned long long num, bool *error);

//Utils
char				*ft_strjoinsep(char *s1, char *s2, unsigned int sep);
char				*ft_getenv(t_vector *env, char *name);
int					ft_is_valid_name(char *name);
int					ft_strcmp(char *s1, char *s2);
void				ft_freesplit(char **arr);
void				ft_sort_vars(char **argv, int minor);
void				ft_clear_ast(t_node	*root);

//Clear functions
void				ft_freephrase(t_vector *vector);
void				ft_freephrase_2(t_vector *vector);
void				ft_inside_ast(t_node *root);
void				ft_clear_pipe(t_shell *shell);
void				ft_clear_brackets(t_shell *shell);

#endif
