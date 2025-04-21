# Text-based-RPG

## Overview

**LEGACY OF YHARNAM: SHADOWS OF THE FORGOTTEN** is a text-based RPG adventure written in C.

You play as a chosen hero on a quest to gather ancient relics, solve puzzles, and defeat powerful foes—including the Dark Lord himself. 
 
The game features branching paths, humorous encounters, turn-based combat, and a classic fantasy storyline.

## Features

- Explore forests, dungeons, and castles

- Solve riddles and puzzles

- Engage in turn-based battles with monsters and bosses

- Collect relics and powerful items

- Make choices that affect your journey

## How to Compile

1. **Ensure you have a C compiler installed** (e.g., `gcc`).
2. **Open a terminal in the project directory.**

### Using the Makefile (Recommended)

If a `Makefile` is provided, simply run:

```sh
make
```

This will build the game and create an executable named `Legacy_of_Yharnam`.

### Manual Compilation

If you prefer to compile manually, you can run:

```sh
gcc -Wall -Werror -ansi -pedantic src/*.c -o Legacy_of_Yharnam
```

Or, if you want to specify files individually:

```sh
gcc -Wall -Werror -ansi -pedantic src/main.c src/player.c src/exploration.c src/quests.c src/combat.c src/common.c src/save.c -o Legacy_of_Yharnam
```

3. **Run the game:**

   ```sh
   ./Legacy_of_Yharnam
   ```

## Notes

- The game is played entirely in the terminal/command prompt.
- Follow on-screen instructions for controls and choices.
- Save/load features may require the game to be run from the same directory.

## Enjoy your adventure!