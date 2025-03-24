#include "exploration.h"
#include "player.h"
#include <stdio.h>

/* Global inventory flags */
int gotMapFragment, gotMysticHerb;
int towerEntered = 0;

/* Declare a persistent player instance. */
Player mainPlayer;

/* Function prototypes */
void start_game(void);
void overworld_exploration(void);
int handle_fork_path(void);
int choose_direction_for_area(int area);
int is_correct_direction(int area, int direction);
void bandit_camp_encounter(void);
void dark_forest_encounter(void);
void obtain_relics(void);
void old_sage_tower(void);

int main(void) {
    /* Start the game/quest. */
    start_game();
    return 0;
}

void start_game(void) {
    /* Initialize mainPlayer with starting stats */
    init_player(&mainPlayer);
    /* Reset quest-related flags if needed */
    gotMapFragment = 0;
    gotMysticHerb = 0;
    printf("Starting game...\n");
    overworld_exploration();
}

void overworld_exploration(void) {
    int area;
    printf("You are in the overworld. Your journey begins...\n");
    
    for (area = 1; area <= 5; area++) {
        int direction;
        printf("\n--- Area %d ---\n", area);
        printf("Choose your path: 1 for Left, 2 for Right, 3 for Straight: ");
        direction = choose_direction_for_area(area);
        
        if (!is_correct_direction(area, direction)) {
            printf("Wrong direction! You get lost and return to village.\n");
            area = 0;
            continue;
        }
        printf("Correct path! You continue to the next area...\n");
    }
    
    /* After 5 successful areas, present the forked path */
    {
        int path_choice;
        path_choice = handle_fork_path();
    
        if (path_choice == 1) {
            /* Logic for heading to the Dark Forest */
            printf("You venture into the Dark Forest...\n");
            /* ...Add Dark Forest logic here... */
            dark_forest_encounter();
        } else if (path_choice == 2) {
            /* Logic for heading to the Bandit Camp */
            printf("You head towards the Bandit Camp...\n");
            bandit_camp_encounter();
        } 
    }
}

int handle_fork_path(void) {
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

int choose_direction_for_area(int area) {
    int choice = 0;
    scanf("%d", &choice);
    return choice;
}

int is_correct_direction(int area, int direction) {
    int correct = 0;
    switch (area) {
        case 1:
        case 4:
            correct = 1; /* Left */
            break;
        case 2:
        case 5:
            correct = 3; /* Straight */
            break;
        case 3:
            correct = 2; /* Right */
            break;
        default:
            return 0;
    }
    return (direction == correct);
}

void bandit_camp_encounter(void) {
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
        /* Award 50 EXP for successful stealth. */
        gain_exp(&mainPlayer, 50);
        /* Check if both relics are acquired */
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
        /* Lose all EXP and quest items. */
        mainPlayer.exp = 0;
        gotMapFragment = 0;
        gotMysticHerb = 0;
        start_game();
    }
}

void dark_forest_encounter(void) {
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
        /* Player loses all EXP and quest items when fleeing */
        mainPlayer.exp = 0;
        gotMapFragment = 0;
        gotMysticHerb = 0;
        start_game();
        return;
    }
    
    /* Variables for battle must be declared at the top of the block. */
    {
        int beastHealth = 60; /* Beast's starting health */
        int beastAttack = 15; /* Beast's attack value */
        int attackChoice = 0;
        int damage = 0;
        
        printf("\nBattle begins!\n");
        display_stats(&mainPlayer);
        printf("Beast HP: %d\n", beastHealth);
    
        while (mainPlayer.health > 0 && beastHealth > 0) {
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
            
            /* Beast's turn: calculate damage factoring in player's defense. */
            damage = beastAttack - mainPlayer.defense;
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
            /* Lose all EXP and quest items on death. */
            mainPlayer.exp = 0;
            gotMapFragment = 0;
            gotMysticHerb = 0;
            start_game();
        } else {
            printf("\nYou have defeated the beast!\n");
            /* Award EXP (which may level up the player) */
            gain_exp(&mainPlayer, 50);
            printf("You obtain the mystic herb from the Dark Forest.\n");
            gotMysticHerb = 1;
            /* Check if both relics are acquired. */
            if (gotMapFragment && gotMysticHerb) {
                old_sage_tower();
            } else { 
                printf("You haven't gathered all required relics yet. Returning to the forked path.\n");
                obtain_relics();
            }
        }
    }
}

void obtain_relics(void) {
    while (!(gotMapFragment && gotMysticHerb)) {
        if (!gotMapFragment)
            printf(" - You need the map fragment from the Bandit Camp.\n");
        if (!gotMysticHerb)
            printf(" - You need the mystic herb from the Dark Forest.\n");
        
        {
            int path_choice;
            path_choice = handle_fork_path();
            
            if (path_choice == 1) {
                /* Dark Forest branch */
                if (!gotMysticHerb) {
                    dark_forest_encounter();
                } else {
                    printf("You already have the mystic herb. Returning to the forked path.\n");
                }
            } else if (path_choice == 2) {
                /* Bandit Camp branch */
                if (!gotMapFragment) {
                    bandit_camp_encounter();
                } else {
                    printf("You already have the map fragment. Returning to the forked path.\n");
                }
            }
        }
    }
    /* When both relics are acquired, proceed to the Old Sage Tower. */
    old_sage_tower();
}

void old_sage_tower(void) {
    if (towerEntered) {
        return;  /* Tower already entered; do nothing */
    }
    towerEntered = 1;
    
    printf("\nYou approach the Old Sage Tower.\n");
    printf("With both the map fragment and the mystic herb in hand, the ancient door opens for you.\n");
    printf("Your quest continues...\n");
}
