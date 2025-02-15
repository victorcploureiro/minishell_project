/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bnespoli <bnespoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 14:55:18 by bnespoli          #+#    #+#             */
/*   Updated: 2025/01/16 16:46:48 by bnespoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_error_brackets(int st)
{
	static int	status;

	if (st != -1)
		status = st;
	return (status);
}

void	ft_process_prompt(t_parse *parse, char *prompt_arranged, int size,
		char *signal)
{
	while (parse->prompt[parse->idx->i] != '\0')
	{
		if (ft_strchr("\'\"", parse->prompt[parse->idx->i]))
			ft_parse_quotes(parse, prompt_arranged, signal);
		ft_parse_brackets(parse, prompt_arranged, 1);
		if (parse->idx->i < size && ft_is_space(parse->prompt[parse->idx->i]))
			parse->prompt[parse->idx->i] = ' ';
		if (!ft_parse_char(parse, prompt_arranged))
			prompt_arranged[parse->idx->j] = parse->prompt[parse->idx->i];
		if (parse->idx->i < size)
			parse->idx->i++;
		parse->idx->j++;
	}
}

void	ft_arranging_prompt(t_parse *parse)
{
	int		size;
	char	*prompt_arranged;
	char	*signal;

	signal = NULL;
	size = 0;
	if (parse->prompt)
		size = ft_strlen(parse->prompt);
	prompt_arranged = malloc((size * 2) * sizeof(char *) + 1);
	parse->idx = malloc(sizeof(t_index));
	parse->idx->i = 0;
	parse->idx->j = 0;
	ft_process_prompt(parse, prompt_arranged, size, signal);
	prompt_arranged[parse->idx->j] = '\0';
	parse->prompt_arranged = prompt_arranged;
	free(parse->idx);
}

char	**ft_parser(t_parse *parse)
{
	char	**split;

	ft_arranging_prompt(parse);
	split = ft_split(parse->prompt_arranged, ' ');
	ft_change_sub_space(split);
	free(parse->prompt_arranged);
	return (split);
}
