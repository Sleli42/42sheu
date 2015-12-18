 /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubaujar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/03 15:59:16 by lubaujar          #+#    #+#             */
/*   Updated: 2015/11/03 15:59:18 by lubaujar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

/*
mkdir test ; cd test ; ls -a ; ls | cat | wc -c > fifi ; cat fifi
*/
/*
void	display_tab(char **tab) {
	while (tab && *tab)
		printf("[%s]\n", *tab++);
}
*/
// void		read_key(char *cmd) {
// 	char	buff[3] = {0};

// 	read(0, buff, 3);
// 	if (K_UP || K_DOWN)
// 		printf("GO TO HISTORY\n");
// 	else if (K_RIGHT || K_LEFT || K_DELETE || K_SPACE)
// 		printf("GO TO DIRECTION\n");
// 	else
// 		ft_putstr(cmd);
// }

void	display_prompt(t_all *all) {
	(void)all;
	tputs_termcap("me");
	tputs_termcap("ve");
	write(1, "$: ", 3);
}

void	display_dlst2(t_dlist2 *lst) {
	t_cmd	*tmp = lst->head;

	printf("\n");
	printf("[ ");
	while (tmp) {
		printf("[ %c ] ", tmp->c);
		tmp = tmp->next;
	}
	printf(" ]");
	printf("\n");
}

void	create_cmd(t_all *all)
{
	t_cmd	*nav;
	int		i;

	nav = all->cmd_termcaps->head;
	i = 0;
	if (all->cmd)
		ft_strdel(&all->cmd);
	if (!(all->cmd = (char *)malloc(sizeof(char) * len_lst_cmd(all->cmd_termcaps->head) + 1)))
		error("MALLOC");
	if (nav)
	{
		while (nav)
		{
			all->cmd[i++] = nav->c;
			nav = nav->next;
		}
		all->cmd[i] = '\0';
	}
}

void	display_current_arg(t_all *all)
{
	t_select	*nav;

	nav = all->list_dir->head;
	while (nav)
	{
		if (nav->onArg)
			write(0, nav->arg, ft_strlen(nav->arg));
		nav = nav->next;
	}
}

void	realloc_termcaps_cmd(t_all *all, char *cmd2realloc)
{
	int		ct;

	ct = 0;
	if (all->cmd_termcaps)
	{
		del_dlist2(all->cmd_termcaps);
		all->cmd_termcaps = create_cmd_dlst();
	}
	while (cmd2realloc[ct])
	{
		dlst_add_back_2(all->cmd_termcaps, dlst_cmd_new(cmd2realloc[ct], ct));
		ct++;
	}
	//display_dlst2(all->cmd_termcaps);
	all->already_in_history = 0;
	// if (all->cmd_termcaps == NULL)

	// 	printf("NULLLLLLLL %s\n", cmd2realloc);
}
/*
	l* -l
	char2add: s
	current char pos->|*|
	prev char ->|l|
	next char ->|-|
*/

void	display_line(t_dlist2 *cmd_termcaps)
{
	t_cmd	*nav;

	nav = cmd_termcaps->head;
	while (nav)
	{
		write(1, &nav->c, 1);
		nav = nav->next;
	}
}

// void	insert_char_cmd(t_all *all, char char2add)
// {
// 	all->cmd_termcaps = dlst_insert_cmd(all->cmd_termcaps,
// 		dlst_cmd_new(char2add, 1), all->cursor_pos);
// 	create_cmd(all);
// 	all->stop = 0;
// }

void	update_cmd_line_insert(t_all *all, char char2add)
{
	size_t	ct;
	
	all->cmd_termcaps = dlst_insert_cmd(all->cmd_termcaps,
		dlst_cmd_new(char2add, 1), all->cursor_pos);
	create_cmd(all);
	ct = (size_t)all->cursor_pos;
	while (++ct < all->cmd_termcaps->lenght)
		tputs_termcap("nd");
	while (--ct > 0)
	{
		tputs_termcap("le");
		tputs_termcap("dc");
	}
	ft_putstr(all->cmd);
	ct = (size_t)all->cmd_termcaps->lenght;
	while (--ct > (size_t)all->cursor_pos)
		tputs_termcap("le");
	all->stop = 0;
	//printf("cursor: %d\n", all->cursor_pos);
	// while (++ct < (size_t)all->cursor_pos)
		//tputs_termcap("nd");
	// while (++ct < (size_t)all->cursor_pos)
	// 	tputs_termcap("nd");
	// while (all->cmd[ct++])
	// {
	// 	write(1, &all->cmdm[ct], 1);
	// }
	// 	tputs_termcap("nd");
}

void	loop(t_all *all)
{
	char	buff[MAXLEN];
	int		key;

	all->stop = 0;
	all->already_open = 0;
	all->already_in_history = 0;
	all->is_history = 0;
	all->ct_select = 0;
	all->cursor_pos = 0;
	all->history_moves = 0;
	all->cmd_termcaps = create_cmd_dlst();
	display_prompt(all);
	if (!(all->cmd = (char *)malloc(sizeof(char) * MAXLEN - 1)))
		error("MALLOC");
	ft_memset(buff, 0, MAXLEN - 1);
	all->nav = all->cmd_history->tail_node;
	while (*buff != '\n')
	{
		read(0, buff, (MAXLEN - 1));
		//printf("buff: |%d|\n", buff[0]);
		// if (all->already_open)
		// 	display_current_arg(all);
		if ((key = check_keys_arrows(all, buff)) < 0)
		{
			if (all->already_open)
				add_to_cmd(all, all->nav_dir->prev->arg);
				//printf("curr dir: %s\n", all->nav_dir->prev->arg);
			break ;
		}
		else if (key > 0)
		{
			all->stop = 1;
			make_moves(all, buff);
		}
		else
		{
			if (*buff == '/')
			{
				if (all->nav_dir)
					add_to_cmd(all, (all->nav_dir->prev) ? all->nav_dir->prev->arg : all->nav_dir->arg);
				all->already_open = 0;
			}
			if (all->already_in_history)
			{
				realloc_termcaps_cmd(all, all->cmd);
				//create_cmd(all);
				ft_strdel(&all->current);
				all->is_history = 1;
				all->stop = 0;
			}
			if ((size_t)all->cursor_pos < all->cmd_termcaps->lenght && *buff != '\n')
			{
				//insert_char_cmd(all, *buff);
				update_cmd_line_insert(all, *buff);
			}
			else
			{
				ft_putchar(*buff);
				dlst_add_back_2(all->cmd_termcaps, dlst_cmd_new(*buff, all->cmd_termcaps->lenght + 1));
			}
			all->cursor_pos++;
		}
	}
	(!all->stop) ? create_cmd(all) : ft_strdel(&all->current);
	(!all->stop && !all->is_history) ? write(1, "\n", 1) : write(1, "\0", 1);
	(all->cmd[ft_strlen(all->cmd) - 1] == '\n') ? all->cmd[ft_strlen(all->cmd) - 1] = '\0'
		: write(1, "\0", 1);
	printf("cmd: |%s|\n", all->cmd);
	// printf("lenght list[main]: %zu\n", all->cmd_termcaps->lenght);
	if (all->cmd[0] != 0 && ft_strlen(all->cmd) > 0)
	{
		dlst_add_back(all->cmd_history, dlst_node_new(all->cmd, all->cmd_history->lenght + 1));
		parse_command(all, all->cmd);
		exec_command(all);
	}
	// del_dlist2(all->cmd_termcaps);
	loop(all);
}

int		main(int ac, char **av, char **env)
{
	t_all	*all;

	(void)ac;
	(void)av;
	all = init_all(env);
	init_term();
	loop(all);
	del_dlist(all->env);
	del_dlist(all->cmd_history);
	restore_term(all->restore);
	write(1, "\n", 1);
	exit(1);
	return (0);
}