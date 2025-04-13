#include <stdio.h>
#include <stdlib.h>
#include "save.h"
#include "game_state.h"
#include "common.h"
#include "exploration.h"
#include "quests.h"
#include "combat.h"

extern GameState currentGameState;
extern Player mainPlayer;


int save_game(const GameState *state, const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) return 0;
    fwrite(state, sizeof(GameState), 1, fp);
    fclose(fp);
    return 1;
}

int load_game(GameState *state, const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return 0;
    fread(state, sizeof(GameState), 1, fp);
    fclose(fp);
    return 1;
}

void resume_from_loaded_state(void) {
    GameLocation loc = currentGameState.location;

    mainPlayer = currentGameState.player;
    display_stats(&mainPlayer); 


    switch (loc) {
        case STATE_MAIN_MENU:
            /*todo*/ 
            break;
        case STATE_VILLAGE:
        case STATE_OVERWORLD:
            overworld_exploration();
            break;
        case STATE_BANDIT_CAMP:
            bandit_camp_encounter();
            break;
        case STATE_DARK_FOREST:
            dark_forest_encounter();
            break;
        case STATE_TOWER:
            old_sage_tower();
            break;
        case STATE_PUZZLE:
            solve_puzzle();
            break;
        case STATE_VOLCANO_DUNGEON:
            combat_volcano_dungeon(&mainPlayer);
            break;
        case STATE_ICE_PUZZLE:
            solve_ice_puzzle();
            break;
        case STATE_CRYSTALS:
            collect_crystals(&mainPlayer);
            break;
        case STATE_MAZE:
            traverse_maze();
            break;
        case STATE_MASTER_SWORD:
            craft_master_sword(&mainPlayer);
            break;
        case STATE_GUARDS:
            solve_guard_riddle(&mainPlayer);
            break;
        case STATE_MINI_BOSS:
            mini_boss_fight(&mainPlayer);
            break;
        case STATE_FINAL_DUNGEON:
            final_dungeon(&mainPlayer);
            break;
        case STATE_FINAL_BOSS:
            final_boss_fight(&mainPlayer);
            break;
        default:
            printf("⚠ Unknown save location. Restarting from overworld...\n");
            overworld_exploration();
            break;
    }
}

void handle_load_game(void) {
    char filename[32];
    GameState temp;
    int slot;
    int valid_slots[5] = {0};
    int found_any = 0;

    printf("\n--- Saved Slots ---\n\n");

    for (int i = 1; i <= 4; i++) {
        sprintf(filename, "save_slot%d.dat", i);
        FILE *fp = fopen(filename, "rb");

        if (fp != NULL) {
            if (fread(&temp, sizeof(GameState), 1, fp) == 1) {
                printf("Slot %d: %s\n", i, get_location_name(temp.location));
                valid_slots[i] = 1;
                found_any = 1;
            } else {
                printf("Slot %d: [Corrupted Save]\n", i);
            }
            fclose(fp);
        } else {
            printf("Slot %d: [Empty]\n", i);
        }
    }

    if (!found_any) {
        printf("\nNo save files found.\n");
        printf("----------------------\n");
        return;
    }

    printf("\n----------------------\n");
    slot = get_int_input("Enter slot number to load (1–4): ");

    if (slot < 1 || slot > 4) {
        printf("Invalid slot number.\n");
        return;
    }

    if (!valid_slots[slot]) {
        printf("Selected slot is empty or corrupted.\n");
        return;
    }

    sprintf(filename, "save_slot%d.dat", slot);
    if (load_game(&currentGameState, filename)) {
        printf("\nGame loaded successfully!\n");
        printf("  Slot %d\n", slot);
        printf("  Location: %s\n\n", get_location_name(currentGameState.location));

        resume_from_loaded_state(); 
    } else {
        printf("Failed to load save file.\n");
    }
}
