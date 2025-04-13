#ifndef SAVE_H
#define SAVE_H

#include "game_state.h"

int save_game(const GameState *state, const char *filename);
int load_game(GameState *state, const char *filename);
void handle_load_game(void);

#endif
