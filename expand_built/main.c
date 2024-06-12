/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emilin <emilin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 01:28:41 by abelayad          #+#    #+#             */
/*   Updated: 2024/06/13 02:59:54 by emilin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_init_minishell(char **env, t_minishell *myshell)
{
	ft_memset(myshell, 0, sizeof(t_minishell));
	myshell->environ = env;
	init_env_list(env, &myshell->envlst);
	myshell->stdin = dup(0);
	myshell->stdout = dup(1);
	tcgetattr(STDIN_FILENO, &myshell->original_term);
}

static void	ft_start_execution(t_minishell *myshell)
{
	signal(SIGQUIT, ft_sigquit_handler);
	ft_init_tree(myshell->ast, myshell);
	if (myshell->heredoc_sigint)
	{
		ft_clear_ast(&myshell->ast, &myshell->tokens);
		myshell->heredoc_sigint = false;
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &myshell->original_term);
	myshell->exit_s = ft_exec_node(myshell->ast, false, myshell);
	ft_clear_ast(&myshell->ast, &myshell->tokens);
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	myshell;
	
	((void)argc, (void)argv);
	ft_init_minishell(env, &myshell);
	while (1)
	{
		ft_init_signals(&myshell);
		myshell.line = readline(PROMPT_MSG);
		if (!myshell.line)
			(ft_clean_ms(&myshell),
				ft_putstr_fd("exit\n", 1), exit(myshell.exit_s));
		if (myshell.line[0])
			add_history(myshell.line);
		myshell.tokens = ft_tokenize(myshell.line, &myshell.exit_s);
		if (!myshell.tokens)
			continue ;
		myshell.ast = ft_parse(&myshell.tokens, &myshell.curr_token, &myshell.parse_err);
		if (myshell.parse_err.type)
		{
			ft_handle_parse_err(&myshell.parse_err, &myshell);
			continue ;
		}
		ft_start_execution(&myshell);
	}
	ft_garbage_collector(NULL, true);
	return (ft_clean_ms(&myshell), myshell.exit_s);
}
