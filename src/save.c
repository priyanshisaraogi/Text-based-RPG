#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "save.h"
#include "game_state.h"
#include "common.h"
#include "exploration.h"
#include "quests.h"
#include "combat.h"

GameState currentGameState;
Player mainPlayer;

#define SAVE_MAGIC "RPG_SAVE"
#define SAVE_VERSION 1

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
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: Could not open file %s for writing.\n", filename);
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
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        printf("Error: Could not open file %s for writing.\n", filename);
        return 0;
    }

    int version = SAVE_VERSION; 
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

    unsigned short checksum = calculate_checksum(state);
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
    if (!fp) {
        printf("Error: Could not open file %s for reading.\n", filename);
        return 0;
    }

    ParseState parse_state = PARSE_HEADER;
    char line[128];
    int version = 0;
    unsigned short expected_checksum = 0;
    int fields_parsed = 0;
    const int expected_fields = 13; 

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0'; 
        if (strlen(line) == 0) continue; 

        char *key = strtok(line, "=");
        char *value = strtok(NULL, "=");
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
                    if (state->location < STATE_MAIN_MENU || state->location > STATE_FINAL_BOSS) {
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

    unsigned short actual_checksum = calculate_checksum(state);
    if (actual_checksum != expected_checksum) {
        printf("Error: Checksum mismatch. Save file may be corrupted.\n");
        return 0;
    }

    return 1;
}

int load_game_binary(GameState *state, const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("Error: Could not open file %s for reading.\n", filename);
        return 0;
    }

    char magic[sizeof(SAVE_MAGIC)];
    fread(magic, sizeof(char), strlen(SAVE_MAGIC) + 1, fp);
    if (strcmp(magic, SAVE_MAGIC) != 0) {
        printf("Error: Invalid save file magic string.\n");
        fclose(fp);
        return 0;
    }

    int version;
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

    if (state->location < STATE_MAIN_MENU || state->location > STATE_FINAL_BOSS ||
        state->player.health < 0 || state->player.level < 1) {
        printf("Error: Invalid data in binary save file.\n");
        fclose(fp);
        return 0;
    }

    unsigned short expected_checksum;
    fread(&expected_checksum, sizeof(unsigned short), 1, fp);
    unsigned short actual_checksum = calculate_checksum(state);
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

void resume_from_loaded_state(void) {
    GameLocation loc = currentGameState.location;

    mainPlayer = currentGameState.player;
    display_stats(&mainPlayer); 

    switch (loc) {
        case STATE_MAIN_MENU:
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
    int format_choice;
    int valid_slots[5] = {0};
    int found_any = 0;

    printf("\n--- Load Game ---\n\n");
    printf("Select file format to load:\n");
    printf("1. Text (.txt)\n");
    printf("2. Binary (.dat)\n");
    format_choice = get_int_input("Enter choice (1-2): ");
    if (format_choice < 1 || format_choice > 2) {
        printf("Invalid choice. Load cancelled.\n");
        return;
    }

    const char *ext = (format_choice == 1) ? ".txt" : ".dat";

    printf("\n--- Saved Slots (%s) ---\n\n", ext);

    for (int i = 1; i <= 4; i++) {
        sprintf(filename, "save_slot%d%s", i, ext);
        FILE *fp = fopen(filename, format_choice == 1 ? "r" : "rb");
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

    sprintf(filename, "save_slot%d%s", slot, ext);
    if (load_game(&currentGameState, filename)) {
        printf("\nGame loaded successfully!\n");
        printf("  Slot %d\n", slot);
        printf("  Location: %s\n\n", get_location_name(currentGameState.location));

        resume_from_loaded_state(); 
    } else {
        printf("Failed to load save file.\n");
    }
}
