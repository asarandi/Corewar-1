/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_live.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/26 07:26:58 by sgardner          #+#    #+#             */
/*   Updated: 2018/10/27 07:35:11 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include <limits.h>

t_bool	op_live(t_core *core, t_proc *p)
{
	t_instr	*instr;
	t_uint	id;
	int		i;

	instr = &p->instr;
	instr->args[0] = instr->epc;
	instr->atypes[0] = T_D;
	id = read_data(core, instr, 0);
	i = 0;
	while (i < core->nplayers && core->champions[i].id != id)
		++i;
	if (i < core->nplayers)
	{
		p->champ = &core->champions[i];
		core->victor = p->champ;
		notice(ANNOUNCE_LIVE, UINT_MAX - core->victor->id, core->victor->name);
	}
	p->instr.epc = ABS_POS(core->arena, p->pc, DIR_SIZE);
	p->lived = TRUE;
	++core->lives;
	return (p->carry);
}
