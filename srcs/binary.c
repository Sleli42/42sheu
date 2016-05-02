/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhatir <skhatir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/18 15:34:42 by lubaujar          #+#    #+#             */
/*   Updated: 2016/03/21 16:06:41 by skhatir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "full_sh.h"

void		exec_right_binary_loop(t_all *all, char **argv_bin)
{
	int		ct;
	char	*bin_tmp;

	ct = 0;
	bin_tmp = NULL;
	while (all->path2exec[ct])
	{
		if (!(bin_tmp = create_path(all->path2exec[ct], argv_bin[0])))
			break ;
		if (bin_tmp && good_access(bin_tmp))
		{
			exec_binary(bin_tmp, argv_bin, all->dupenv);
			add_to_hash_table(all, bin_tmp);
			break ;
		}
		bin_tmp ? ft_strdel(&bin_tmp) : NULL;
		ct++;
	}
	if (!all->path2exec[ct])
		write_error(argv_bin[0]);
}

void		exec_right_binary(t_all *all, char **argv_bin)
{
	(all->path2exec && *all->path2exec) ? del_array(&all->path2exec) : NULL;
	all->path2exec = ft_strsplit(find_env_arg(all, "PATH") + 5, ':');
	if (hash_exist(all->hash, argv_bin[0]) && argv_bin && argv_bin[0])
		exec_binary(all->hash[hash_bin(argv_bin[0])], argv_bin, all->dupenv);
	else
		exec_right_binary_loop(all, argv_bin);
	argv_bin ? del_array(&argv_bin) : NULL;
}

void		exec_binary(char *bin, char **argv_bin, char **env)
{
	int		buff;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		error("FORK");
	if (pid == 0)
	{
		if (execve(bin, argv_bin, env) == -1)
			error("EXECVE");
		exit(1);
	}
	if (pid > 0)
		waitpid(pid, &buff, 0);
}