#ifndef GAME_STATE_H
#define GAME_STATE_H
#include "player.h"


typedef enum {
    STATE_MAIN_MENU, /*start state i guess*/
    STATE_VILLAGE,
    STATE_OVERWORLD,
    STATE_FORKED_PATH,
    STATE_DARK_FOREST,
    STATE_BANDIT_CAMP,
    STATE_TOWER,
    STATE_PUZZLE,
    STATE_VOLCANO_DUNGEON,
    STATE_ICE_PUZZLE,
    STATE_CRYSTALS,
    STATE_MAZE,
    STATE_MASTER_SWORD,
    STATE_GUARDS,
    STATE_MINI_BOSS,
    STATE_FINAL_DUNGEON,
    STATE_FINAL_BOSS,
    STATE_VICTORY
} GameLocation;

typedef struct {
    Player player;
    GameLocation location;
} GameState;

const char* get_location_name(GameLocation loc);

#endif
