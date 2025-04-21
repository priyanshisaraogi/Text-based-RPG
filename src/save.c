#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "save.h"
#include "game_state.h"
#include "common.h"
#include "exploration.h"
#include "quests.h"
#include "combat.h"

extern GameState currentGameState;
extern Player mainPlayer;

unsigned short calculate_checksum(const GameState *state) {
    unsigned short checksum = 0;
    checksum += state->location;
    checksum += state->player.health + state->player.attack + state->player.defense;
    checksum += state->player.level + state->player.exp;
    checksum += state->player.has_mystic_herb + state->player.has_map_fragment;
    checksum += state->player.has_fire_crystal + state->player.has_ice_crystal;
    checksum += state->player.has_shadow_crystal + state->player.has_rare_ore;
    checksum += state->player.has_master_sword;
    return checksum;
}

int save_game_text(const GameState *state, const char *filename) {
    char fullpath[64];
    FILE *fp;

    sprintf(fullpath, "src/%s", filename);

    fp = fopen(fullpath, "w");
    if (!fp) {
        printf("Error: Could not open file %s for writing.\n", fullpath);
        return 0;
    }

    fprintf(fp, "magic=%s\n", SAVE_MAGIC);
    fprintf(fp, "version=%d\n", SAVE_VERSION);
    fprintf(fp, "location=%d\n", state->location);
    fprintf(fp, "player_health=%d\n", state->player.health);
    fprintf(fp, "player_attack=%d\n", state->player.attack);
    fprintf(fp, "player_defense=%d\n", state->player.defense);
    fprintf(fp, "player_level=%d\n", state->player.level);
    fprintf(fp, "player_exp=%d\n", state->player.exp);
    fprintf(fp, "player_has_mystic_herb=%d\n", state->player.has_mystic_herb);
    fprintf(fp, "player_has_map_fragment=%d\n", state->player.has_map_fragment);
    fprintf(fp, "player_has_fire_crystal=%d\n", state->player.has_fire_crystal);
    fprintf(fp, "player_has_ice_crystal=%d\n", state->player.has_ice_crystal);
    fprintf(fp, "player_has_shadow_crystal=%d\n", state->player.has_shadow_crystal);
    fprintf(fp, "player_has_rare_ore=%d\n", state->player.has_rare_ore);
    fprintf(fp, "player_has_master_sword=%d\n", state->player.has_master_sword);
    fprintf(fp, "checksum=%u\n", calculate_checksum(state));

    fclose(fp);
    return 1;
}


int save_game_binary(const GameState *state, const char *filename) {
    char fullpath[64];
    FILE *fp;
    int version;
    unsigned short checksum;

    sprintf(fullpath, "src/%s", filename);

    fp = fopen(fullpath, "wb");
    if (!fp) {
        printf("Error: Could not open file %s for writing.\n", fullpath);
        return 0;
    }

    version = SAVE_VERSION;
    fwrite(SAVE_MAGIC, sizeof(char), strlen(SAVE_MAGIC) + 1, fp);
    fwrite(&version, sizeof(int), 1, fp);

    fwrite(&state->location, sizeof(GameLocation), 1, fp);
    fwrite(&state->player.health, sizeof(int), 1, fp);
    fwrite(&state->player.attack, sizeof(int), 1, fp);
    fwrite(&state->player.defense, sizeof(int), 1, fp);
    fwrite(&state->player.level, sizeof(int), 1, fp);
    fwrite(&state->player.exp, sizeof(int), 1, fp);
    fwrite(&state->player.has_mystic_herb, sizeof(int), 1, fp);
    fwrite(&state->player.has_map_fragment, sizeof(int), 1, fp);
    fwrite(&state->player.has_fire_crystal, sizeof(int), 1, fp);
    fwrite(&state->player.has_ice_crystal, sizeof(int), 1, fp);
    fwrite(&state->player.has_shadow_crystal, sizeof(int), 1, fp);
    fwrite(&state->player.has_rare_ore, sizeof(int), 1, fp);
    fwrite(&state->player.has_master_sword, sizeof(int), 1, fp);

    checksum = calculate_checksum(state);
    fwrite(&checksum, sizeof(unsigned short), 1, fp);

    fclose(fp);
    return 1;
}


int save_game(const GameState *state, const char *filename) {
    const char *ext = strrchr(filename, '.');
    if (ext && strcmp(ext, ".dat") == 0) {
        return save_game_binary(state, filename);
    }
    return save_game_text(state, filename);
}

typedef enum {
    PARSE_HEADER,
    PARSE_DATA,
    PARSE_CHECKSUM,
    PARSE_ERROR
} ParseState;

int load_game_text(GameState *state, const char *filename) {
    FILE *fp = fopen(filename, "r");
    ParseState parse_state;
    char line[128];
    int version;
    int fields_parsed;
    const int expected_fields = 13;
    char *key;
    char *value;
    unsigned short expected_checksum;
    unsigned short actual_checksum;

    if (!fp) {
        printf("Error: Could not open file %s for reading.\n", filename);
        return 0;
    }

    parse_state = PARSE_HEADER;
    version = 0;
    expected_checksum = 0;
    fields_parsed = 0;

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0'; 
        if (strlen(line) == 0) continue; 

        key = strtok(line, "=");
        value = strtok(NULL, "=");
        if (!key || !value) {
            parse_state = PARSE_ERROR;
            break;
        }

        switch (parse_state) {
            case PARSE_HEADER:
                if (strcmp(key, "magic") == 0) {
                    if (strcmp(value, SAVE_MAGIC) != 0) {
                        printf("Error: Invalid save file magic string.\n");
                        parse_state = PARSE_ERROR;
                        break;
                    }
                } else if (strcmp(key, "version") == 0) {
                    version = atoi(value);
                    if (version != SAVE_VERSION) {
                        printf("Error: Unsupported save file version %d.\n", version);
                        parse_state = PARSE_ERROR;
                        break;
                    }
                    parse_state = PARSE_DATA;
                } else {
                    parse_state = PARSE_ERROR;
                }
                break;

            case PARSE_DATA:
                if (strcmp(key, "checksum") == 0) {
                    expected_checksum = (unsigned short)atoi(value);
                    parse_state = PARSE_CHECKSUM;
                    break;
                }
                if (strcmp(key, "location") == 0) {
                    state->location = (GameLocation)atoi(value);
                    if (state->location < STATE_MAIN_MENU || state->location > STATE_VICTORY) {
                        printf("Error: Invalid location value.\n");
                        parse_state = PARSE_ERROR;
                        break;
                    }
                } else if (strcmp(key, "player_health") == 0) {
                    state->player.health = atoi(value);
                    if (state->player.health < 0) {
                        printf("Error: Invalid player health value.\n");
                        parse_state = PARSE_ERROR;
                        break;
                    }
                } else if (strcmp(key, "player_attack") == 0) {
                    state->player.attack = atoi(value);
                } else if (strcmp(key, "player_defense") == 0) {
                    state->player.defense = atoi(value);
                } else if (strcmp(key, "player_level") == 0) {
                    state->player.level = atoi(value);
                    if (state->player.level < 1) {
                        printf("Error: Invalid player level.\n");
                        parse_state = PARSE_ERROR;
                        break;
                    }
                } else if (strcmp(key, "player_exp") == 0) {
                    state->player.exp = atoi(value);
                } else if (strcmp(key, "player_has_mystic_herb") == 0) {
                    state->player.has_mystic_herb = atoi(value);
                } else if (strcmp(key, "player_has_map_fragment") == 0) {
                    state->player.has_map_fragment = atoi(value);
                } else if (strcmp(key, "player_has_fire_crystal") == 0) {
                    state->player.has_fire_crystal = atoi(value);
                } else if (strcmp(key, "player_has_ice_crystal") == 0) {
                    state->player.has_ice_crystal = atoi(value);
                } else if (strcmp(key, "player_has_shadow_crystal") == 0) {
                    state->player.has_shadow_crystal = atoi(value);
                } else if (strcmp(key, "player_has_rare_ore") == 0) {
                    state->player.has_rare_ore = atoi(value);
                } else if (strcmp(key, "player_has_master_sword") == 0) {
                    state->player.has_master_sword = atoi(value);
                } else {
                    printf("Error: Unknown key %s in save file.\n", key);
                    parse_state = PARSE_ERROR;
                    break;
                }
                fields_parsed++;
                break;

            case PARSE_CHECKSUM:
                parse_state = PARSE_ERROR;
                break;

            case PARSE_ERROR:
                break;
        }
    }

    fclose(fp);

    if (parse_state == PARSE_ERROR) {
        printf("Error: Failed to parse save file.\n");
        return 0;
    }

    if (fields_parsed != expected_fields) {
        printf("Error: Incomplete save file, only %d/%d fields parsed.\n", fields_parsed, expected_fields);
        return 0;
    }

    actual_checksum = calculate_checksum(state);
    if (actual_checksum != expected_checksum) {
        printf("Error: Checksum mismatch. Save file may be corrupted.\n");
        return 0;
    }

    return 1;
}

int load_game_binary(GameState *state, const char *filename) {
    int version;
    unsigned short expected_checksum;
    unsigned short actual_checksum;
    char magic[sizeof(SAVE_MAGIC)];

    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("Error: Could not open file %s for reading.\n", filename);
        return 0;
    }
    
    fread(magic, sizeof(char), strlen(SAVE_MAGIC) + 1, fp);
    if (strcmp(magic, SAVE_MAGIC) != 0) {
        printf("Error: Invalid save file magic string.\n");
        fclose(fp);
        return 0;
    }

    fread(&version, sizeof(int), 1, fp);
    if (version != SAVE_VERSION) {
        printf("Error: Unsupported save file version %d.\n", version);
        fclose(fp);
        return 0;
    }

    fread(&state->location, sizeof(GameLocation), 1, fp);
    fread(&state->player.health, sizeof(int), 1, fp);
    fread(&state->player.attack, sizeof(int), 1, fp);
    fread(&state->player.defense, sizeof(int), 1, fp);
    fread(&state->player.level, sizeof(int), 1, fp);
    fread(&state->player.exp, sizeof(int), 1, fp);
    fread(&state->player.has_mystic_herb, sizeof(int), 1, fp);
    fread(&state->player.has_map_fragment, sizeof(int), 1, fp);
    fread(&state->player.has_fire_crystal, sizeof(int), 1, fp);
    fread(&state->player.has_ice_crystal, sizeof(int), 1, fp);
    fread(&state->player.has_shadow_crystal, sizeof(int), 1, fp);
    fread(&state->player.has_rare_ore, sizeof(int), 1, fp);
    fread(&state->player.has_master_sword, sizeof(int), 1, fp);

    if (state->location < STATE_MAIN_MENU || state->location > STATE_VICTORY ||
        state->player.health < 0 || state->player.level < 1) {
        printf("Error: Invalid data in binary save file.\n");
        fclose(fp);
        return 0;
    }

    fread(&expected_checksum, sizeof(unsigned short), 1, fp);
    actual_checksum = calculate_checksum(state);
    if (actual_checksum != expected_checksum) {
        printf("Error: Checksum mismatch in binary save file.\n");
        fclose(fp);
        return 0;
    }

    fclose(fp);
    return 1;
}

int load_game(GameState *state, const char *filename) {
    const char *ext = strrchr(filename, '.');
    if (ext && strcmp(ext, ".dat") == 0) {
        return load_game_binary(state, filename);
    }
    return load_game_text(state, filename);
}

int handle_load_game(void) {
    char filename[64];
    GameState temp;
    int slot;
    int format_choice;
    const char *ext;
    int i;
    FILE *fp;
    int retry;
    int valid_slots[5];
    int found_any;

    while (1) {
        for (i = 0; i < 5; i++) {
            valid_slots[i] = 0;
        }
        found_any = 0;

        printf("\n--- Load Game ---\n\n");
        printf("Select file format to load:\n");
        printf("1. Text (.txt)\n");
        printf("2. Binary (.dat)\n");
        format_choice = get_int_input("Enter choice (1–2): ");

        if (format_choice == 1) {
            ext = ".txt";
        } else if (format_choice == 2) {
            ext = ".dat";
        } else {
            printf("Invalid choice. Returning to main menu.\n");
            return 0;
        }

        printf("\n--- Saved Slots (%s) ---\n\n", ext);

        for (i = 1; i <= 4; i++) {
            sprintf(filename, "src/save_slot%d%s", i, ext);
            fp = fopen(filename, (format_choice == 1) ? "r" : "rb");

            if (fp) {
                if (load_game(&temp, filename)) {
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
            printf("\nNo save files found for %s format.\n", ext);

            while (1) {
                printf("Would you like to try a different format?\n");
                printf("1. Yes\n");
                printf("2. No (Return to main menu)\n");
                retry = get_int_input("Enter choice: ");
                if (retry == 1) {
                    break;  /* Retry file format selection */
                } else if (retry == 2) {
                    return 0;
                } else {
                    printf("Invalid input. Please choose 1 or 2.\n");
                }
            }
            continue;
        }

        printf("\n----------------------\n");
        slot = get_int_input("Enter slot number to load (1–4): ");

        if (slot < 1 || slot > 4) {
            printf("Invalid slot number. Returning to main menu.\n");
            return 0;
        }

        if (!valid_slots[slot]) {
            printf("Selected slot is empty or corrupted. \n");
            return 0;
        }

        sprintf(filename, "src/save_slot%d%s", slot, ext);
        if (load_game(&currentGameState, filename)) {
            printf("\nGame loaded successfully!\n");
            printf("  Slot %d\n", slot);
            printf("  Location: %s\n\n", get_location_name(currentGameState.location));
            resume_from_loaded_state();
            return 1;
        } else {
            printf("Failed to load save file. Returning to main menu.\n");
            return 0;
        }
    }
}

void resume_from_loaded_state(void) {
    GameLocation loc = currentGameState.location;

    mainPlayer = currentGameState.player;
    display_stats(&mainPlayer); 

    switch (loc) {
        case STATE_MAIN_MENU:
            break;
        case STATE_VILLAGE:
            print_backstory();
        case STATE_OVERWORLD:
            overworld_exploration();
            break;
        case STATE_BANDIT_CAMP:
            bandit_camp_encounter();
            break;
        case STATE_DARK_FOREST:
            dark_forest_encounter();
            break;
        case STATE_OLD_SAGE_TOWER:
            old_sage_tower();
            break;
        case STATE_VOLCANO_DUNGEON:
            combat_volcano_dungeon(&mainPlayer);
            break;
        case STATE_ICE_PUZZLE:
            if (!mainPlayer.has_ice_crystal) {
                while (!solve_ice_puzzle()) {
                    print_pause("The inscription remains unreadable... your answer was not quite right. Try again.");
                }
                print_pause("The wall shimmers and reveals the hidden Ice Crystal!");
                mainPlayer.has_ice_crystal = 1;
                gain_exp(&mainPlayer, 50);
            }
            collect_crystals(&mainPlayer); 
            break;
        case STATE_HAUNTED_RUINS:
            haunted_ruins_encounter(&mainPlayer);
            break;
        case STATE_CRYSTALS:
            collect_crystals(&mainPlayer);
            break;
        case STATE_MAZE:
            traverse_maze(&mainPlayer);
            break;
        case STATE_MASTER_SWORD:
            if (!mainPlayer.has_rare_ore)
            {
                print_pause("With all 3 Crystals of Power, you head below the basement of the Old Sage Tower, towards the mines.");
                print_pause("You must find a Rare Ore to craft the Master Sword.");
                print_pause("You venture into the mines and face a maze of twisting corridors...");
                traverse_maze(&mainPlayer);
            }
        
            if (!mainPlayer.has_master_sword)
            {
                craft_master_sword(&mainPlayer); 
            }
        
            if (mainPlayer.has_master_sword)
            {
                approach_dark_lord_castle(&mainPlayer);
            }
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
        case STATE_VICTORY:
            ending_scene();
            printf("\nThank you for playing!\n\n");
            exit(0);
        default:
            printf("⚠ Unknown save location. Restarting from overworld...\n");
            overworld_exploration();
            break;
    }
}
