#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "quests.h"
#include "common.h"
#include "combat.h"
#include "game_state.h" 

extern GameState currentGameState;

int strcasecmp(const char *s1, const char *s2);

void enter_old_sage_tower(Player *player)
{
    int solved = 0;
    currentGameState.location = STATE_OLD_SAGE_TOWER;
    
    
    if (!player->has_mystic_herb || !player->has_map_fragment)
    {
        printf("You cannot enter without the Mystic Herb and Map Fragment.\n");
        return;
    }

    print_pause("In the old sage tower, your mission is to find the 3 Crystals of Power:");
    print_pause(" - The Fire Crystal,");
    print_pause(" - The Ice Crystal,");
    print_pause(" - and the Shadow Crystal.");
    print_pause("With the three crytals collected, you must find a way to forge them into the Master Sword...but don't worry, that comes later.");
    print_pause("First thing first, you need to find clues as to where the crystals are hidden.");
    print_pause("As you start to rummage around, you find a puzzle made of ancient riddles that seems to point towards the location of the crystals. You study it carefully...");

    /* Loop until the puzzle is solved correctly. */
    while (!solved)
    {
        solved = solve_puzzle();
        if (!solved)
        {
            print_pause("The puzzle confounds you. The clues are shown again. Try again dummy.");
        }
    }
    
    print_pause("Puzzle solved! You learn the location of the crystals.");
    gain_exp(player, 50);
    collect_crystals(player);
}

int solve_puzzle(void)
{
    int i;
    int c1, c2, c3;
    const char *clues[5] = {
        "1. In the chamber where flames burn without respite, the power of heat endures.",
        "2. In the hall of brilliant light, no shadow dares to linger.",
        "3. In the frozen vestibule, where silence is the language, cold whispers abound.",
        "4. In the depths of the earthen vaults, false promises lie buried.",
        "5. In the corridor of perpetual dusk, the veil of darkness conceals ancient secrets."
    };
    

    printf("\n--- Sage's Puzzle ---\n");
    for (i = 0; i < 5; ++i)
    {
        printf("%s\n", clues[i]);
    }

    printf("\nChoose 3 clues you believe are true (enter numbers separated by spaces): ");
    scanf("%d %d %d", &c1, &c2, &c3);

    if ((c1 == 1 || c2 == 1 || c3 == 1) &&
        (c1 == 3 || c2 == 3 || c3 == 3) &&
        (c1 == 5 || c2 == 5 || c3 == 5))
    {
        return 1;
    }

    return 0;
}

void combat_volcano_dungeon(Player *player)
{
    int beastHealth = 75;
    int beastAttack = 20;
    int attackChoice = 0;
    int damage = 0;
    currentGameState.location = STATE_VOLCANO_DUNGEON;
    print_pause("You enter the volcano dungeon...");
    print_pause("...and encounter what looks like a beast made out of lava! The door closes behind you. It's time to stand your ground!");
    print_pause("Battle begins!");
    printf("Your HP: %d\n", player->health);
    printf("Lava Beast HP: %d\n", beastHealth);
    
    while (player->health > 0 && beastHealth > 0)
    {
        printf("\nYour turn:\n");
        printf("Choose your attack:\n");
        printf("1. Mighty blow (deals %d damage)\n", player->attack);
        printf("2. Swift jab (deals %d damage)\n", player->attack / 2);
        attackChoice = get_int_input("Enter your choice: ");
        
        if (attackChoice == 1)
        {
            printf("You deliver a mighty blow!\n");
            beastHealth -= player->attack;
        }
        else if (attackChoice == 2)
        {
            printf("You strike quickly with a swift jab.\n");
            beastHealth -= player->attack / 2;
        }
        else
        {
            printf("Invalid choice. Please try again.\n");
            continue;
        }
        
        if (beastHealth <= 0)
        {
            break;
        }
        
        damage = beastAttack - player->defense;
        if (damage < 0)
        {
            damage = 0;
        }
        printf("\nThe Lava Beast attacks and deals %d damage to you!\n", damage);
        player->health -= damage;
        printf("Your HP: %d\n", player->health);
        printf("Lava Beast HP: %d\n", beastHealth);
    }
    
    if (player->health <= 0)
    {
        print_pause("You have been slain by the Lava Beast. Better luck next time.");
        print_pause("Respawning back to the beginning of the old sage tower... try to be more strategic in your attacks.");
        /* Reset necessary player parameters */
        player->exp = 0;
        player->health = BASE_HEALTH + ((player->level - 1) * (BASE_HEALTH / 2));
        enter_old_sage_tower(player);
    }
    else
    {
        print_pause("You have defeated the Lava Beast!");
        gain_exp(player, 50);
        print_pause("You have obtained the Fire Crystal from the Volcano Dungeon.");
        player->has_fire_crystal = 1;
    }
    collect_crystals(player);
}

int solve_ice_puzzle(void)
{
    char answer[64];
    size_t len;

    currentGameState.location = STATE_ICE_PUZZLE;

    print_pause("You come upon an ancient icy inscription carved into the frozen wall...");
    printf("\nIt reads:\n");
    printf("\"Born of winter's breath, I am the silent guard of the mountain's heart.\n");
    printf("Formed in stillness and cold, I glitter like a shard of forgotten art.\n");
    printf("What am I?\"\n\n");

    while (1) {
        printf("Enter your answer: ");
        if (fgets(answer, sizeof(answer), stdin) != NULL)
        {
            len = strlen(answer);
            if (len > 0 && answer[len - 1] == '\n')
                answer[len - 1] = '\0';  

            if (strlen(answer) == 1 && (answer[0] == 'p' || answer[0] == 'P')) {
                handle_pause_menu();
                continue;
            }

            if (strcasecmp(answer, "ice") == 0 || strcasecmp(answer, "ice crystal") == 0)
                return 1;
        }
        printf("That doesn't seem right. Try again.\n");
    }

    return 0; 
}

void haunted_ruins_encounter(Player *player)
{
    char choice[16];
    currentGameState.location = STATE_HAUNTED_RUINS;
    

    print_pause("You enter the haunted ruins...");

    while (1)
    {
        printf("...and encounter a soul trapped in torment. It begs you to free it in exchange for the Shadow Crystal. Do you free it? (y/n): ");
        if (fgets(choice, sizeof(choice), stdin) == NULL) continue;

        if (choice[0] == 'p' || choice[0] == 'P') {
            handle_pause_menu();
            continue;
        }

        if (choice[0] == 'y' || choice[0] == 'Y')
        {
            print_pause("You free the soul. It gifts you the Shadow Crystal in gratitude and starts to ascend to heaven...");
            print_pause("...but all of a sudden a red light shines from the ground and the soul is pulled back down! Oops!");
            break;
        }
        else if (choice[0] == 'n' || choice[0] == 'N')
        {
            print_pause("You destroy the soul. The Shadow Crystal materialises and you collect it...");
            print_pause("...but because of you, the soul is doomed to oblivion, never to enter any afterlife.");
            print_pause("Congratulations...you are a monster!");
            break;
        }
        else
        {
            printf("Invalid input. Please enter y or n.\n");
        }
    }

    player->has_shadow_crystal = 1;
    gain_exp(player, 50);

    collect_crystals(player);  
}



void collect_crystals(Player *player)
{
    int room_choice = 0;
    currentGameState.location = STATE_CRYSTALS;

    

    while (!(player->has_fire_crystal && player->has_ice_crystal && player->has_shadow_crystal))
    {
        int first = 1;
        print_pause("You explore the old sage tower with the clues in mind.");
        

        printf("\nChoose a room to enter: (");
        if (!player->has_fire_crystal)
        {
            printf("1 = Volcano Dungeon");
            first = 0;
        }
        if (!player->has_ice_crystal)
        {
            if (!first) printf(", ");
            printf("2 = Frozen Mountain");
            first = 0;
        }
        if (!player->has_shadow_crystal)
        {
            if (!first) printf(", ");
            printf("3 = Haunted Ruins");
        }
        printf("): ");

        room_choice = get_int_input("");

        if (room_choice < 1 || room_choice > 3)
        {
            printf("Invalid input. Please enter 1, 2, or 3.\n");
            continue;
        }

        if ((room_choice == 1 && player->has_fire_crystal) ||
            (room_choice == 2 && player->has_ice_crystal) ||
            (room_choice == 3 && player->has_shadow_crystal))
        {
            printf("\nYou have already collected that crystal. Please choose a different room.\n");
            continue;
        }

        switch (room_choice)
        {
            case 1:
                combat_volcano_dungeon(player);
                break;
            case 2:
                print_pause("You enter the icy dungeon...");
                while (!solve_ice_puzzle())
                {
                    print_pause("The inscription remains unreadable... your answer was not quite right. Try again.");
                }
                print_pause("The wall shimmers and reveals the hidden Ice Crystal!");
                player->has_ice_crystal = 1;
                gain_exp(player, 50);
                break;
            case 3:
                haunted_ruins_encounter(player);
                break;
        }
    }

    craft_master_sword(player);
}

void traverse_maze(Player *player)
{
    int correct[3] = {1, 2, 1};
    int step, choice;
    currentGameState.location = STATE_MAZE;

    while (1) {
        for (step = 0; step < 3; step++) {
            while (1) {
                printf("Step %d: Choose your path (1 for Left, or 2 for Right): ", step + 1);
                choice = get_int_input("");
                
                if (choice != 1 && choice != 2) {
                    printf("Invalid input. Please enter 1 or 2.\n");
                    continue;
                }

                if (choice != correct[step]) {
                    print_pause("Wrong choice! The passages suddenly turn dark, and so you hurriedly head back to the start of the maze.");
                    goto restart_maze;
                } else {
                    if (step == 0) {
                        print_pause("You turn left and discover a narrow passage where the walls glisten faintly.");
                    } else if (step == 1) {
                        print_pause("You choose the right path and notice ancient symbols carved along the corridor.");
                    } else if (step == 2) {
                        print_pause("Finally, you turn left again and the passage opens into a secret chamber.");
                    }
                    printf("Correct choice!\n");
                    break;
                }
            }
        }
        break;

    restart_maze:
        print_pause("Restarting the maze from the beginning...");
    }

    print_pause("After a tense journey through twisting corridors, you finally uncover a hidden chamber glowing with Rare Ore!");
    player->has_rare_ore = 1;  
    craft_master_sword(player);
}


void craft_master_sword(Player *player)
{
    currentGameState.location = STATE_MASTER_SWORD;
    if (!player->has_rare_ore) {
        print_pause("With all 3 Crystals of Power, you head below the basement of the Old Sage Tower, towards the mines.");
        print_pause("You must find a Rare Ore to craft the Master Sword.");
        print_pause("You venture into the mines and face a maze of twisting corridors...");
        traverse_maze(player); 
    }

    currentGameState.location = STATE_MASTER_SWORD;

    if (player->has_rare_ore && !player->has_master_sword) {
        gain_exp(player, 100);
        print_pause("After finding the Rare Ore, you leave the mines and travel back to the ground floor, where a forge awaits you.");
        print_pause("You place the 3 Crystals of Power and the Rare Ore into the forge. The forge roars to life, and the air crackles with energy.");
        print_pause("With your skills as a blacksmith's apprentice, you begin crafting the Master Sword.");
        print_pause("Crafting Master Sword...");
        player->has_master_sword = 1;
        print_pause("*** Master Sword crafted and added to inventory! ***");
    }

    if (player->has_master_sword) {
        approach_dark_lord_castle(player);
    }
}



void approach_dark_lord_castle(Player *player)
{
    print_pause("Clutching the legendary Master Sword, you leave the Old Sage Tower behind.");
    print_pause("You traverse treacherous lands and murky forests, the weight of destiny on your shoulders.");
    print_pause("At long last, you approach the foreboding entrance of the Dark Lord's Castle, ready to defeat him once and for all.");
    solve_guard_riddle(player);
}
