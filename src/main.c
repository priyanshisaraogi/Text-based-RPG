#include <stdio.h>
#include "exploration.h"
#include "player.h"
#include "common.h"
#include "quests.h"
#include "combat.h"

int main(void) {
    reset_player_state();
    start_game();
    
    print_pause("\nThank you for playing!\n");
    return 0;
}
