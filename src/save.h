#ifndef SAVE_H
#define SAVE_H
#define SAVE_MAGIC "RPG_SAVE"
#define SAVE_VERSION 1

#include "game_state.h"

int save_game(const GameState *state, const char *filename);
int load_game(GameState *state, const char *filename);
int handle_load_game(void);
void resume_from_loaded_state(void);

#endif
