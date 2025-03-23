#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
    int health;
    int attack;
    int defense;
    int level;
    int exp;
} Player;

void init_player(Player *player);
void display_stats(const Player *player);
void gain_exp(Player *player, int exp_gain);

#endif // PLAYER_H