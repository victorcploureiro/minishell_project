/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bnespoli <bnespoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 18:57:15 by bnespoli          #+#    #+#             */
/*   Updated: 2025/01/16 16:42:15 by bnespoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern volatile sig_atomic_t	g_status;

void	ft_fork_and_execute(char *cmd, t_vector *phrase, char **vars,
		t_shell *shell)
{
	pid_t	pid;
	char	**argv_exec;

	pid = fork();
	if (!pid)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		ft_status_here(FORK, 1);
		rl_clear_history();
		argv_exec = ft_build_argv_exec(phrase);
		if (access(cmd, F_OK) < 0)
			ft_error(cmd, NULL, "No such file or directory", ENOENT);
		else if (access(cmd, X_OK) < 0)
			ft_error(cmd, NULL, "Permission denied", EACCES);
		else if (execve(cmd, argv_exec, vars) < 0)
			ft_error(cmd, NULL, strerror(errno), errno);
		ft_free_matrix(argv_exec);
		ft_free_matrix(vars);
		ft_clear(shell);
		free(cmd);
		exit(ft_status(-1));
	}
	free(cmd);
	ft_pid_status(pid);
}

static void	ft_do(t_vector *phrase, t_shell *shell)
{
	char	*cmd;
	char	**vars;

	cmd = ft_strdup(ft_value(phrase, 0, 0));
	vars = ft_env_export(shell->envp_dict);
	if (cmd && (cmd[0] == '/' || ft_strncmp(cmd, "./", 2) == 0))
		ft_fork_and_execute(cmd, phrase, vars, shell);
	else
	{
		shell->path = ft_getenv(shell->envp_dict, "PATH");
		if (shell->path_splitted)
			ft_free_matrix(shell->path_splitted);
		shell->path_splitted = ft_split(shell->path, ':');
		free(cmd);
		cmd = ft_get_pathname(shell->path_splitted, ft_value(phrase, 0, 0));
		if (cmd && (cmd[0] == '/' || ft_strncmp(cmd, "./", 2) == 0))
			ft_fork_and_execute(cmd, phrase, vars, shell);
		else
		{
			ft_error(cmd, NULL, "command not found", ENOENT);
			free(cmd);
		}
	}
	ft_free_matrix(vars);
}

static void	ft_or_and(t_node *root, t_shell *shell)
{
	if (root->type & OR)
	{
		ft_execution(root->left, shell);
		if (ft_status(-1) > 0)
			ft_execution(root->right, shell);
	}
	else
	{
		ft_execution(root->left, shell);
		if (ft_status(-1) == 0)
			ft_execution(root->right, shell);
	}
}

void	ft_execution_aux(t_node *root, t_shell *shell)
{
	ft_expand_before_exec(root, shell);
	if (root->phrase->size == 0)
		return ;
	if (ft_builtins(root, shell))
		return ;
	if (root->str && root->str[0] == '(')
	{
		ft_eliminate_ch_corner(root->str);
		ft_exec_brackets(root, shell);
	}
	else if (root->str && root->fd != -1)
		ft_do(root->phrase, shell);
}

void	ft_execution(t_node *root, t_shell *shell)
{
	if (!root)
		return ;
	if (root->type & OR_AND)
		ft_or_and(root, shell);
	else if (root->type & PIPE)
		ft_pipe(root, shell);
	else if (root->type & REDALL)
		ft_exec_redirects(root, shell);
	else if (root->left)
		ft_execution(root->left, shell);
	else if (root->right)
		ft_execution(root->right, shell);
	else if (root->type == 0 && root->phrase)
	{
		ft_execution_aux(root, shell);
	}
}
