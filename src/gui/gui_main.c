/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 22:32:23 by asarandi          #+#    #+#             */
/*   Updated: 2019/04/04 20:03:24 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gui.h"
#include "corewar.h"

int	gui_clean_up(t_gui *g, char *msg)
{
    if (msg)
        ft_printf("%s\n", msg);
    if (g->music)
        sfMusic_destroy(g->music);
    if (g->font)
        sfFont_destroy(g->font);
    if (g->sprite)
        sfSprite_destroy(g->sprite);
    if (g->window)
        sfRenderWindow_destroy(g->window);    
	free(g);
	return (0);
}

int	gui_init(t_core *core)
{
	t_gui	*g;
    bool    running;

	g = ft_memalloc(sizeof(t_gui));
	g->core = core;

    g->mode.width = WIN_TOTAL_WIDTH;
    g->mode.height = WIN_TOTAL_HEIGHT;
    g->mode.bitsPerPixel = 32;
    g->img_bpp = g->mode.bitsPerPixel;
    g->img_size = g->mode.width * (g->mode.bitsPerPixel / CHAR_BIT);

    g->window = sfRenderWindow_create(g->mode, WIN_BLOCK_TITLE, sfClose, NULL);
    if (!g->window)
        return gui_clean_up(g, "error: sfRenderWindow_create() failed");

    g->font = sfFont_createFromFile("res/font.ttf");
    if (!g->font)
        return gui_clean_up(g, "error: sfFont_createFromFile() failed");
    g->music = sfMusic_createFromFile("res/music.ogg");
    if (!g->music)
        return gui_clean_up(g, "error: sfMusic_createFromFile() failed");
    sfMusic_play(g->music);
    sfMusic_setLoop(g->music, TRUE);

    g->sprite = sfSprite_create();
    g->img_data = ft_memalloc(g->mode.width * g->mode.height * 4);
	(void)gui_init_colors(g);
    
    g->cpf = 1;
    running = true;
    while (running && sfRenderWindow_isOpen(g->window))
    {
        while (sfRenderWindow_pollEvent(g->window, &g->event))
        {
            if (g->event.type == sfEvtClosed)
                sfRenderWindow_close(g->window);

            if (g->event.type == sfEvtKeyPressed) {
                if ((g->event.key.code == sfKeyQ) || (g->event.key.code == sfKeyEscape))
                    running = false;

                else if (g->event.key.code == sfKeySpace)
                    g->state ^= 1;

                else if (gui_is_numeric_key(g->event.key.code))
                    (void)gui_set_cpf_to_numeric_key(g->event.key.code, g);

                else if (sfKeyboard_isKeyPressed(sfKeyAdd))
                    (void)gui_increment_cpf(sfKeyAdd, g);
                
                else if (sfKeyboard_isKeyPressed(sfKeySubtract))
                    (void)gui_increment_cpf(sfKeySubtract, g);

//               ft_printf("you pressed %08x\n", g->event.key.code);
            }
        }

        for (int i=0; g->state && (i < g->cpf); i++) {
            if (g->core->processes)
                execute_war(g->core);
        }

        (void)ft_memset(g->img_data, 0, g->mode.width * g->mode.height * (g->mode.bitsPerPixel / CHAR_BIT));
        (void)gui_pc_boxes(g);
        (void)gui_block_visuals(g);
        (void)gui_live_bar(g);
        (void)gui_dist_bar(g);
        (void)gui_calc_fps(g);

        g->image = sfImage_createFromPixels(g->mode.width, g->mode.height, (sfUint8*)g->img_data);
        g->texture = sfTexture_createFromImage(g->image, NULL); //null to load entire image
        sfSprite_setTexture(g->sprite, g->texture, TRUE);

        sfRenderWindow_clear(g->window, sfBlack);
        sfRenderWindow_drawSprite(g->window, g->sprite, NULL);
        
        (void)gui_info_panel(g);
        sfRenderWindow_display(g->window);

        sfTexture_destroy(g->texture);
        sfImage_destroy(g->image);
    }
	(void)gui_clean_up(g, NULL);
	return (0);
}
