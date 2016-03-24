/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop_tools_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhatir <skhatir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/18 18:06:36 by lubaujar          #+#    #+#             */
/*   Updated: 2016/03/21 16:33:06 by skhatir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "full_sh.h"

void		check_glob(t_glob *g)
{
	if (g->err)
		return ;
	else if (g->sub != 0 || g->crush != 0 || g->c_indx != 0 || g->quote != 0 ||\
			g->d_quote != 0)
		g->err = g->err && *g->err ? g->err : ft_strdup("\nParse error");
}

void		save_glob(t_all *all, t_glob *g)
{
	if (*all->buff == '(' || *all->buff == ')')
		g->sub += *all->buff == '(' ? 1 : -1;
	else if (*all->buff == '{' || *all->buff == '}')
		g->crush += *all->buff == '{' ? 1 : -1;
	else if (*all->buff == '[' || *all->buff == ']')
		g->c_indx += *all->buff == '[' ? 1 : -1;
	else if (*all->buff == '\'')
		g->quote += g->quote ? -1 : 1;
	else if (*all->buff == '\"')
		g->d_quote += g->d_quote ? -1 : 1;
	else if (*all->buff == '\\')
		g->back += g->back ? -1 : 1;
	if (g->sub < 0 || g->crush < 0 || g->c_indx < 0)
		g->err = ft_strdup("\nParse error");
	all->cursor_pos++;
}

void		create_and_exec_command(t_all *all)
{
	create_cmd(all);
	(all->cmd[ft_strlen(all->cmd) - 1] == '\n') ?
		all->cmd[ft_strlen(all->cmd) - 1] = '\0' : write(1, "\0", 1);
	write(1, "\n", 1);
	if (all->cmd[0] != 0 && ft_strlen(all->cmd) > 0)
	{
		add_to_history(all);
		parse_command(all, all->cmd);
		all->parsecmd && *all->parsecmd ? exec_command(all) : NULL;
	}
	!CMD_NULL ? del_dlist2(all->cmd_termcaps) : NULL;
	all->cmd && *all->cmd ? ft_strdel(&all->cmd) : NULL;
	loop(all);
}

void		write_buffer(t_all *all)
{
	int		ct;

	ct = 0;
	if (all->globing.dt_print)
		*&all->globing.dt_print = 0x0;
	else
		ft_putstr(all->buff);
	if (ft_strlen(all->buff) > 1)
	{
		while (all->buff[ct])
		{
			dlst_add_back_2(all->cmd_termcaps, dlst_cmd_new(all->buff[ct]));
			CURSOR++;
			ct++;
		}
	}
	else
	{
		dlst_add_back_2(all->cmd_termcaps, dlst_cmd_new(*all->buff));
		CURSOR++;
	}
}

void		insert_char(t_all *all)
{
	all->already_open = 0;
	if (*all->buff == '$')
		all->lv += 1;
	tputs_termcap("im");
	if (all->cursor_pos - PROMPT_LEN < (int)all->cmd_termcaps->lenght \
													&& *all->buff != '\n')
	{
		if (*all->buff != '\n')
			ft_putstr(all->buff);
		update_cmd_line_insert(all, *all->buff);
		if (all->nb_lines >= 1)
			shift(all);
	}
	else
	{
		if (all->cursor_pos == all->ws.ws_col * all->curr_line)
			all->curr_line++;
		if (*all->buff != '\n')
			write_buffer(all);
	}
	tputs_termcap("ei");
}