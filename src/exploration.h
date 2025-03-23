#ifndef EXPLORATION_H
#define EXPLORATION_H

void start_game();
void overworld_exploration();
int handle_fork_path(); // returns 1 for forest, 2 for camp
int choose_direction_for_area(int area);
int is_correct_direction(int area, int direction);
void bandit_camp_encounter();
void dark_forest_encounter();
void obtain_relics();
void old_sage_tower();

#endif // EXPLORATION_H