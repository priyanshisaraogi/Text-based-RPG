#include <stdio.h>
#include <stdlib.h> 
#include "common.h"
#include "save.h"
#include "player.h"
#include "exploration.h"

extern GameState currentGameState;
extern Player mainPlayer;

const char* get_location_name(GameLocation loc);

void wait_for_enter(void) {
    int c;

    while ((c = getchar()) != '\n') {
        /* Consume extra characters */
        if (c == 'p' || c == 'P') {
            handle_pause_menu();
            return;
        }
    }
}

void print_pause(const char *line) {
    printf("\n%s\n", line);
    wait_for_enter();
}

int get_int_input(const char *prompt) {
    int choice;
    char input[128];
    char *endptr;
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) != NULL) {
            if (input[0] == 'p' || input[0] == 'P') {
                handle_pause_menu();
                continue;
            }
            choice = strtol(input, &endptr, 10);
            if (endptr == input || (*endptr != '\n' && *endptr != '\0')) {
                printf("Wrong input, please choose a numeric value.\n");
                continue;
            }
            return choice;
        } else {
            printf("Input error, please try again.\n");
        }
    }
}

void handle_pause_menu(void) {
    int option = 0;
    printf("\n--- GAME PAUSED ---\n");
    while (1) {
        printf("1. Resume Game\n");
        printf("2. Save Game\n");
        printf("3. Restart Game\n");
        printf("4. Exit Game\n");
        printf("Enter option: ");
        if (scanf("%d", &option) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Try again.\n");
            continue;
        }
        while (getchar() != '\n');

        if (option == 1) {
            return;
        } else if (option == 2) {
            int slot = 0;
            int format_choice;
            char filename[64];
            const char *ext;
            const char* location_name;

            printf("\nChoose a save slot:\n");
            printf("1. Save Slot 1\n");
            printf("2. Save Slot 2\n");
            printf("3. Save Slot 3\n");
            printf("4. Save Slot 4\n");

            slot = get_int_input("Enter slot number: ");
            if (slot < 1 || slot > 4) {
                printf("Invalid slot. Save cancelled.\n");
                continue;
            }

            printf("\nSelect save format:\n");
            printf("1. Text (.txt)\n");
            printf("2. Binary (.dat)\n");
            format_choice = get_int_input("Enter choice (1-2): ");
            if (format_choice < 1 || format_choice > 2) {
                printf("Invalid choice. Save cancelled.\n");
                continue;
            }

            ext = (format_choice == 1) ? ".txt" : ".dat";
            sprintf(filename, "save_slot%d%s", slot, ext);

            currentGameState.player = mainPlayer;
            location_name = get_location_name(currentGameState.location);

            if (save_game(&currentGameState, filename)) {
                printf("\nGame saved to:\n");
                printf("  Slot %d\n", slot);
                printf("  Location: %s\n", location_name);
                printf("  Format: %s\n\n", ext);
            } else {
                printf("Failed to save game.\n");
            }

        } else if (option == 3) {
            printf("\nRestarting game...\n");
            reset_player_state();
            currentGameState.location = STATE_VILLAGE;
            print_backstory();
            return;

        } else if (option == 4) {
            printf("Farewell, hero.\n");
            exit(0);
        } else {
            printf("Invalid option.\n");
        }
    }
}


const char* get_location_name(GameLocation loc) {
    switch (loc) {
        case STATE_VILLAGE: return "VILLAGE";
        case STATE_OVERWORLD: return "OVERWORLD";
        case STATE_FORKED_PATH: return "FORKED PATH";
        case STATE_BANDIT_CAMP: return "BANDIT CAMP";
        case STATE_DARK_FOREST: return "DARK_FOREST";
        case STATE_OLD_SAGE_TOWER: return "OLD SAGE TOWER";
        case STATE_VOLCANO_DUNGEON: return "VOLCANO DUNGEON";
        case STATE_ICE_PUZZLE: return "ICE PUZZLE";
        case STATE_HAUNTED_RUINS: return "HAUNTED RUINS";
        case STATE_CRYSTALS: return "CRYSTALS";
        case STATE_MAZE: return "MAZE";
        case STATE_MASTER_SWORD: return "MASTER SWORD";
        case STATE_GUARDS: return "TWO GUARDS";
        case STATE_MINI_BOSS: return "MINIBOSS";
        case STATE_FINAL_DUNGEON: return "FINAL DUNGEON";
        case STATE_FINAL_BOSS: return "FINAL BOSS";
        case STATE_VICTORY: return "ENDING SCENE";
        default: return "UNKNOWN";
    }
} 