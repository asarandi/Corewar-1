/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui_text.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 22:31:03 by asarandi          #+#    #+#             */
/*   Updated: 2019/04/04 20:30:36 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gui.h"

#define SFML_PUTS   sfml_puts

void    sfml_puts(t_gui *g, int y, int x, int c, char *s)
{
    sfColor     color;
    sfVector2i  point;
    sfVector2f  position;
    sfText      *text;

    text = sfText_create();
    sfText_setString(text, s);
    sfText_setFont(text, g->font);
    sfText_setCharacterSize(text, 20); /* XXX */

    color = sfColor_fromRGB((c >> 16) & 0xff, (c >> 8) & 0xff, c & 0xff);
    sfText_setColor(text, color);

    point.x = y;
    point.y = x;
    position = sfRenderWindow_mapPixelToCoords(g->window, point, NULL);

    sfText_setPosition(text, position);
    sfRenderWindow_drawText(g->window, text, NULL);
    sfText_destroy(text);
}



int	gui_text_winner(t_gui *g, int x, int y, char *s)
{
	int	i;
	int	c;

	i = 0;
	c = 0xffffff;
	while (i < MAX_PLAYERS)
	{
		if (g->core->victor == &g->core->champions[i])
		{
			SFML_PUTS(g, y, x, c, GUI_WINNER1);
			x += GUI_CHAR_HEIGHT;
			ft_sprintf(s, GUI_STRING, MSL, g->core->victor->name);
			SFML_PUTS(g, y, x, g->player_colors[i + 1][0], s);
			x += GUI_CHAR_HEIGHT;
			ft_sprintf(s, GUI_COMMENT_QUOTES, MSL - 3,
				g->core->victor->comment);
			SFML_PUTS(g, y, x, g->player_colors[i + 1][0], s);
			break ;
		}
		i++;
	}
	return (0);
}

int	gui_text_stats(t_gui *g, int x, int y, char *s)
{
	int	c;

	c = INFO_TEXT_COLOR;
	ft_sprintf(s, FPS_STR, g->fps, g->cpf, g->fps * g->cpf);
	SFML_PUTS(g, y, x, c, s);
	x += GUI_CHAR_HEIGHT * 3;
	ft_sprintf(s, PROC_NUM, g->nprocesses);
	SFML_PUTS(g, y, x, c, s);
	x += GUI_CHAR_HEIGHT;
	ft_sprintf(s, CYCLE_NUM, g->core->cycle);
	SFML_PUTS(g, y, x, c, s);
	x += GUI_CHAR_HEIGHT * 2;
	ft_sprintf(s, CYCLE_TO_DIE_STR, g->core->cull.ctd);
	SFML_PUTS(g, y, x, c, s);
	x += GUI_CHAR_HEIGHT;
	ft_sprintf(s, CYCLE_DELTA_STR, CYCLE_DELTA);
	SFML_PUTS(g, y, x, c, s);
	x += GUI_CHAR_HEIGHT * 2;
	ft_sprintf(s, NBR_LIVE_STR, g->core->cull.nbr_lives, NBR_LIVE);
	SFML_PUTS(g, y, x, c, s);
	x += GUI_CHAR_HEIGHT;
	ft_sprintf(s, MAX_CHECKS_STR, g->core->cull.checks, MAX_CHECKS);
	SFML_PUTS(g, y, x, c, s);
	return (x + GUI_CHAR_HEIGHT * 3);
}

int	gui_text_players(t_gui *g, int x, int y, char *s)
{
	int	i;
	int	c;

	i = 0;
	c = INFO_TEXT_COLOR;
	while (i < g->core->nplayers)
	{
		ft_sprintf(s, PLAYER_NUM, i, g->core->champions[i].id);
		SFML_PUTS(g, y, x, c, s);
		x += GUI_CHAR_HEIGHT;
		ft_sprintf(s, GUI_STRING, MSL, g->core->champions[i].name);
		SFML_PUTS(g, y, x, g->player_colors[i + 1][0], s);
		x += GUI_CHAR_HEIGHT;
		ft_sprintf(s, LAST_LIVE_STR, g->core->champions[i].last_live);
		SFML_PUTS(g, y, x, c, s);
		x += GUI_CHAR_HEIGHT;
		ft_sprintf(s, LIVES_IN_PERIOD_STR, g->core->champions[i].plives);
		SFML_PUTS(g, y, x, c, s);
		x += GUI_CHAR_HEIGHT * 2;
		i++;
	}
	return (x + GUI_CHAR_HEIGHT);
}

int	gui_info_panel(t_gui *g)
{
	char	text[COMMENT_LENGTH + 1];
	int		x;
	int		y;

	y = INFO_PANEL_Y + 10;
	x = INFO_PANEL_X;
	if (g->state == 1)
		SFML_PUTS(g, y, x, INFO_TEXT_COLOR, STATE_RUNNING);
	else
		SFML_PUTS(g, y, x, INFO_TEXT_COLOR, STATE_PAUSED);
	x += GUI_CHAR_HEIGHT;
	x = gui_text_stats(g, x, y, &text[0]);
	x = gui_text_players(g, x, y, &text[0]);
	if (g->core->processes == 0)
		(void)gui_text_winner(g, x, y, &text[0]);
	SFML_PUTS(g, LIVE_BAR_TEXT_Y_POS,
		LIVE_BAR_TEXT_X_POS, INFO_TEXT_COLOR, LIVE_BAR_TEXT_STR);
	SFML_PUTS(g, DIST_TEXT_Y_POS,
		DIST_TEXT_X_POS, INFO_TEXT_COLOR, DIST_TEXT);
	return (0);
}
