#ifndef _ACTION_H_
#define _ACTION_H_

#include "board.h"
/* Searches stdin for a valid player move */
move get_player_move(void);
/* Checks user arrow shot for a HIT or MISS */
shot check_shot(board * brd, move m);
void display_help(void);
void display_intro(void);
void display_wall(void);
void display_unrecognized(void);
void display_win(void);
void display_loss(death d);
/* Scans stdin for EOF */
void clear_stdin(void);
#endif
