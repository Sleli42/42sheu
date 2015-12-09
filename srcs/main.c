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
	tputs_termcap("ve");
	write(1, "$: ", 3);
}

void	display_dlst(t_dlist2 *lst) {
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
	if (!(all->cmd = (char *)malloc(sizeof(char) * MAXLEN - 1)))
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

void	loop(t_all *all)
{
	char	buff[MAXLEN];
	int		key;
	int		stop = 0;

	all->cmd_termcaps = create_cmd_dlst();
	display_prompt(all);
	if (!(all->cmd = (char *)malloc(sizeof(char) * MAXLEN - 1)))
		error("MALLOC");
	ft_memset(buff, 0, MAXLEN - 1);
	all->nav = all->cmd_history->tail_node;
	while (*buff != '\n')
	{
		read(0, buff, (MAXLEN - 1));
		if ((key = check_keys_arrows(buff)) < 0)
			break ;
		else if (key > 0)
		{
			make_moves(all, buff);
			stop = 1;
		}
		else
		{
			ft_putchar(*buff);
			dlst_add_back_2(all->cmd_termcaps, dlst_cmd_new(*buff, all->cmd_termcaps->lenght));
		}
	}
	(!stop) ? create_cmd(all) : ft_strdel(&all->current);
	(!stop) ? write(1, "\n", 1) : write(1, "\0", 1);
	if (all->cmd[0] != 0 && ft_strlen(all->cmd) > 0)
	{
		dlst_add_back(all->cmd_history, dlst_node_new(all->cmd, all->cmd_history->lenght + 1));
		parse_command(all, all->cmd);
		exec_command(all);
	}
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