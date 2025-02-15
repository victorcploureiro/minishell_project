/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bnespoli <bnespoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:30:56 by bnespoli          #+#    #+#             */
/*   Updated: 2025/01/16 16:28:25 by bnespoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_freesplit(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

char	*ft_strcpy(char *dest, char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_getenv(t_vector *env, char *name)
{
	unsigned long	i;
	t_vector		*line;
	char			*identifier;
	char			*value;

	i = 0;
	if (!env || !name)
		return (NULL);
	while (i < ft_vector_size(env))
	{
		line = ft_vector_at(env, i);
		if (!line || ft_vector_size(line) < 2)
		{
			i++;
			continue ;
		}
		identifier = ft_vector_at(line, 0);
		value = ft_vector_at(line, 1);
		if (identifier && ft_strcmp(identifier, name) == 0)
			return ((char *)value);
		i++;
	}
	return (NULL);
}

int	ft_atoll_check(char *str, bool *error)
{
	unsigned long long	num;
	int					signal;
	int					i;

	num = 0;
	signal = 1;
	i = 0;
	while (str[i] && ft_is_space(str[i]))
		i++;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
	{
		signal *= -1;
		i++;
	}
	while (str[i] && ft_isdigit(str[i]))
	{
		num = (num * 10) + (str[i] - '0');
		if (ft_check_limits(signal, num, error))
			break ;
		i++;
	}
	return (num * signal);
}
