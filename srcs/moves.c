/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubaujar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/02 14:24:07 by lubaujar          #+#    #+#             */
/*   Updated: 2015/12/02 14:24:08 by lubaujar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

int		check_keys_arrows(t_all *all, char *buff)
{
	int		i;
	int		test;
	static const	t_keys	keys[] =

	{{K_UP, goto_latest_commands}};

	i = 0;
	test = ft_getkey(buff);
	printf("-> |%d|\n", test);
	printf("->> |%d|\n", keys[i].action_name);
//	// int j = 0;
	// while (buff[j])
	// 	printf("-> [ %d ] ", buff[j++]);
	// printf("\n");
	while (i < 1)
	{
		if (test ==  keys[i].action_name)
		{
			keys[i].f(all, buff);
		}
		i++;
	}
	//write(1, "here\n", 5);
	/*(void)all;
	if (K_CTRL_LEFT)
		printf("OPT LEFT OK\n");
	if (K_ENTER)
	{
		tputs_termcap("me");
		return (-1);
	}
	if (K_UP || K_DOWN)
	{
		return (1);
	}
	else if (K_RIGHT || K_LEFT)
	{
		return (2);
	}
	// if (K_TAB)
	// {
	// 	//printf("-> %d\n", all_is_spaces(all->cmd_termcaps->head));
		
	// 	return (3);
	// }
	if (K_DELETE || K_DELETE2 || buff[0] == 127 || K_BACKSPACE)
	{
		return (4);
	}*/
	return (0);
}
/*
void	horizontal_moves(t_all *all, char *buff)
{
	// if (all->current && !all->history_moves)
	// {
	// 	all->history_moves = 1;
	// 	realloc_termcaps_cmd(all, all->current);
	// 	all->cursor_pos = (int)all->cmd_termcaps->lenght;
	// 	all->stop = 1;
	// }
	// printf("all->cmd: %s\n", all->cmd);
	// printf("cursor pos: %d\n", all->cursor_pos);
	if (!ft_strcmp(buff, K_LEFT) && all->cmd_termcaps->lenght > 0 && all->cursor_pos > 1)
	{
		all->cursor_pos--;
		tputs_termcap("le");
	}
	if (!ft_strcmp(buff, K_RIGHT) && all->cmd_termcaps->lenght > 0
		&&  (size_t)all->cursor_pos < all->cmd_termcaps->lenght + 1)
	{
		all->cursor_pos++;
		tputs_termcap("nd");
	}
	// all->stop = 0;
}

void	update_cmd_line_del(t_all *all)
{
	int		ct;

	ct = all->cursor_pos;
	while ((size_t)ct++ <= all->cmd_termcaps->lenght)
		tputs_termcap("nd");
	while (--ct)
	{
		tputs_termcap("le");
		tputs_termcap("dc");
	}
	create_cmd(all);
	ft_putstr(all->cmd);
	ct = (int)all->cmd_termcaps->lenght;
	while (ct-- >= all->cursor_pos)
		tputs_termcap("le");
}

void	del_char(t_all *all, char *buff)
{
	if (all->already_in_history)
		realloc_termcaps_cmd(all, all->cmd);
	tputs_termcap("me");
	tputs_termcap("dm");
	if (all->cmd_termcaps->lenght > 0 && all->cursor_pos > 1)
	{
		if ((size_t)all->cursor_pos <= all->cmd_termcaps->lenght + 1)
		{
		//	write(1, "HEERE\n", 6);
			(!ft_strcmp(buff, K_BACKSPACE)) ? dlst_del_one2(all->cmd_termcaps, all->cursor_pos)
				: dlst_del_one2(all->cmd_termcaps, all->cursor_pos - 1);
		}
		(ft_strcmp(buff, K_BACKSPACE)) ? tputs_termcap("le") : NULL;
		tputs_termcap("dc");
		if (ft_strcmp(buff, K_BACKSPACE))
			all->cursor_pos--;
		tputs_termcap("ed");
	}
}

void	make_moves(t_all *all, char buff[3])
{
	//int			deltab = 0;
	//write(1, "here\n", 5);
	//printf("|%c|\n", all->cmd_termcaps->tail->c);
	if (K_RIGHT || K_LEFT)
	{
		horizontal_moves(all, buff);
	}
	if (K_UP || K_DOWN)
		goto_latest_commands(all, buff);
	// if (K_TAB)
	// {
	// 	if (!all->cmd_termcaps->head)
	// 	{
	// 		return ;
	// 	}
	// 	else
	// 		open_directories(all);
	// }
	if (K_DELETE || K_DELETE2 || buff[0] == 127 || K_BACKSPACE)
	{
		if (all->already_in_history)
			realloc_termcaps_cmd(all, all->cmd);
		// if (all->already_autocomplete)
		// {
		// 	//printf("tail: %c\n", all->cmd_termcaps->tail->c);
		// 	printf("cursor_ pos: %d\n", all->cursor_pos);
		// 	printf("lenght list: %zu\n", all->cmd_termcaps->lenght);
		// }
		tputs_termcap("me");
		tputs_termcap("dm");
		if (all->cmd_termcaps->lenght > 0 && all->cursor_pos > 1)
		{
			if ((size_t)all->cursor_pos <= all->cmd_termcaps->lenght + 1)
			{
			//	write(1, "HEERE\n", 6);
				(K_BACKSPACE) ? dlst_del_one2(all->cmd_termcaps, all->cursor_pos)
					: dlst_del_one2(all->cmd_termcaps, all->cursor_pos - 1);
			}
			(!K_BACKSPACE) ? tputs_termcap("le") : NULL;
			tputs_termcap("dc");
			if (!K_BACKSPACE)
				all->cursor_pos--;
			tputs_termcap("ed");
		}
	}
}
*/