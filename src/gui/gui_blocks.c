/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui_blocks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/07 20:48:57 by asarandi          #+#    #+#             */
/*   Updated: 2019/04/04 21:23:08 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gui.h"

int	gui_get_block_color(t_gui *g, int i, int j)
{
	int	owner;
	int	age;
	int	idx;

	idx = (i * GUI_BLOCK_NUM_COLS + j) % GUI_ARENA_SIZE;
	owner = g->core->owner[idx] % (MAX_PLAYERS + 1);
	age = g->core->epoch[idx] % (MAX_LUM_STEPS + 1);
	return (g->player_colors[owner][age]);
}

int	gui_put_block(t_gui *g, int i, int j)
{
	int	i_dst;
	int j_dst;
	int	rgb;
	uint32_t *ptr;
	int idx;

	rgb = gui_get_block_color(g, i, j);
	i = GUI_BLOCK_ARENA_X_POS + (i * GUI_BLOCK_ROW_HEIGHT);
	j = GUI_BLOCK_ARENA_Y_POS + (j * GUI_BLOCK_COL_WIDTH);
	i_dst = i + GUI_BLOCK_HEIGHT;
	j_dst = j + GUI_BLOCK_WIDTH;
	while (i < i_dst)
	{
		while (j < j_dst)
		{
			idx = (i * g->img_size) + (j * g->img_bpp / CHAR_BIT);
			ptr = (uint32_t *)&g->img_data[idx];
			*ptr = rgb | 0xff000000;
			j++;
		}
		j -= GUI_BLOCK_WIDTH;
		i++;
	}
	return (0);
}

int	gui_block_visuals(t_gui *g)
{
	int		i;
	int		j;
	int		k;

	(void)ft_memset(g->distrib, 0, (MAX_PLAYERS + 1) * sizeof(int));
	i = 0;
	while (i < GUI_BLOCK_NUM_ROWS)
	{
		j = 0;
		while (j < GUI_BLOCK_NUM_COLS)
		{
			(void)gui_put_block(g, i, j);
			k = g->core->owner[i * GUI_BLOCK_NUM_COLS + j];
			g->distrib[k]++;
			j++;
		}
		i++;
	}
	return (0);
}
