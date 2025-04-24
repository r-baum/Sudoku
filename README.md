# Project 02: Grid Game

In this project, you'll implement a game that uses a two-dimensional grid. You can choose from any of the following games:
* [2048](https://en.wikipedia.org/wiki/2048_(video_game))
* [Boggle](https://en.wikipedia.org/wiki/Boggle)
* [Minesweeper](https://en.wikipedia.org/wiki/Minesweeper_(video_game))
* [Nonogram](https://en.wikipedia.org/wiki/Nonogram)
* [Sudoku](https://en.wikipedia.org/wiki/Sudoku)
* [Wordle](https://en.wikipedia.org/wiki/Wordle)
* Your own idea that has been approved by Kevin

### Important tips
* **Start the project shortly after it is released and work on the project over multiple sessions.** This will give you time to think about how to solve problems, allow you to ask questions, and result in better outcomes. The opportunity to revise your project is contingent upon your git commit history demonstrating that you started the project shortly after it was released and you worked on the project over multiple sessions.
* **Follow good [program design](#program-design), [testing, and debugging](#testing-and-debugging) practices** The code you submit will be assessed for both functionality and design.

## Requirements
Clone your repository on your RPi. Write your code in the `game.c` file included in your repository. Use the included `Makefile` to compile your code.

Your game must be meet **all** of the following requirements:
* Allocate a 2D array on the heap to store the main game state
* When the game starts, print instructions on how to play the game
* Read (e.g., using `getch` or `getstr`) and validate user input
* Display the game state using the ncurses library – read the [Ncurses Programming Guide](https://jbwyatt.com/ncurses.html) to learn how to use this library
* End the game based on appropriate criteria
* Free all heap-allocated memory
* Run without any memory errors or leaks (check using `valgrind`)
* Provide a "cheat mode" so that your instructor can quickly win or lose the game, set by passing "cheat" on the command line.

## Program design

A program whose design is **satisfactory** must adhere to **all of the following**:
* **No memory errors:** Run your game with valgrind to discover and debug memory errors.
* **No memory leaks:** Run your game with valgrind to discover and debug memory leaks. You **must** use the command `valgrind --leak-check=full --suppressions=game.sp ./game` to ignore memory leaks within the ncurses library.
* **Use multiple functions:** Do not put all of your code in the `main` function.
* **Avoid duplicated code:** Do not repeat the same (or very similar) code in multiple functions. Instead, put the repeated code into a separate "helper" function that is called by other functions that rely on this code.
* **Avoid global variables:** Use local variables and parameters instead.
* **Use constants:** #define constants at the top of `game.c` to avoid putting fixed numeric (or character) values in function bodies.
* **Fix compilation warnings:** You may be tempted to ignore the warnings, but they almost always mean there is a bug in your code.
* **Include comments:** Each function (except for main), must be preceded by a short comment that describes what the function does.

A program that adheres to **most, but not all, of the above** has a design that is **close to satisfactory**.

## Testing and debugging your code

The first step in debugging your code is to **fix any errors or warnings output by the compiler.**  If the program compiles without errors or warnings but still doesn't work as expected, then **add `printf` statements in strategic locations** to help you understand what your program is doing.

Play your game several times with different inputs.

A program whose functionality is **satisfactory** must have two or fewer minor bugs, such as:
* Allowing one form of invalid user input
* Violating a rule of game play under specific, uncommon scenarios
* Expected format of user input is not intuitive – ask friends to play and provide feedback on your game
* Display of game state is not intuitive – again, ask friends to play and provide feedback on your game

A program whose functionality is **close to satisfactory** must have two or fewer major bugs, such as:
* Game play rules are vague not provided 
* Crashing during game play due to a memory error
* Displaying the wrong game state
* Allowing multiple forms of invalid user input
* Violating a rule of game play during most/all games
* Game does not end at the appropriate point

## Submission instructions
You should **commit and push** your updated `game.c` file to your git repository. However, do not wait until your entire program is working before you commit it to your git repository; you should commit your code each time you write and debug a piece of functionality.
