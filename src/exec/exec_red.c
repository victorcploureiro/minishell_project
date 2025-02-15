/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_red.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bnespoli <bnespoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:57:13 by bnespoli          #+#    #+#             */
/*   Updated: 2025/01/16 16:34:38 by bnespoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_close_fd_tmp(const int *fd)
{
	dup2(fd[1], STDOUT_FILENO);
	dup2(fd[0], STDIN_FILENO);
	close(fd[1]);
	close(fd[0]);
}

static void	ft_dup_right_flag(t_node *root)
{
	if (root->type & (REDOUT | APPEND))
		ft_change_fds(root->fd, STDOUT_FILENO);
	if (root->type & (REDIN | HEREDOC))
		ft_change_fds(root->fd, STDIN_FILENO);
}

static void	ft_which_red(t_node *root, int flag, t_shell *shell)
{
	char	*str;

	str = root->right->str;
	root->right->str = ft_parse_expand(root->right->str, shell);
	if (ft_strncmp(root->right->str, "0x1A", 4) == 0)
	{
		free(root->right->str);
		root->right->str = ft_strdup("");
	}
	free(str);
	if (flag & REDOUT)
		root->fd = open(root->right->str, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else if (flag & APPEND)
		root->fd = open(root->right->str, O_WRONLY | O_CREAT | O_APPEND, 0666);
	else if (flag & REDIN)
		root->fd = open(root->right->str, O_RDONLY, 0644);
}

static void	ft_redirects(t_node *root, t_shell *shell)
{
	int	flag;

	if (root->left && root->left->type & REDALL)
		ft_redirects(root->left, shell);
	flag = 0;
	flag |= root->type;
	if (root->type & (REDALL)
		&& (!root->left || root->left->fd != -1))
	{
		ft_which_red(root, flag, shell);
		ft_dup_right_flag(root);
	}
	else if (root->left && root->left->fd == -1)
		root->fd = -1;
}

void	ft_exec_redirects(t_node *root, t_shell *shell)
{
	const int	fd[2] = {dup(STDIN_FILENO), dup(STDOUT_FILENO)};

	if (!root->fd)
		ft_redirects(root, shell);
	if (root->fd == -1)
	{
		ft_status(1);
		perror("minishell");
		return (ft_close_fd_tmp(fd));
	}
	ft_execution(root->left, shell);
	ft_close_fd_tmp(fd);
}
