#include "player.h"
#include <stdio.h>

void init_player(Player *player) {
    player->health = 100;
    player->attack = 10;
    player->defense = 5;
    player->level = 1;
    player->exp = 0;
}

void display_stats(const Player *player) {
    printf("\nPlayer Stats:\n");
    printf("Health: %d\n", player->health);
    printf("Attack: %d\n", player->attack);
    printf("Defense: %d\n", player->defense);
    printf("Level: %d\n", player->level);
    printf("Experience: %d\n", player->exp);
}

void gain_exp(Player *player, int exp_gain) {
    player->exp += exp_gain;
    printf("\nYou gained %d EXP. Total EXP is now %d.\n", exp_gain, player->exp);
    while (player->exp >= 100) {
        player->exp -= 100;
        player->level++;
        player->health += player->health / 2;   // increase by 50%
        player->attack += player->attack / 2;   // increase by 50%
        player->defense += player->defense / 2; // increase by 50%
        printf("\n*** LEVEL UP! You are now level %d! ***\n", player->level);
        display_stats(player);
    }
}