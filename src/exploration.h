#ifndef EXPLORATION_H
#define EXPLORATION_H

/* Function prototypes */
void print_backstory(void);
void overworld_exploration(void);
int handle_fork_path(void);
int choose_direction_for_area(int area);
int is_correct_direction(int area, int direction);
void bandit_camp_encounter(void);
void dark_forest_encounter(void);
void obtain_relics(void);
void old_sage_tower(void);

#endif
