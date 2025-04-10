#include "exploration.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>

/* Global flag for tower entry - relic flags now reside in mainPlayer */
int towerEntered = 0;

/* Declare a persistent player instance. */
Player mainPlayer;

void start_game(void) {
    /* Initialize mainPlayer with starting stats (and relic flags are set in init_player) */
    init_player(&mainPlayer);
    printf("Starting game...\n");
    print_backstory();
}

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

void print_backstory(void) {
    printf("\n--------------------------------------------------\n");
    print_pause("In the shadowed lands of Yharnam, a dark lord terrorizes the realm.");
    print_pause("Chosen by the gods, you are the beacon of hope destined to stop him.");
    print_pause("Leaving your humble village behind, you venture into the overworld, a confusing and perilous place where few have dared to tread.");
    print_pause("Your quest is to navigate these mysterious paths and collect two sacred relics:");
    print_pause("The map fragment,");
    print_pause("and the mystic herb.");
    print_pause("Only by possessing these can you unlock the ancient gates of the Old Sage Tower, your gateway to confronting the dark lord and bringing light back to Yharnam.");
    printf("Prepare well, for without these relics, you shall be ill prepared to face the dark lord.\n");
    printf("\n--------------------------------------------------\n");
    overworld_exploration();
}

/* Helper function: returns a valid integer from the user */
int get_int_input(const char *prompt) {
    int choice;
    char input[128];
    char *endptr;
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) != NULL) {
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

void overworld_exploration(void) {
    int area;
    printf("\nAs you leave your village, you enter the overworld. Your journey begins...\n");
    
    for (area = 1; area <= 5; area++) {
        int direction;
        printf("\n--- Area %d ---\n", area);
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
            printf("You venture into the Dark Forest...\n");
            dark_forest_encounter();
        } else if (path_choice == 2) {
            printf("You head towards the Bandit Camp...\n");
            bandit_camp_encounter();
        } 
    }
}

int handle_fork_path(void) {
    int choice = 0;               
    char input[128];
    char *endptr = NULL;          
    while (1) {
        printf("\n--- Forked Path ---\n");
        printf("Choose your path: 1 for Dark Forest, 2 for Bandit Camp: ");
        if (fgets(input, sizeof(input), stdin) != NULL) {
            choice = strtol(input, &endptr, 10);
            if (endptr == input || (*endptr != '\n' && *endptr != '\0')) {
                printf("Wrong input, please choose again.\n");
                continue;
            }
            if (choice != 1 && choice != 2) {
                printf("Wrong input, please choose again.\n");
                continue;
            }
            return choice;
        } else {
            printf("Input error, please try again.\n");
        }
    }
}

int choose_direction_for_area(int area) {
    int choice;
    while (1) {
        choice = get_int_input("\nChoose your path: 1 for Left, 2 for Right, 3 for Straight: ");
        if (choice != 1 && choice != 2 && choice != 3) {
            printf("Wrong input, please choose 1, 2, or 3.\n");
            continue;
        }
        return choice;
    }
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
    while (1) {
        printf("\nAt the Bandit Camp, your mission is to retrieve the map fragment.\n");
        printf("You have two options:\n");
        printf("1. Fight head-on with honour for glory\n");
        printf("2. Use stealth like a craven\n");
        choice = get_int_input("Enter your choice: ");
        if (choice == 1 || choice == 2) {
            break;
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }
    
    if (choice == 2) {
        print_pause("You chose stealth: Successful stealth! You obtain the map fragment from the bandits.");
        mainPlayer.has_map_fragment = 1;
        gain_exp(&mainPlayer, 50);
        printf("Current status:\n");
        display_stats(&mainPlayer);
        if (mainPlayer.has_map_fragment && mainPlayer.has_mystic_herb) {
            print_pause("You have gathered all required relics yet. Hence, you can now head towards the old sage tower.");
            old_sage_tower();
        } else {
            print_pause("However, you haven't gathered all required relics yet. Thus you return to the forked path.");
            obtain_relics();
        }
    } else if (choice == 1) {
        print_pause("You chose to fight head-on, but it was a trap!");
        print_pause("Overwhelmed by the bandits, you are thrown into their own jail.");
        print_pause("You fortunately manage to escape, but you lose all quest items and all experience.");
        print_pause("Sometimes there is a time to be brave, and a time to be smart. That was not the time to be brave. Sorry.");
        print_pause("Respawning back to the village...try to be smarter next time.");
        mainPlayer.exp = 0;
        mainPlayer.has_map_fragment = 0;
        mainPlayer.has_mystic_herb = 0;
        overworld_exploration();
    }
}

void dark_forest_encounter(void) {
    int choice = 0;
    while (1) {
        printf("\nAt the Dark Forest, a ferocious beast guards the mystic herb.\n");
        printf("Choose your approach:\n");
        printf("1. Fight the beast\n");
        printf("2. Attempt to flee\n");
        choice = get_int_input("\nEnter your choice: ");
        if (choice == 1 || choice == 2) {
            break;
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }
    
    if (choice == 2) {
        print_pause("You attempt to flee, but the beast chases you down and tear you into pieces before devouring you!");
        print_pause("You have died while trying to escape...you bloody coward");
        print_pause("Respawning back to the village...try to grow a spine next time.");
        mainPlayer.exp = 0;
        mainPlayer.has_map_fragment = 0;
        mainPlayer.has_mystic_herb = 0;
        overworld_exploration();
        return;
    }
    
    {
        int beastHealth = 60; /* Beast's starting health */
        int beastAttack = 15; /* Beast's attack value */
        int attackChoice = 0;
        int damage = 0;
        
        printf("\nBattle begins!\n");
        printf("Your HP: %d\n", mainPlayer.health);
        printf("Beast HP: %d\n", beastHealth);
    
        while (mainPlayer.health > 0 && beastHealth > 0) {
            printf("\nYour turn:\n");
            printf("Choose your attack:\n");
            printf("1. Powerful strike (deals %d damage)\n", mainPlayer.attack);
            printf("2. Normal strike (deals %d damage)\n", mainPlayer.attack / 2);
            attackChoice = get_int_input("Enter your choice: ");
            
            if (attackChoice == 1) {
                printf("You use a powerful strike!\n");
                beastHealth -= mainPlayer.attack;
            } else if (attackChoice == 2) {
                printf("You use a normal strike.\n");
                beastHealth -= mainPlayer.attack / 2;
            } else {
                printf("Invalid choice. Please try again.\n");
                continue;
            }
            
            if (beastHealth <= 0) {
                break;
            }
            
            damage = beastAttack - mainPlayer.defense;
            if (damage < 0) {
                damage = 0;
            }
            printf("\nThe beast attacks and deals %d damage to you!\n", damage);
            mainPlayer.health -= damage;
            printf("Your HP: %d\n", mainPlayer.health);
            printf("Beast HP: %d\n", beastHealth);
        }
        
        if (mainPlayer.health <= 0) {
            print_pause("You have been slain by the beast. Better luck next time");
            print_pause("Respawning back to the village...try to be more strategic in your attacks.");
            mainPlayer.exp = 0;
            mainPlayer.has_map_fragment = 0;
            mainPlayer.has_mystic_herb = 0;
            mainPlayer.health = 100; /* Reset health */
            overworld_exploration();
        } else {
            print_pause("You have defeated the beast!");
            gain_exp(&mainPlayer, 50);
            print_pause("You obtain the mystic herb from the Dark Forest.");
            mainPlayer.has_mystic_herb = 1;
            printf("Current status:\n");
            display_stats(&mainPlayer);
            if (mainPlayer.has_map_fragment && mainPlayer.has_mystic_herb) {
                print_pause("You have gathered all required relics yet. Hence, you can now head towards the old sage tower.");
                old_sage_tower();
            } else { 
                print_pause("However, you haven't gathered all required relics yet. Thus you return to the forked path.");
                obtain_relics();
            }
        }
    }
}

void obtain_relics(void) {
    while (!(mainPlayer.has_map_fragment && mainPlayer.has_mystic_herb)) {
        if (!mainPlayer.has_map_fragment)
            printf(" - You need the map fragment from the Bandit Camp.\n");
        if (!mainPlayer.has_mystic_herb)
            printf(" - You need the mystic herb from the Dark Forest.\n");
        
        {
            int path_choice = handle_fork_path();
            
            if (path_choice == 1) {
                if (!mainPlayer.has_mystic_herb) {
                    dark_forest_encounter();
                } else {
                    printf("You already have the mystic herb.\n");
                }
            } else if (path_choice == 2) {
                if (!mainPlayer.has_map_fragment) {
                    bandit_camp_encounter();
                } else {
                    printf("You already have the map fragment.\n");
                }
            }
        }
    }
    old_sage_tower();
}

void old_sage_tower(void) {
    if (towerEntered) {
        return;  /* Tower already entered */
    }
    towerEntered = 1;
    
    print_pause("You approach the Old Sage Tower.");
    print_pause("With both the map fragment and the mystic herb in hand, the ancient door opens for you.");
    print_pause("Your quest continues...");
}
