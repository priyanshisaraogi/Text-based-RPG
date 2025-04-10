#include <stdio.h>
#include <string.h>
#include "player.h"
#include "quests.h"

/* Wait for the player to press the enter key before continuing */
void wait_for_enter(void) {
    int c;
    while ((c = getchar()) != '\n') {
        /* Consume extra characters */
    }
}

/* Print a line then pause for user input */
void print_pause(const char *line) {
    printf("\n%s\n", line);
    wait_for_enter();
}

void enter_old_sage_tower(Player *player) {
    if (!player->has_mystic_herb || !player->has_map_fragment) {
        printf("You cannot enter without the Mystic Herb and Map Fragment.\n");
        return;
    }

    print_pause("In the old sage tower, your mission is to find the 3 Crystals of Power:");
    print_pause(" - The Fire Crystal,");
    print_pause(" - The Ice Crystal,");
    print_pause(" - and the Shadow Crystal.");
    print_pause("With the three crytals collected, you must find a way to forge them into the Master Sword...but don't worry, that comes later.");
    print_pause("First thing first, you need to find clues as to where the crystals are hidden.");
    print_pause("As you start to rummage around, you find a puzzle made of ancient riddles that seems to point towards the location of the crystals. You study it carefully...");

    int solved = 0;
    /* Loop until the puzzle is solved correctly. */
    while (!solved) {
        solved = solve_puzzle();
        if (!solved) {
            print_pause("The puzzle confounds you. The clues are shown again. Try again dummy.");
        }
    }
    
    print_pause("Puzzle solved! You learn the location of the crystals.");
    print_pause("You start heading upstairs towards the first crystal: The Fire Crystal.");
    collect_crystals(player);
}

int solve_puzzle() {
    const char *clues[] = {
        "1. In the chamber where flames burn without respite, the power of heat endures.",
        "2. In the hall of brilliant light, no shadow dares to linger.",
        "3. In the frozen vestibule, where silence is the language, cold whispers abound.",
        "4. In the depths of the earthen vaults, false promises lie buried.",
        "5. In the corridor of perpetual dusk, the veil of darkness conceals ancient secrets."
    };

    printf("\n--- Sage's Puzzle ---\n");
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
    print_pause("You explore the old sage tower with the clues in mind.");
    while (!(player->has_fire_crystal && player->has_ice_crystal && player->has_shadow_crystal)) {
        int room_choice = 0;
        int result;
        
        printf("\nChoose a room to enter: (");
        int first = 1;
        if (!player->has_fire_crystal) {
            printf("1 = Volcano Dungeon");
            first = 0;
        }
        if (!player->has_ice_crystal) {
            if (!first) {
                printf(", ");
            }
            printf("2 = Frozen Mountain");
            first = 0;
        }
        if (!player->has_shadow_crystal) {
            if (!first) {
                printf(", ");
            }
            printf("3 = Haunted Ruins");
        }
        printf("): ");
        
        result = scanf("%d", &room_choice);
        while(getchar() != '\n');
        
        if(result != 1 || room_choice < 1 || room_choice > 3) {
            printf("Invalid input. Please enter 1, 2, or 3.\n");
            continue;
        }
        
        /* Check if the selected room has already been visited. */
        if ((room_choice == 1 && player->has_fire_crystal) ||
            (room_choice == 2 && player->has_ice_crystal) ||
            (room_choice == 3 && player->has_shadow_crystal)) {
            printf("\nYou have already collected that crystal. Please choose a different room.\n");
            continue;
        }
        
        switch(room_choice) {
            case 1:
                print_pause("🔥 Fire Crystal: You enter the volcano dungeon...");
                printf("You fight a lava beast and win!\n");
                player->has_fire_crystal = 1;
                gain_exp(player, 50);
                break;
            case 2:
                print_pause("❄️ Ice Crystal: Climbing the frozen mountain...");
                printf("You solve a freezing puzzle and defeat the guardian.\n");
                player->has_ice_crystal = 1;
                gain_exp(player, 50);
                break;
            case 3: {
                print_pause("🕯 Shadow Crystal: Entering haunted ruins...");
                char choice;
                do {
                    printf("You encounter a soul trapped in torment. It begs you to free it in exchange for the Shadow Crystal. Do you spare it? (y/n): ");
                    result = scanf(" %c", &choice);
                    while(getchar() != '\n');
                    if(result != 1 || (choice!='y' && choice!='Y' && choice!='n' && choice!='N')) {
                        printf("Invalid input. Please choose either y or n.\n");
                    } else {
                        break;
                    }
                } while(1);
                
                if (choice=='y' || choice=='Y') {
                    print_pause("You spared the soul. It gifts you the Shadow Crystal in gratitude and starts to ascend to heaven...");
                    print_pause("...but all of a sudden a red light shines from the ground and the soul is pulled back down! Oops!");
                } else {
                    print_pause("You destroy the soul. The Shadow Crystal materializes and you collect it...");
                    print_pause("...but because of you, the soul is doomed to oblivion, never to enter any afterlife.");
                    print_pause("Congratulations, you are a monster!");
                }
                player->has_shadow_crystal = 1;
                gain_exp(player, 50);
                break;
            }
        }
    } 
    
    print_pause("You now have all 3 crystals in your inventory!");
    craft_master_sword(player);
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

void run_quests(Player *player) {
    enter_old_sage_tower(player);
    collect_crystals(player);

    if (player->has_fire_crystal && player->has_ice_crystal && player->has_shadow_crystal) {
        printf("\nYou now have all 3 crystals!\n");
        craft_master_sword(player);
    } else {
        printf("\nYou need all 3 crystals before crafting the Master Sword.\n");
    }

    display_stats(player);
}
