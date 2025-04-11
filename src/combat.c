#include "combat.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>

#define SAFE_DOOR 2  /* In our riddle, door number 2 is the safe door */

/* Function declarations */
void final_boss_fight(Player *player);

/* Function to solve the Temple Gate puzzle */
int solve_guard_riddle(void) {
    int choice;
    
    /* Present the Temple Gate puzzle instructions */
    printf("\n--- Temple Gate Puzzle ---\n");
    printf("You stand before two guards - one always tells the truth and the other always lies.\n");
    printf("One door leads to safety and the other to certain doom.\n");
    printf("Ask one guard: 'If I asked the other which door is safe, what would he say?'\n");
    printf("Then choose the opposite door.\n");
    printf("Which door do you choose? (Enter 1 for the left door, 2 for the right door): ");
    
    if (scanf("%d", &choice) != 1) {
        /* Clear invalid input */
        while (getchar() != '\n');
        printf("Invalid input. Defaulting to door 1.\n");
        choice = 1;
    }
    
    if (choice == SAFE_DOOR) {
        printf("You have chosen wisely!\n");
        return 1;
    } else {
        printf("Wrong choice! The guardians remain unconvinced. You must try again.\n");
        return 0;
    }
}

/* Function for the mini-boss fight */
void mini_boss_fight(Player *player) {
    int bossHealth = 100;
    int bossAttack = 20;
    int attackChoice;
    int damage;
    
    printf("\n--- Mini Boss Fight ---\n");
    printf("A fearsome mini boss emerges, ready to test your mettle!\n");
    
    while (player->health > 0 && bossHealth > 0) {
        /* Display player and boss stats */
        printf("\nYour Stats:");
        printf("\n  Health:  %d", player->health);
        printf("\n  Attack:  %d", player->attack);
        printf("\n  Defense: %d\n", player->defense);
        printf("Mini Boss Health: %d\n", bossHealth);

        printf("\nChoose your attack:\n");
        printf("1. Powerful strike (deals %d damage)\n", player->attack);
        printf("2. Quick strike (deals %d damage)\n", player->attack / 2);
        printf("Enter your choice: ");
        
        if (scanf("%d", &attackChoice) != 1) {
            /* Clear input buffer on invalid input */
            while (getchar() != '\n');
            printf("Invalid input. You lose your chance to attack!\n");
        } else {
            if (attackChoice == 1) {
                printf("You deliver a powerful strike!\n");
                bossHealth -= player->attack;
            } else if (attackChoice == 2) {
                printf("You execute a quick strike.\n");
                bossHealth -= player->attack / 2;
            } else {
                printf("Invalid choice! You miss your attack.\n");
            }
        }
        
        if (bossHealth <= 0)
            break;
            
        /* Boss counterattacks */
        damage = bossAttack - player->defense;
        if (damage < 0)
            damage = 0;
        printf("The mini boss attacks and deals %d damage to you!\n", damage);
        player->health -= damage;
    }
    
    /* Evaluate battle outcome */
    if (player->health <= 0) {
        printf("\nYou have fallen in battle against the mini boss.\n");
        printf("Returning to the village to recover...\n");
        /* Reset any necessary player stats here (e.g., experience loss) */
        player->exp = 0;
    } else {
        printf("\nYou have defeated the mini boss!\n");
        /* Award EXP for victory */
        int expGain = 100;
        player->exp += expGain;
        printf("You gained %d EXP! Total EXP is now %d.\n", expGain, player->exp);
    }
}

/* Function to enter the final dungeon */
void final_dungeon(Player *player) {
    char ready;
    
    printf("\n--- Final Dungeon ---\n");
    printf("You have reached the Final Dungeon.\n");
    printf("Are you ready to face the final challenge? (y/n): ");
    
    /* Clear any lingering input characters */
    while (getchar() != '\n');
    scanf("%c", &ready);
    
    if (ready == 'y' || ready == 'Y') {
        /* Optionally, check for potions/inventory requirements here */
        printf("You brace yourself and venture deeper into the darkness...\n");
        final_boss_fight(player);
    } else {
        printf("You decide to retreat and prepare further. Return when you are ready.\n");
    }
}

/* Function to fight the final boss */
void final_boss_fight(Player *player) {
    int bossHealth = 150;
    int bossAttack = 25;
    int attackChoice;
    int damage;
    
    printf("\n--- Final Boss Fight ---\n");
    printf("The final boss appears! This is the ultimate challenge.\n");
    
    while (player->health > 0 && bossHealth > 0) {
        /* Display current stats for player and final boss */
        printf("\nYour Stats:");
        printf("\n  Health:  %d", player->health);
        printf("\n  Attack:  %d", player->attack);
        printf("\n  Defense: %d\n", player->defense);
        printf("Final Boss Health: %d\n", bossHealth);
        
        /* Prompt player to choose an attack */
        printf("\nChoose your attack:\n");
        printf("1. Mighty blow (deals %d damage)\n", player->attack);
        printf("2. Swift jab (deals %d damage)\n", player->attack / 2);
        printf("Enter your choice: ");
        
        if (scanf("%d", &attackChoice) != 1) {
            while(getchar() != '\n');
            printf("Invalid input. You forfeit your turn!\n");
        } else {
            if (attackChoice == 1) {
                printf("You deliver a mighty blow!\n");
                bossHealth -= player->attack;
            } else if (attackChoice == 2) {
                printf("You strike quickly with a swift jab.\n");
                bossHealth -= player->attack / 2;
            } else {
                printf("Invalid choice! You miss your attack.\n");
            }
        }
        
        if (bossHealth <= 0)
            break;
            
        /* Final boss counterattack */
        damage = bossAttack - player->defense;
        if (damage < 0)
            damage = 0;
        printf("The final boss retaliates, dealing %d damage to you!\n", damage);
        player->health -= damage;
    }
    
    if (player->health <= 0) {
        printf("\nYou have been defeated by the final boss.\n");
        printf("The princess remains captive... darkness prevails.\n");
        
    } else {
        printf("\nWith one final strike, you defeat the final boss!\n");
        printf("The princess is saved and light returns to the kingdom.\n");
    }
}
