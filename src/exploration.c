#include "exploration.h"
#include "player.h"
#include <stdio.h>

// Global inventory flags
int gotMapFragment = 0;
int gotMysticHerb = 0;

// Declare a persistent player instance.
Player mainPlayer;

int choose_direction_for_area(int area);
int is_correct_direction(int area, int direction);
void bandit_camp_encounter();
void dark_forest_encounter();
void obtain_relics();
void old_sage_tower();

void start_game() {
    // Start game initialization and launch overworld exploration
    printf("Starting game...\n");
    overworld_exploration();
}

void overworld_exploration() {
    // The player goes through 5 areas in the overworld before reaching the forked path.
    printf("You are in the overworld. Your journey begins...\n");
    
    for (int area = 1; area <= 5; area++) {
        printf("\n--- Area %d ---\n", area);
        printf("Choose your path: 1 for Left, 2 for Right, 3 for Straight: ");
        int direction = choose_direction_for_area(area);
        
        if (!is_correct_direction(area, direction)) {
            printf("Wrong direction! You get lost and return to the village.\n");
            return; // back to village on wrong choice
        }
        printf("Correct path! You continue to the next area...\n");
    }
    
    // After 5 successful areas, present the forked path
    int path_choice = handle_fork_path();
    
    if (path_choice == 1) {
        // Logic for heading to the Dark Forest
        printf("You venture into the Dark Forest...\n");
        // ...Add Dark Forest logic here (e.g., beast fights, inventory updates)...
    } else if (path_choice == 2) {
        // Logic for heading to the Bandit Camp
        printf("You head towards the Bandit Camp...\n");
        bandit_camp_encounter();
    } 
}

int handle_fork_path() {
    // Q&A traversal logic: ask the player to choose a path after five areas
    int choice = 0;
    do {
        printf("\n--- Forked Path ---\n");
        printf("Choose your path: 1 for Dark Forest, 2 for Bandit Camp: ");
        scanf("%d", &choice);
        if (choice != 1 && choice != 2) {
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 1 && choice != 2);
    
    return choice;
}

// Reads the player's directional choice for an area.
int choose_direction_for_area(int area) {
    int choice = 0;
    scanf("%d", &choice);
    return choice;
}

// Determines whether the chosen direction is correct for the given area.
// Example logic:
//   Area 1 & 4: correct direction is Left (1)
//   Area 2 & 5: correct direction is Straight (3)
//   Area 3: correct direction is Right (2)
int is_correct_direction(int area, int direction) {
    int correct = 0;
    switch(area) {
        case 1:
        case 4:
            correct = 1; // Left
            break;
        case 2:
        case 5:
            correct = 3; // Straight
            break;
        case 3:
            correct = 2; // Right
            break;
        default:
            return 0;
    }
    return (direction == correct);
}

void bandit_camp_encounter() {
    int choice = 0;
    do {
        printf("\nAt the Bandit Camp your mission is to retrieve the map fragment. You have two options:\n");
        printf("1. Fight head-on with honour for glory\n");
        printf("2. Use stealth like a craven\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        if (choice != 1 && choice != 2) {
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 1 && choice != 2);
    
    if (choice == 2) {
        printf("You chose stealth: Successful stealth! You obtain the map fragment from the bandits.\n");
        gotMapFragment = 1;
        // Award 50 EXP for successful stealth.
        gain_exp(&mainPlayer, 50);
        // Check if both relics are acquired
        if (gotMapFragment && gotMysticHerb) {
            old_sage_tower();
        } else {
            printf("You haven't gathered all required relics yet. Returning to the forked path.\n");
            obtain_relics();
        }
    } else if (choice == 1) {
        printf("You chose to fight head-on, but it was a trap!\n");
        printf("Overwhelmed by the bandits, you are thrown into their own jail.\n");
        printf("You fortunately manage to escape, but you lose all quest items and all experience.\n");
        // Lose all EXP and quest items.
        mainPlayer.exp = 0;
        gotMapFragment = 0;
        gotMysticHerb = 0;
        start_game();
    }
}

void dark_forest_encounter() {
    int choice = 0;
    do {
        printf("\nAt the Dark Forest, a ferocious beast guards the mystic herb.\n");
        printf("Choose your approach:\n");
        printf("1. Fight the beast\n");
        printf("2. Attempt to flee\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        if (choice != 1 && choice != 2) {
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 1 && choice != 2);
    
    if (choice == 2) {
        printf("\nYou attempt to flee, but the beast chases you down!\n");
        printf("You have died while trying to escape...\n");
        printf("Respawning back to the village...\n");
        // Player loses all EXP and quest items when fleeing
        mainPlayer.exp = 0;
        gotMapFragment = 0;
        gotMysticHerb = 0;
        start_game();
        return;
    }
    
    // Use the persistent mainPlayer (do not reinitialize)
    int beastHealth = 60; // Beast's starting health
    int beastAttack = 15; // Beast's attack value
    int attackChoice = 0;
    
    printf("\nBattle begins!\n");
    display_stats(&mainPlayer);
    printf("Beast HP: %d\n", beastHealth);
    
    // Battle loop
    while (mainPlayer.health > 0 && beastHealth > 0) {
        // Player's turn
        printf("\nYour turn:\n");
        printf("Choose your attack:\n");
        printf("1. Powerful strike (deals %d damage)\n", mainPlayer.attack);
        printf("2. Normal strike (deals %d damage)\n", mainPlayer.attack / 2);
        printf("Enter your choice: ");
        scanf("%d", &attackChoice);
        
        if (attackChoice == 1) {
            printf("You use a powerful strike!\n");
            beastHealth -= mainPlayer.attack;
        } else {
            printf("You use a normal strike.\n");
            beastHealth -= mainPlayer.attack / 2;
        }
        
        if (beastHealth <= 0) {
            break;
        }
        
        // Beast's turn: calculate damage factoring in player's defense.
        int damage = beastAttack - mainPlayer.defense;
        if (damage < 0) {
            damage = 0;
        }
        printf("\nThe beast attacks and deals %d damage to you!\n", damage);
        mainPlayer.health -= damage;
        
        printf("\nCurrent status:\n");
        display_stats(&mainPlayer);
        printf("Beast HP: %d\n", beastHealth);
    }
    
    if (mainPlayer.health <= 0) {
        printf("\nYou have been slain by the beast.\n");
        printf("Respawning back to the village...\n");
        // Lose all EXP and quest items on death.
        mainPlayer.exp = 0;
        gotMapFragment = 0;
        gotMysticHerb = 0;
        start_game();
    } else {
        printf("\nYou have defeated the beast!\n");
        // Award EXP (which may level up the player)
        gain_exp(&mainPlayer, 50);
        printf("You obtain the mystic herb from the Dark Forest.\n");
        gotMysticHerb = 1;
        // Check if both relics are acquired.
        if (gotMapFragment && gotMysticHerb) {
            old_sage_tower();
        } else { 
            printf("You haven't gathered all required relics yet. Returning to the forked path.\n");
            obtain_relics();
        }
    }
}

// Obtain relics: loop until both the map fragment and mystic herb are acquired.
void obtain_relics() {
    while (!(gotMapFragment && gotMysticHerb)) {
        if (!gotMapFragment)
            printf(" - You need the map fragment from the Bandit Camp.\n");
        if (!gotMysticHerb)
            printf(" - You need the mystic herb from the Dark Forest.\n");
        
        int path_choice = handle_fork_path();
        
        if (path_choice == 1) {
            // Dark Forest branch
            if (!gotMysticHerb) {
                dark_forest_encounter();
            } else {
                printf("You already have the mystic herb. Returning to the forked path.\n");
            }
        } else if (path_choice == 2) {
            // Bandit Camp branch
            if (!gotMapFragment) {
                bandit_camp_encounter();
            } else {
                printf("You already have the map fragment. Returning to the forked path.\n");
            }
        }
    }
    // When both relics are acquired, proceed to the Old Sage Tower.
    old_sage_tower();
}

// Old Sage Tower encounter: player proceeds only if both relics have been acquired.
void old_sage_tower() {
    if (!(gotMapFragment && gotMysticHerb)) {
        printf("\nYou do not have all the required relics to enter the Old Sage Tower.\n");
        printf("Returning to the forked path...\n");
        obtain_relics();
        return;
    }
    
    printf("\nYou approach the Old Sage Tower.\n");
    printf("With both the map fragment and the mystic herb in hand, the ancient door opens for you.\n");
    printf("Your quest continues...\n");
}
