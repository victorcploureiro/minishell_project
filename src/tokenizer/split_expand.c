/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bnespoli <bnespoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 17:58:15 by bnespoli          #+#    #+#             */
/*   Updated: 2025/01/16 16:55:53 by bnespoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_process_dollar_question(char *str, t_vector *split, t_index *idx)
{
	idx->j++;
	idx->j++;
	if (str[idx->i] != '\0' && idx->j != idx->i)
		ft_vector_push_back(split, ft_substr(str, idx->i, idx->j - idx->i));
	idx->i = idx->j;
}

void	ft_process_dollar(char *str, t_vector *split, t_index *idx)
{
	idx->j++;
	while ((ft_isalnum(str[idx->j]) == 8 || str[idx->j] == '_'
			|| str[idx->j] == '?') && str[idx->j] != '\0')
		idx->j++;
	if (str[idx->i] != '\0' && idx->j != idx->i)
		ft_vector_push_back(split, ft_substr(str, idx->i, idx->j - idx->i));
	idx->i = idx->j;
}

void	ft_process_alnum(char *str, t_vector *split, t_index *idx)
{
	if ((ft_isalnum(str[idx->j]) == 0 && str[idx->j] != '$')
		&& str[idx->j] != '\0')
		idx->j++;
	if (str[idx->i] != '\0' && idx->j != idx->i)
		ft_vector_push_back(split, ft_substr(str, idx->i, idx->j - idx->i));
	idx->i = idx->j;
}

void	ft_process_split(char *str, t_vector *split, t_index *idx)
{
	while (str[idx->i] != '\0')
	{
		while ((ft_isalnum(str[idx->j]) == 8) && str[idx->j] != '\0')
			idx->j++;
		if (str[idx->i] != '\0' && idx->j != idx->i)
			ft_vector_push_back(split, ft_substr(str, idx->i, idx->j - idx->i));
		idx->i = idx->j;
		if (str[idx->j] == '$' && str[idx->j + 1] == '?')
			ft_process_dollar_question(str, split, idx);
		else if (str[idx->j] == '$')
			ft_process_dollar(str, split, idx);
		else
			ft_process_alnum(str, split, idx);
	}
}

void	ft_split_expand(char *str, t_vector *split)
{
	t_index	idx;

	idx.i = 0;
	idx.j = 0;
	if (!str)
		return (ft_vector_push_back(split, ft_strdup("")));
	ft_process_split(str, split, &idx);
}
