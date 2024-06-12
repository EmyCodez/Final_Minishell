/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_err.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emilin <emilin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 11:08:58 by emilin            #+#    #+#             */
/*   Updated: 2024/06/12 19:36:36 by emilin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_set_parse_err(t_parse_err *parse_error, t_parse_err_type type)
{
	parse_error->type = type;
}

void	ft_handle_parse_err(t_parse_err *parse_error, t_minishell *myshell)
{
	t_parse_err_type	type;
	t_token_type		token_type;
	char				**types;

	types = (char *[]){"T_IDENTIFIER",
		"<", ">", "<<", ">>", "|", "(", ")", "&&", "||", "newline"};
	type = parse_error->type;
	(void)token_type;
	(void)types;
	if (type)
	{
		if (type == E_SYNTAX)
		{
			if (!(myshell->curr_token))
				token_type = T_NL;
			else
				token_type = myshell->curr_token->type;
			ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
			ft_putstr_fd(types[token_type], 2);
			ft_putstr_fd("'\n", 2);
			myshell->exit_s = 258;
		}
		ft_clear_ast(&myshell->ast, &myshell->tokens);
		ft_bzero(parse_error, sizeof(t_parse_err));
	}
}
