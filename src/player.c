#include "player.h"
#include <stdio.h>

void init_player(Player *player) {
    player->health = 100;
    player->attack = 10;
    player->defense = 5;
    player->level = 1;
    player->exp = 0;
    player->has_mystic_herb = 0;
    player->has_map_fragment = 0;
    player->has_fire_crystal = 0;
    player->has_ice_crystal = 0;
    player->has_shadow_crystal = 0;
    player->has_rare_ore = 0;
    player->has_master_sword = 0;
}

void display_stats(const Player *player) {
    printf("\nPlayer Stats:\n");
    printf("Health: %d\n", player->health);
    printf("Attack: %d\n", player->attack);
    printf("Defense: %d\n", player->defense);
    printf("Level: %d\n", player->level);
    printf("Experience: %d\n", player->exp);
    printf("\nInventory:\n");
    printf("Mystic Herb: %s\n", player->has_mystic_herb ? "Yes" : "No");
    printf("Map Fragment: %s\n", player->has_map_fragment ? "Yes" : "No");
    printf("Fire Crystal: %s\n", player->has_fire_crystal ? "Yes" : "No");
    printf("Ice Crystal: %s\n", player->has_ice_crystal ? "Yes" : "No");
    printf("Shadow Crystal: %s\n", player->has_shadow_crystal ? "Yes" : "No");
    printf("Rare Ore: %s\n", player->has_rare_ore ? "Yes" : "No");
    printf("Master Sword: %s\n", player->has_master_sword ? "Yes" : "No");
}

void gain_exp(Player *player, int exp_gain) {
    player->exp += exp_gain;
    printf("\nYou gained %d EXP. Total EXP is now %d.\n", exp_gain, player->exp);
    while (player->exp >= 100) {
        player->exp -= 100;
        player->level++;
        player->health += player->health / 2;   /* increase by 50% */
        player->attack += player->attack / 2;   /* increase by 50% */
        player->defense += player->defense / 2; /* increase by 50% */
        printf("\n*** LEVEL UP! You are now level %d! ***\n", player->level);
        display_stats(player);
    }
}
