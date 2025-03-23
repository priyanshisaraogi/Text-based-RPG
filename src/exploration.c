#include "exploration.h"
#include <stdio.h>

int choose_direction_for_area(int area);
int is_correct_direction(int area, int direction);

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
        // ...Add Bandit Camp logic here (e.g., bandit map updates)...
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
