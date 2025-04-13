#include "combat.h"
#include "common.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>

#define BASE_HEALTH 100

/* Function declarations */
void final_boss_fight(Player *player);

/* Function to solve the Temple Gate puzzle */
void solve_guard_riddle(Player *player) {
    int choice;
    
    while (1) {
        print_pause("Upon approaching the entrance, you stand before two guards with two doors behind them: \"One always tells the truth and the other always lies!\" they announce.");
        print_pause("\"One door leads to safety and the other to certain doom!\"");
        print_pause("\"So ask your questions wisely!\" the guards say.");
        print_pause("So hero, what will you do?");
        printf("1. Ask the guard on the left a question.\n2. Take action\n");
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            printf("Invalid input. Please enter 1 or 2.\n");
            continue; 
        }
        while(getchar() != '\n');
        
        if (choice == 1) {
            print_pause("\nYou boldly ask the guard on the left, \"Have you slept with this guy's wife?\"");
            print_pause("The guard instictively blurts out, \"Yes I have.\" He suddenly realises his mistake as the guard on the right looks at him venomously.");
            print_pause("\"Well, would you look at that...\"The left guard says sheepishly.");
            print_pause("\"I forgive you!\" The right guard bellows as he takes out his sword and starts attacking the left guard.");
            print_pause("As much as you would wish to see who would win the fight, you have a dark lord to slay, so you shout to the guard on the left which door is safe, and he shouts back \"The right door!\" while desparetly defending himself.");
            print_pause(" With that, you take the right door and enter the castle.");
            break;
        } else if (choice == 2) {
            print_pause("Without hesitation, you snatch the Master Sword and lunge at the guard on the left. The sword finds its mark and the guard crumples to the ground.");
            print_pause("Once he drops to the ground dead, you look to the other guard and ask him, \"Did I just kill this man?\"");
            print_pause("The guard looks at you in silence before finally stating,\"......No......\"");
            print_pause("Satisfied, you ask him which door is safe, and he points to the left door, all the while looking at his deceased comrade in shock and dismay.");
            print_pause("With that, you head towards the door on the right and enter the castle.");
            break;
        } else {
            printf("Invalid input. Please enter 1 or 2.\n");
        }
    }
    gain_exp(player, 100);
    mini_boss_fight(player);
}

/* Function for the mini-boss fight */
void mini_boss_fight(Player *player)
{
    int bossHealth = 100;
    int bossAttack = 25;
    int attackChoice = 0;
    int damage = 0;
    
    print_pause("As you enter the castle, you hear a load roar echoing through the halls.");
    print_pause("You unsheathe the Master Sword and prepare for battle, trudging forward to face whatever monstrosity is waiting for you.");
    print_pause("You enter a giant hall, and a sight of a large Cerberus greets you. The doors behind you closes, leaving you with no way out.");
    print_pause("The beast growls and bellows out, \"Foolish mortal! Do you truly believe that you could slay my master?! I shall devour you and gnaw on your bones for your audacity!\"");
    print_pause("The Cerberus lunges at you, but you are ready...");
    print_pause("Mini Boss Cerberus Fight begins!");
    printf("Your HP: %d\n", player->health);
    printf("Mini Boss HP: %d\n", bossHealth);
    
    while (player->health > 0 && bossHealth > 0)
    {
        printf("\nYour turn:\n");
        printf("Choose your attack:\n");
        printf("1. Mighty blow (deals %d damage)\n", player->attack);
        printf("2. Swift jab (deals %d damage)\n", player->attack / 2);
        
        attackChoice = get_int_input("Enter your choice: ");
        
        if (attackChoice == 1)
        {
            printf("You deliver a might blow!\n");
            bossHealth -= player->attack;
        }
        else if (attackChoice == 2)
        {
            printf("You strike quickly with a swift jab!\n");
            bossHealth -= player->attack / 2;
        }
        else
        {
            printf("Invalid input. Please try again.\n");
            continue;
        }
        
        if (bossHealth <= 0)
        {
            break;
        }
        
        damage = bossAttack - player->defense;
        if (damage < 0)
        {
            damage = 0;
        }
        
        printf("\nThe Mini Boss attacks and deals %d damage to you!\n", damage);
        player->health -= damage;
        printf("Your HP: %d\n", player->health);
        printf("Mini Boss HP: %d\n", bossHealth);
    }
    
    if (player->health <= 0)
    {
        print_pause("You have been defeated by the Mini Boss. Respawning back to the entrance of the castle...");
        player->exp -=100;
        player->health = BASE_HEALTH + ((player->level - 1) * (BASE_HEALTH / 2));
        solve_guard_riddle(player);
    }
    else
    {
        print_pause("You have defeated the Cerberus!");
        gain_exp(player, 100);
        print_pause("As the Cerberus lay dying, the dooors behind it swings open, revealing a dark corridor.");
        print_pause("A voice beckons you, \"Congratulations! You have slain my pet. At the very least you are worthy to face me...if you dare!\"");
        final_dungeon(player);
    }
}

/* Function to enter the final dungeon */
void final_dungeon(Player *player) {
    char ready;
    
    print_pause("You trudge forward into the dark corridor, the air thick with tension.");
    print_pause("The walls are adorned with ancient runes, and the atmosphere is heavy with foreboding.");
    print_pause("You can feel the presence of the Dark Lord lurking in the shadows, waiting for you to make your move.");
    print_pause("As you approach the end of the corridor, you see a massive door, engraved with ominous symbols.");
    print_pause("You know that beyond this door lies the final confrontation with the Dark Lord.");
    
    while (1) {
        printf("Are you ready to face the dark lord? (y/n): ");
        
        if (scanf(" %c", &ready) != 1) {
            printf("Invalid input. Please enter only 'y' or 'n'.\n");
            continue;
        }
        
        /* Check if input is valid */
        if (ready == 'y' || ready == 'Y' || ready == 'n' || ready == 'N') {
            break;
        } else {
            printf("Invalid input. Please enter only 'y' or 'n'.\n");
        }
    }
    
    if (ready == 'y' || ready == 'Y') {
        print_pause("With courage in your heart, you take a deep breath and step forward, pushing open the massive gates.");
    } else {  /* (ready is 'n' or 'N') */
        print_pause("Too bad! The doors behind you are locked shut. There's no turning back now, so suck it up and face the Dark Lord!");
        print_pause("Upon hearing this narrator, you break in a cold sweat. You have no choice but to face the Dark Lord.");
        print_pause("You take a deep breath and step forward, pushing open the massive gates.");
    }
    
    final_boss_fight(player);
}

/* Function to fight the final boss */
void final_boss_fight(Player *player)
{
    int bossHealth = 150;  
    int bossAttack = 25;    
    int attackChoice = 0;
    int damage = 0;
    
    print_pause("--- Final Boss Fight: The Dark Lord ---");
    print_pause("You stand before the Dark Lord in his throne room—the ultimate test of your valor and skill!");
    print_pause("The air crackles with dark energy as he glares at you, his eyes burning with malice.");
    print_pause("He raises his sword, and the ground trembles beneath your feet.");
    print_pause("The Dark Lord bellows, \"At last we meet face to face little hero! Lets us see what you are made of!\"");
    print_pause("With that, the final battle begins!");
    printf("Your HP: %d\n", player->health);
    printf("Final Boss HP: %d\n", bossHealth);
    
    while (player->health > 0 && bossHealth > 0)
    {
        printf("\nYour turn:\n");
        printf("Choose your attack:\n");
        printf("1. Mighty strike (deals %d damage)\n", player->attack);
        printf("2. Swift jab (deals %d damage)\n", player->attack / 2);
        
        attackChoice = get_int_input("Enter your choice: ");
        
        if (attackChoice == 1)
        {
            printf("You deliver a mighty strike!\n");
            bossHealth -= player->attack;
        }
        else if (attackChoice == 2)
        {
            printf("You launch a swift jab!\n");
            bossHealth -= player->attack / 2;
        }
        else
        {
            printf("Invalid choice. You miss your attack!\n");
            continue;
        }
        
        if (bossHealth <= 0)
            break;
        
        /* Boss counterattack */
        damage = bossAttack - player->defense;
        if (damage < 0)
            damage = 0;
        printf("\nThe Dark Lord retaliates, dealing %d damage to you!\n", damage);
        player->health -= damage;
        
        printf("Your HP: %d\n", player->health);
        printf("Final Boss HP: %d\n", bossHealth);
    }
    
    if (player->health <= 0)
    {
        print_pause("You have been defeated by the Dark Lord. You have failed! Darkness has consumed the kingdom...");
        print_pause("As you slowly close your eyes, the last sight you see is the dark lord simply trudginng back to his throne, sitting upon it and remarking, \"What a weakling...maybe the next hero would give me a better fight...\".");
        print_pause("Respawning back to the entrance of the castle...");
        player->level -= 1;
        player->exp = 100;
        player->health = BASE_HEALTH + ((player->level - 1) * (BASE_HEALTH / 2));
        solve_guard_riddle(player);
    }
    else
    {
        print_pause("With one final, decisive blow, you stabbed through the Dark Lord!");
        print_pause("The Dark Lord lets out a blood-curdling scream, he croaks out, \"Impossible...a mortal has defeated me?\".");
        print_pause("As he falls to the ground, the dark energy surrounding him dissipates, and the castle begins to crumble.");
        print_pause("You stand victorious, but the battle has taken its toll.");
        print_pause("You look around, and the castle begins to collapse around you.");
        print_pause("You sprint towards the exit, dodging falling debris and flames.");
        print_pause("As you escape the castle, you feel a surge of energy coursing through you.");
        print_pause("The dark magic that once plagued the kingdom is lifted, and the skies clear.");
        print_pause("You have saved Yharnam from the clutches of darkness!");
        print_pause("Light returns to the kingdom, and years later, your legend is forever engraved in the annals of history.");
    }
}
