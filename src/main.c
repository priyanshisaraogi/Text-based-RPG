#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "exploration.h"
#include "common.h"
#include "game_state.h"
#include "save.h"

GameState currentGameState;
Player mainPlayer;

static int main_menu(void) {
    int choice;

    printf("\n================== MAIN MENU =========================\n\n");
    printf("       ╔════════════════════════════════╗       \n");
    printf("       ║        LEGACY OF YHARNAM       ║       \n");
    printf("       ║    SHADOWS OF THE FORGOTTEN    ║       \n");
    printf("       ╚════════════════════════════════╝       \n");
    printf("\n================== MAIN MENU =========================\n\n");
    printf("Welcome, chosen one.\n\n");
    printf("What would you like to do?\n");
    printf("1. Play\n");
    printf("2. Load Game\n");
    printf("3. Exit\n\n");

    printf("Controls:\n\n");
    printf("Press ENTER to continue dialogue.\n");
    printf("Press 'p' at any time to pause and access the save/exit menu.\n\n");

    choice = get_int_input("Enter choice: ");
    return choice;
}

static void start_game(void) {
    int choice;

    currentGameState.location = STATE_MAIN_MENU;
    init_player(&mainPlayer);

    while (1) {
        choice = main_menu();

        if (choice == 1) {
            print_backstory();
            break;
        } else if (choice == 2) {
            if (handle_load_game()) {
                break;
            } else {
                printf("Returning to main menu.\n\n");
            }
        } else if (choice == 3) {
            printf("Farewell, hero.\n");
            exit(0);
        } else {
            printf("Invalid choice.\n");
        }
    }
}

int main(void) {
    reset_player_state();
    start_game();
}
