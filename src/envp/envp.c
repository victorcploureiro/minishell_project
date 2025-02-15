/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bnespoli <bnespoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:40:52 by bnespoli          #+#    #+#             */
/*   Updated: 2025/01/16 16:31:44 by bnespoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_matrixreplace(char **s, char signal, char old_ch, char new_ch)
{
	int	i;

	i = 0;
	while (s[i])
	{
		ft_replace_char_between_signal(s[i], signal, old_ch, new_ch);
		i++;
	}
}

t_vector	*ft_build_line_envp(char *str)
{
	t_vector	*line_dict;
	char		**split;

	line_dict = ft_vector_create();
	if (!line_dict)
		return ((t_vector *)0);
	split = ft_split(str, '=');
	ft_vector_push_back(line_dict, ft_strdup(split[0]));
	if (split[1])
	{
		ft_vector_push_back(line_dict, ft_strdup(split[1]));
		ft_replace_char_between_signal(split[1], '\'', ':', 0x1A);
		ft_vector_push_back(line_dict, ft_split(split[1], ':'));
		ft_matrixreplace(line_dict->values[2], '\'', 0x1A, ':');
	}
	else
	{
		ft_vector_push_back(line_dict, ft_strdup(""));
		ft_vector_push_back(line_dict, NULL);
	}
	ft_vector_push_back(line_dict, ft_strdup(str));
	ft_freesplit(split);
	return (line_dict);
}

void	ft_free_line_envp(t_vector *line_dict)
{
	if (line_dict)
	{
		free(line_dict->values[0]);
		free(line_dict->values[1]);
		if (line_dict->values[2])
			ft_freesplit(line_dict->values[2]);
		free(line_dict->values[3]);
		ft_vector_free(line_dict);
	}
}

t_vector	*ft_envp_dict(char **envp)
{
	t_vector	*envp_dict;
	int			i;
	t_vector	*line_dict;

	i = 0;
	envp_dict = ft_vector_create();
	if (!envp_dict)
		return (NULL);
	while (envp[i] != NULL)
	{
		line_dict = ft_build_line_envp(envp[i]);
		if (!line_dict)
		{
			ft_vector_free(envp_dict);
			return (NULL);
		}
		ft_vector_push_back(envp_dict, line_dict);
		i++;
	}
	return (envp_dict);
}

void	ft_envp(t_shell *shell)
{
	shell->envp_dict = ft_envp_dict(shell->envp);
	shell->path = ft_getenv(shell->envp_dict, "PATH");
	if (!shell->path)
	{
		ft_env_add(shell->envp_dict,
			"PATH", "/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin");
		shell->path = ft_getenv(shell->envp_dict, "PATH");
	}
	shell->path_splitted = ft_split(shell->path, ':');
}
