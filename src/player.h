#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
    int health;
    int attack;
    int defense;
    int level;
    int exp;
    int has_mystic_herb;
    int has_map_fragment;
    int has_fire_crystal;
    int has_ice_crystal;
    int has_shadow_crystal;
    int has_rare_ore;
    int has_master_sword;
} Player;

void init_player(Player *player);
void display_stats(const Player *player);
void gain_exp(Player *player, int exp_gain);

#endif // PLAYER_H