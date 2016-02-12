/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paste.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubaujar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/05 10:04:19 by lubaujar          #+#    #+#             */
/*   Updated: 2016/01/05 10:04:22 by lubaujar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void		copy_buffer(t_all *all)
{
	if (!all->already_in_select)
	{
		all->save_cursor_pos = all->cursor_pos - PROMPT_LEN + 1;
		all->already_in_select = 1;
		all->cpy_move_right = 0;
		all->cpy_move_left = 0;
	}
	else
	{
		tputs_termcap("me");
		all->already_in_select = 0;
		if (all->cpy_move_right > 0)
			copy_right(all);
		else if (all->cpy_move_left > 0)
			copy_left(all);
	}
}

void		cut_buffer(t_all *all)
{
	if (!all->already_in_select)
	{
		all->save_cursor_pos = all->cursor_pos;
		all->already_in_select = 1;
		all->cpy_move_right = 0;
		all->cpy_move_left = 0;
	}
	else
	{
		tputs_termcap("me");
		all->already_in_select = 0;
		if (all->cpy_move_right > 0)
			cut_right(all);
		else if (all->cpy_move_left > 0)
			cut_left(all);
	}
}

// static char	*insert_str_to_str(char *cmd, char *cpy, int pos)
// {
// 	char	*ret;
// 	int		ct;
// 	int		i;
// 	int		j;

// 	ct = 0;
// 	i = 0;
// 	j = 0;
// 	if (!(ret = (char *)malloc(sizeof(char) * ft_strlen(cmd) + \
// 												ft_strlen(cpy) + 1)))
// 		return (NULL);
// 	while (cmd[i] && i < pos - 1)
// 		ret[ct++] = cmd[i++];
// 	while (cpy[j])
// 		ret[ct++] = cpy[j++];
// 	if ((size_t)i < ft_strlen(cmd))
// 		while (cmd[i])
// 			ret[ct++] = cmd[i++];
// 	ret[ct] = 0;
// 	return (ret);
// }

void		paste_buffer(t_all *all)
{
	int		save;
	// char	*new_str;

	save = 0;
	if (all->copy)
	{
		all->already_in_paste = 1;
		save = (all->cursor_pos - PROMPT_LEN) + 1;
		// create_cmd(all);
		// new_str = insert_str_to_str(all->cmd, all->copy, save);
		tputs_termcap("im");
		int			i = 0;
		if (CMD_NULL) /* FAUX */
			all->cmd_termcaps = create_cmd_dlst();
		while (all->copy[i])
		{
			if (all->copy[i] != '\n')
				ft_putchar(all->copy[i]);
			if (all->cursor_pos - PROMPT_LEN < (int)all->cmd_termcaps->lenght)
				update_cmd_line_insert(all, all->copy[i]);
			else
			{
				dlst_add_back_2(all->cmd_termcaps, dlst_cmd_new(all->copy[i]));
				CURSOR++;
			}
			if (all->nb_lines >= 1)
				shift(all);
			i++;
		}
		create_cmd(all);
		// printf("allcmd: |%s|\n", all->cmd);
		// new_str = insert_str_to_str(all->cmd, all->copy, save);
		realloc_termcaps_cmd(all, all->cmd);
		// ft_strdel(&new_str);
		// ft_putstr(all->copy);
		tputs_termcap("ei");
		CURSOR = save + PROMPT_LEN + (ft_strlen(all->copy) - 1);
	}
}
