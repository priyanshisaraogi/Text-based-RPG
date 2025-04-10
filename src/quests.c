#include <stdio.h>
#include <string.h>
#include "player.h"

// Function declarations
void enter_old_sage_tower(Player *player);
int solve_puzzle();
void collect_crystals(Player *player);
void craft_master_sword(Player *player);

// Main test function
int main() {
    Player player;
    init_player(&player);
    display_stats(&player);

    printf("\nYou acquired the Mystic Herb from the Dark Forest!\n");
    player.has_mystic_herb = 1;

    printf("You obtained the Map Fragment from the Bandit Camp!\n");
    player.has_map_fragment = 1;

    enter_old_sage_tower(&player);

    if (player.has_fire_crystal && player.has_ice_crystal && player.has_shadow_crystal) {
        printf("\nYou now have all 3 crystals!\n");
        craft_master_sword(&player);
    } else {
        printf("\nYou need all 3 crystals before crafting the Master Sword.\n");
    }

    display_stats(&player);
    return 0;
}

void enter_old_sage_tower(Player *player) {
    printf("\nApproaching the Old Sage’s Tower...\n");

    if (!player->has_mystic_herb || !player->has_map_fragment) {
        printf("You cannot enter without the Mystic Herb and Map Fragment.\n");
        return;
    }

    printf("You used the Mystic Herb and Map Fragment to unlock the tower.\n");
    printf("Inside, you encounter a puzzle made of ancient clues...\n");

    int solved = solve_puzzle();

    if (solved) {
        printf("\nPuzzle solved! You learn the location of the crystals.\n");
        collect_crystals(player);
    } else {
        printf("\nThe puzzle confuses you. Try again later...\n");
    }
}

int solve_puzzle() {
    const char *clues[] = {
        "1. The truth lies not in the middle.",
        "2. A false path hides within a true one.",
        "3. Trust the one who contradicts themselves.",
        "4. Symmetry is not always honesty.",
        "5. Two truths and a lie walk into a riddle."
    };

    printf("\n--- Sage’s Puzzle ---\n");
    for (int i = 0; i < 5; ++i) {
        printf("%s\n", clues[i]);
    }

    printf("\nChoose 3 clues you believe are true (enter numbers separated by spaces): ");
    int c1, c2, c3;
    scanf("%d %d %d", &c1, &c2, &c3);

    // Let's say the correct clues are 1, 3, and 5
    if ((c1 == 1 || c2 == 1 || c3 == 1) &&
        (c1 == 3 || c2 == 3 || c3 == 3) &&
        (c1 == 5 || c2 == 5 || c3 == 5)) {
        return 1;
    }

    return 0;
}

void collect_crystals(Player *player) {
    printf("\nYou begin your journey to collect the crystals...\n");

    printf("\n🔥 Fire Crystal: You enter the volcano dungeon...\n");
    printf("You fight a lava beast and win!\n");
    player->has_fire_crystal = 1;
    gain_exp(player, 50);

    printf("\n❄️ Ice Crystal: Climbing the frozen mountain...\n");
    printf("You solve a freezing puzzle and defeat the guardian.\n");
    player->has_ice_crystal = 1;
    gain_exp(player, 50);

    printf("\n🕯 Shadow Crystal: Entering haunted ruins...\n");
    printf("You encounter a soul trapped in torment. Do you spare it? (y/n): ");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'y' || choice == 'Y') {
        printf("You spared the soul. It gifts you the Shadow Crystal.\n");
    } else {
        printf("You destroy the soul. The Shadow Crystal materializes.\n");
    }
    player->has_shadow_crystal = 1;
    gain_exp(player, 50);
}

void craft_master_sword(Player *player) {
    printf("\nYou visit the legendary Blacksmith...\n");

    printf("You must find Rare Ore to craft the Master Sword.\n");
    printf("You venture into a dangerous mine and retrieve the ore!\n");
    player->has_rare_ore = 1;

    if (player->has_rare_ore) {
        printf("Crafting Master Sword...\n");
        player->has_master_sword = 1;
        printf("*** Master Sword crafted and added to inventory! ***\n");
    }
}
