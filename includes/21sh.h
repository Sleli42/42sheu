/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   21sh.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubaujar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/03 16:12:43 by lubaujar          #+#    #+#             */
/*   Updated: 2015/11/03 16:12:44 by lubaujar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _21SH_H
# define _21SH_H

# include <stdlib.h>
# include <unistd.h>
# include <libft.h>
# include <stdio.h>
# include <sys/types.h> 
# include <sys/wait.h>
# include <sys/stat.h>
// termcaps
# include <term.h>
# include <termios.h>
# include <curses.h>
# include <sys/ioctl.h>
# include <dirent.h>

# define	MAXLEN			2048
# define	K_UP			279165000
# define	K_CTRL_UP		-23278475
# define	K_DOWN			279166000
# define	K_CTRL_DOWN		-23278474
# define	K_RIGHT			279167000
# define	K_CTRL_RIGHT	-23278473
# define	K_LEFT			279168000
# define	K_CTRL_LEFT		-23278472
# define	K_ECHAP			2700000
# define	K_BACKSPACE		12700000
# define	K_DELETE		2145308824
# define	K_ENTER			1000000
# define	K_TAB			900000
# define	K_HOME			279172000
# define	K_END			279170000
# define	K_CTRL_CLEAR	-7040000
# define	K_CTRL_COPY		-6990000
# define	K_CTRL_PASTE	-4302000
# define	K_CTRL_CUT		-4310000

# define 	NOTATTY 	1

typedef struct dirent	t_dirent;
typedef struct termios	t_termios;
typedef struct winsize	t_winsize;
typedef struct stat		t_stat;

typedef struct			s_node
{
	char				*s;
	size_t				index;
	struct s_node		*next;
	struct s_node		*prev;
}						t_node;

typedef struct			s_dlist
{
	struct s_node		*head;
	struct s_node		*tail;
	size_t				lenght;
}						t_dlist;

typedef struct			s_cmd
{
	char				c;
	struct s_cmd		*next;
	struct s_cmd		*prev;
}						t_cmd;

typedef struct			s_dlist2
{
	struct s_cmd		*head;
	struct s_cmd		*tail;
	size_t				lenght;
}						t_dlist2;

typedef struct			s_select
{
	char				*arg;
	int					onArg;
	struct s_select		*next;
	struct s_select		*prev;
}						t_select;

typedef struct			s_clist
{
	struct s_select		*head;
	struct s_select		*tail;
	size_t				lenght;
}						t_clist;

typedef struct			s_all
{
	// TERM 2 USE && TERM 2 RESTORE
	t_termios			term;
	t_termios			restore;
	// TERMCAPS CMD
	t_dlist2			*cmd_termcaps;
	int					current_key;
		// --AUTOCOMPLETE
	t_clist				*list_dir;
	t_select			*nav_dir;
	t_winsize			ws;
	t_stat				stat;
	char				*tmp_cmd;
	char				*tmp_dir;
	int					already_autocomplete;
	int					already_equ;
	int					hidden_file;

	int					maxlen_arg;
	int					files_by_row;
	int					already_open;
	int					nb_char_write;
	int					ct_select;
		// --HORIZONTAL MOVES
	int					cursor_pos;
	int					history_moves;
	int					already_in_moves;
		// --VERTICAL MOVES
	int					nb_lines;
	int					curr_line;
	int					nl;
		// --HISTORY
	char				**history_buff;
	int					fd_history;
	int					pos_history;
	int					index_history;

	t_dlist				*cmd_history;
	int					already_in_history;
	int					is_history;
		// --COPY & PASTE & CUT
	int					already_in_select;
			// --COPY
	int					save_cursor_pos;
	int					cpy_move_right;
	int					cpy_move_left;
			// --PASTE
	char				*copy;
	// PARSE && EXEC
	t_dlist				*env;
	t_node				*nav;
	char				**dupenv;
	char				**path2exec;
	char				**parsecmd;
	char				**pipe;
	char				*oldpwd;
	char				*cmd;
	int					fd2open;
	int					stop;
}						t_all;

typedef	struct			s_redirect
{
	char				*action_name;
	void				(*f)(t_all *, char *);
}						t_redirect;

typedef	struct			s_builtins
{
	char				*action_name;
	void				(*f)(t_all *, char *);
}						t_builtins;

typedef	struct			s_keys
{
	int					action_name;
	void				(*f)(t_all *);
}						t_keys;


/* -------------------------------------------------------------
*** ============================================================ ARROWS_AND_MOVEMENTS
*/
	/*
	*** ======================= horizontal_movements_tools.c
	*/
	int			ft_getkey(char *s);
	int			check_keys_arrows(t_all *all, char *buff);
	void		parse_keys(t_all *all);
	/*
	*** ======================= delete_char.c
	*/
	void		del_char(t_all *all);
	/*
	*** ======================= horizontal_movements.c
	*/
	void		horizontal_moves(t_all *all);
	void		opt_left_move(t_all *all);
	void		opt_right_move(t_all *all);
	/*
	*** ======================= horizontal_movements_tools.c
	*/
	int			check_if_spaces_before(t_dlist2 *lst, int pos);
	int			check_if_spaces_after(t_dlist2 *lst, int pos);
	void		goto_end(t_all *all);
	void		goto_begin(t_all *all);
	t_cmd		*goto_cursor_pos(t_cmd *lst, int pos);
	/*
	*** ======================= vertical_movements.c
	*/
	void		goto_up_line(t_all *all);
	void		goto_down_line(t_all *all);
	/*
	*** ======================= clear_screen.c
	*/
	void		ft_clear_screen(t_all *all);
	/*
	*** ======================= copy_cut_and_paste.c
	*/
	void		copy_buffer(t_all *all);
	void		cut_buffer(t_all *all);
	void		paste_buffer(t_all *all);
	/*
	*** ======================= copy_tools.c
	*/
	void		standard_mode(char char2print);
	void		reverse_mode(char char2print);
	void		copy_right(t_all *all);
	void		copy_left(t_all *all);
	/*
	*** ======================= cut_tools.c
	*/
	void		cut_right(t_all *all);
	void		cut_left(t_all *all);
	/*
	*** ======================= shift.c
	*/
	int			define_current_line(t_all *all, int pos);
	void		shift(t_all *all);

/* -------------------------------------------------------------
*** ============================================================ AUTOCOMPLETE
*/
	/*
	*** ======================= autocomplete.c
	*/
	int			define_nb_files_by_row(t_all *all, t_clist *lst);
	char		*find_path(char *cmd);
	void		swap_elems(t_select *a, t_select *b);
	void		sort_name(t_select **lst);
	void		list_elems(t_all *all, DIR *entry);
	void		new_line_autocomplet(t_all *all);
	void		display_current(t_all *all, t_select *nav);
	void		search_equ(t_all *all, char *dir);
	void		search_current_dir(t_all *all);
	void		search_bin_path(t_all *all);
	void		open_directories(t_all *all);

/* -------------------------------------------------------------
*** ============================================================ BUILTINS
*/
	/*
	*** ======================= builtins.c
	*/
	void		pwd_display(t_all *all, char *cmd);
	void		goto_dir(t_all *all, char *cmd);
	void		free_all(t_all *all, char *cmd);
	/*
	*** ======================= builtins_tools.c
	*/
	void		update_oldpwd(t_all *all);

/* -------------------------------------------------------------
*** ============================================================ ENV
*/
	/*
	*** ======================= env.c
	*/
	void		env_display(t_all *all, char *cmd);
	void		env_set(t_all *all, char *cmd);
	void		env_unset(t_all *all, char *cmd);
	/*
	*** ======================= env_init.c
	*/
	t_dlist		*init_env(char **env);
	/*
	*** ======================= env_tools.c
	*/
	char		*find_env_arg(t_all *all, char *arg2find);
	char		**ft_dupenv(char **env);

/* -------------------------------------------------------------
*** ============================================================ LISTS
*/
	/*
	*** ======================= dlist.c
	*/
	t_node		*dlst_node_new(char *data, size_t index);
	t_dlist		*dlst_add_back(t_dlist *lst, t_node *node);
	t_dlist		*dlst_del_one(t_dlist *lst, char *arg2del);
	void		del_dlist(t_dlist *lst);
	/*
	*** ======================= dlist_tools.c
	*/
	t_dlist		*create_dlst(void);
	int			len_lst_node(t_node *lst);
	int			update_list(t_dlist *lst, t_node *elem);
	/*
	*** ======================= dlist_2.c
	*/
	t_dlist2	*dlst_insert_cmd(t_dlist2 *lst, t_cmd *node, int pos);
	t_dlist2	*dlist_add_front_2(t_dlist2 *lst, t_cmd *node);
	t_dlist2	*dlst_add_back_2(t_dlist2 *lst, t_cmd *node);
	t_dlist2	*dlst_del_one2(t_dlist2 *lst, int pos);
	void		del_dlist2(t_dlist2 *lst);
	/*
	*** ======================= dlist_2_tools.c
	*/
	t_cmd		*dlst_cmd_new(char c);
	t_dlist2	*create_cmd_dlst(void);
	void		swap_elems_cmd(t_cmd *a, t_cmd *b);
	int			len_lst_cmd(t_cmd *lst);
	int			update_list2(t_dlist2 *lst, t_cmd *elem);
	/*
	*** ======================= clist.c
	*/
	t_clist		*create_clst(void);
	t_select	*clst_create_elem(char *s);
	t_clist		*clst_add_elem_back(t_clist *lst, t_select *node);
	int			len_clst(t_select *lst);
	void		del_clist(t_clist **lst);

/* -------------------------------------------------------------
*** ============================================================ EXEC_BINARY
*/
	/*
	*** ======================= binary_tools.c
	*/
	int			good_access(char *bin);
	char		*create_path(char *path, char *bin);
	void		exec_right_binary(t_all *all, char **argv_bin);
	void		exec_binary(char *bin, char **argv_bin, char **env);
	/*
	*** ======================= cmd_parse.c
	*/
	void		parse_command(t_all *all, char *buff);
	int			check_redirection(char *s);
	/*
	*** ======================= cmd_exec.c
	*/
	void		exec_simple_cmd(t_all *all, char *cmd);
	void		exec_redirection_cmd(t_all *all, char *cmd);
	void		exec_command(t_all *all);

/* -------------------------------------------------------------
*** ============================================================ SHELL
*/
	/*
	*** ======================= main.c
	*/
	int			main(int ac, char **av, char **env);
	/*
	*** ======================= shell_init.c
	*/
	t_all		*init_all(char **env);
	/*
	*** ======================= shell_loop.c
	*/
	void		display_prompt(t_all *all);
	void		create_and_exec_command(t_all *all);
	void		loop(t_all *all);
	/*
	*** ======================= shell_tools.c
	*/
	int			find_maxlen_elem(t_clist *lst);
	char		goto_elem(t_cmd *cmd, int pos);
	void		update_cmd_line_insert(t_all *all, char char2add);
	void		realloc_termcaps_cmd(t_all *all, char *cmd2realloc);
	void		create_cmd(t_all *all);
	/*
	*** ======================= debug.c
	*/
	void		display_dlst(t_dlist2 *lst);
	void		display_current_arg(t_all *all);
	void		display_line(t_dlist2 *cmd_termcaps);
	void		display_cursor(t_dlist2 *lst, int pos);
	void		read_key(char *buff);
	/*
	*** ======================= error.c
	*/
	void		error(char *err);
	void		term_error(char *err);

/* -------------------------------------------------------------
*** ============================================================ REDIRECTS
*/
	/*
	*** ======================= pipe.c
	*/
	void		create_pipe(t_all *all, char *cmd);
	void		exec_pipe_process(t_all *all, char *cmd, char **args);
	/*
	*** ======================= pipe_tools.c
	*/
	char		*create_good_path(t_all *all, char *cmd);
	int			check_redirect(char *s);
	int			open_file(char *file, int redir);
	char		*redirected_in_args(char **args, int *redir_type);
	/*
	*** ======================= redirection.c
	*/
	void		erase_and_replace(t_all *all, char *cmd);
	void		add_to_end(t_all *all, char *cmd);
	void		read_file(t_all *all, char *cmd);
	void		read_stdin(t_all *all, char *cmd);
	void		exec_redirect(t_all *all, char *cmd, char **args, char *file, int redir);
	/*
	*** ======================= redirection_tools.c
	*/
	void		dup_and_exec(t_all *all, char **argv, int fd2back, int fd2dup);

/* -------------------------------------------------------------
*** ============================================================ HISTORY
*/
	/*
	*** ======================= history.c
	*/
	char		**parse_history(void);
	void		add_to_history(t_all *all);
	void		display_index_cmd(t_all *all);
	void		goto_latest_commands(t_all *all);
	int			check_history_file(char **histo);

/* -------------------------------------------------------------
*** ============================================================ TERMCAPS
*/
	/*
	*** ======================= termcaps_init.c
	*/
	void		init_term(void);
	void		init_windows_size(t_all *all);
	int			init_tty(void);
	/*
	*** ======================= termcaps_tools.c
	*/
	t_all		*f_cpy(t_all *all);
	int			lu_putchar(int c);
	void		tputs_termcap(char *tc);
	void		restore_term(t_termios restore);
	void 		reset_term(void);

#endif