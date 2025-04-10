#include <stdio.h>
#include "exploration.h"
#include "player.h"
#include "combat.h"


extern Player mainPlayer;

int main(void) {
    /* Start the game/quest. */
    start_game();

    display_stats(&mainPlayer);
     
    printf("start combat section\n\n");
    mainPlayer.health = 300; /* for testing only*/

     
    /* Solve the Temple Gate riddle until the correct door is chosen */
    int solved = 0;
    while (!solved) {
        solved = solve_guard_riddle();
    }
     
    /* Initiate the mini boss fight */
    mini_boss_fight(&mainPlayer);
     
    /* Proceed to the final dungeon after the mini boss fight */
    final_dungeon(&mainPlayer);
     
    return 0;
}
