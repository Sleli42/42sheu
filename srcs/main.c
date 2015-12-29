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
	//tputs_termcap("me");
	tputs_termcap("ve");
	write(1, "$: ", 3);
}

void	display_dlst(t_dlist2 *lst)
{
	t_cmd *tmp = lst->head;

	printf("\n");
	while (tmp) {
		printf("[ %c ]", tmp->c);
		tmp = tmp->next;
	}
	printf("\nlenght: %zu\n", lst->lenght);
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
	}
	all->cmd[i] = '\0';
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
		dlst_add_back_2(all->cmd_termcaps, dlst_cmd_new(cmd2realloc[ct++]));
	all->cursor_pos = (int)all->cmd_termcaps->lenght + 1;
}


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

void	display_cursor(t_dlist2 *lst, int pos)
{
	t_cmd *tmp = lst->head;
	int		ct = 0;

	while (ct < pos) {
		tmp = tmp->next;
		ct++;
	}
	printf("cursor: |%c|\n", tmp->c);
}

void	update_cmd_line_insert(t_all *all, char char2add)
{
	all->cmd_termcaps = dlst_insert_cmd(all->cmd_termcaps,
		dlst_cmd_new(char2add), all->cursor_pos);
	all->cursor_pos++;
}

int		ft_getkey(char *s)
{
	int			result;
	int			i;
	int			mult;
	int			value;

	result = 0;
	i = 0;
	while (i < 6)
	{
		mult = 10;
		value = s[i];
		while (value > 10)
		{
			mult *= 10;
			value /= 10;
		}
		result = result * mult + s[i];
		i++;
	}
	return (result);
}

void	read_key(char *buff)
{
	printf("|%d|\n", ft_getkey(buff));
}

void	create_and_exec_command(t_all *all)
{
	create_cmd(all);
	(all->cmd[ft_strlen(all->cmd) - 1] == '\n') ?
		all->cmd[ft_strlen(all->cmd) - 1] = '\0' : write(1, "\0", 1);
	if (all->cmd)
		printf("cmd: |%s|\n", all->cmd);
	if (!all->cmd[0] == 0 && ft_strlen(all->cmd) > 0)
	{
		add_to_history(all);
		parse_command(all, all->cmd);
		exec_command(all);
	}
	del_dlist2(all->cmd_termcaps);
	//reset_term();
	loop(all);
}

void	loop(t_all *all)
{
	char	*buff;
	int		key;

	all->stop = 0;
	all->already_open = 0;
	all->already_in_history = 0;
	all->cursor_pos = 1;
	all->index_history = all->pos_history;
	all->cmd_termcaps = create_cmd_dlst();
	// init_term();
	//tputs_termcap("mm");
	display_prompt(all);
	if (!all->cmd)
	{
		if (!(all->cmd = (char *)malloc(sizeof(char) * MAXLEN - 1)))
			error("MALLOC");
	}
	// if (all->already_autocomplete)
	// {
	// 	if (all->tmp_cmd != NULL)
	// 	{
	// 		realloc_termcaps_cmd(all, all->tmp_cmd);
	// 		ft_putstr(all->tmp_cmd);
	// 		ft_strdel(&all->tmp_cmd);
	// 	}
	// 	del_clist(&all->list_dir);
	// }
	///buff = (char *)malloc(sizeof(char *));
	buff = ft_strnew(6);
	ft_memset(buff, 0, 6);
	//tputs_termcap("ti");
	while (*buff != '\n')
	{
		read(0, buff, 6);
	//	read_key(buff);
		if ((key = check_keys_arrows(all, buff)) < 0)
			create_and_exec_command(all);
		else if (key > 0)
		{
			continue ;
		}
		else
		{
			tputs_termcap("im");
			// if (*buff == '/')
			// {
			// 	if (all->nav_dir)
			// 		add_to_cmd(all, (all->nav_dir->prev) ? all->nav_dir->prev->arg : all->nav_dir->arg);
			// 	all->already_open = 0;
			// }
			// if (all->already_in_history)
			// {
			// 	realloc_termcaps_cmd(all, all->cmd);
			// 	//create_cmd(all);
			// 	ft_strdel(&all->current);
			// 	all->is_history = 1;
			// 	all->stop = 0;
			// }
			//printf("pos cursor: %d\n", all->cursor_pos);
			if ((size_t)all->cursor_pos <= all->cmd_termcaps->lenght && *buff != '\n')
			{
				//write(1, "here\n", 5);
				//tputs_termcap("im");
				if (*buff != '\n')
					ft_putchar(*buff);
				//insert_char_cmd(all, *buff);
				update_cmd_line_insert(all, *buff);
				//tputs_termcap("ei");
			}
			else
			{
				//printf("|%d| && |%d|\n", buff[0], buff[1]);
				all->cursor_pos++;
				if (*buff != '\n')
					ft_putchar(*buff);
				dlst_add_back_2(all->cmd_termcaps, dlst_cmd_new(*buff));
			}
			tputs_termcap("ei");
			// all->cursor_pos++;
		}
	}
	create_and_exec_command(all);
	// reset_term();
	// loop(all);
	//printf("last: %c\n", all->cmd_termcaps->tail->c);
	//create_cmd(all);
	// (!all->stop && !all->is_history) ? write(1, "\n", 1) : write(1, "\0", 1);
	// (all->cmd[ft_strlen(all->cmd) - 1] == '\n') ? all->cmd[ft_strlen(all->cmd) - 1] = '\0'
	// 	: write(1, "\0", 1);
	// if (all->cmd)
	// 	printf("cmd: |%s|\n", all->cmd);
	// printf("lenght list[main]: %zu\n", all->cmd_termcaps->lenght);
	// if (!all->cmd[0] == 0 && ft_strlen(all->cmd) > 0)
	// {
	// 	//write(1, "NO\b", 3);
	// 	add_to_history(all);
	// 	parse_command(all, all->cmd);
	// 	exec_command(all);
	// }
	//reset_term();
	// del_dlist2(all->cmd_termcaps);
	//tputs_termcap("te");
	//tputs_termcap("mo");
	//loop(all);
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
	reset_term();
	//restore_term(all->restore);
	write(1, "\n", 1);
	exit(1);
	return (0);
}