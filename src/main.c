#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "exploration.h"
#include "common.h"
#include "game_state.h"
#include "save.h"

GameState currentGameState;
Player mainPlayer;

void main_menu(void) {
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

    int choice = get_int_input("Enter choice: ");

    if (choice == 1) {
        print_backstory();
    } else if (choice == 2) {
        handle_load_game();
    } else if (choice == 3) {
        printf("Farewell, hero.\n");
        exit(0);
    } else {
        printf("Invalid choice. Please restart the game.\n");
        exit(1);
    }
}

void start_game(void) {
    currentGameState.location = STATE_MAIN_MENU;
    init_player(&mainPlayer);
    main_menu();
}

int main(void) {
    reset_player_state();
    start_game();
    print_pause("\nThank you for playing!\n");
    return 0;
}
