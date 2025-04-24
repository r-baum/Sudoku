/*****
 * Project 02: Sudoku
 * COSC 208, Introduction to Computer Systems, Spring 2024
 *****/

#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Macro variables
#define ROWS 9
#define COLS 9
#define NUM_PUZZLES 15
#define LINE_SUM 45
#define LINE_LEN 83
#define TOTAL_EMPTY_CELLS 38

//function declare
int **makeBoard(void);
int **makeGameBoard(int **board);
void initializeVals(int **board, char[]);
void printBoard(int **board);
void copyVals(int **dest, int **src);
int **initializeArr(void);
int checkValidBoard(int **board);
void freeArr(int **board);
void drawBoard(WINDOW * win, int **gameBoard, int **solution, int cheatMode, int curY, int curX);
int **makeLockedMask(int **gameBoard);
int checkRowCol(int **board, int x, int y);
int checkArr(int inputArr[]);


int main(int argc, char * argv[]) {
    srand(time(NULL)); //changes the rand function on each function call

    //if cheat mode is active; if "cheat is passed on the command line"
    int cheatMode = 0;
    if (argc > 1 && strcmp(argv[1], "cheat") == 0) {
        cheatMode = 1;
    }

    int **board = makeBoard(); //initializing game board and answer key board
    int **gameBoard = makeGameBoard(board);
    int **locked = NULL;
    if (!cheatMode) {
        //use the answer board as the game board
        locked = makeLockedMask(gameBoard);
    }

    //initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    //looked up how to disable mouse so that the touchpad is ignored
    mousemask(0, NULL);

    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK); //new color pair for cheat mode hints.
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK); //for duplicates

    int curY = 0, curX = 0; //initial cursor position
    //make the game board
    drawBoard(stdscr, gameBoard, board, cheatMode, curY, curX);
    int ch;
    while ((ch = getch()) != 'q') {
        switch(ch) {
            case KEY_UP:
                if (curY > 0) curY--;
                break;
            case KEY_DOWN:
                if (curY < ROWS - 1) curY++;
                break;
            case KEY_LEFT:
                if (curX > 0) curX--;
                break;
            case KEY_RIGHT:
                if (curX < COLS - 1) curX++;
                break;
            case KEY_BACKSPACE:
            //allow for deletion only if in cheat mode or if the cell is not locked
                if (cheatMode || (locked && locked[curY][curX] == 0))
                    gameBoard[curY][curX] = 0;
                break;
            default: 
            //if a number key (1-9) is pressed, update the cell only if allowed
                if (ch >= '1' && ch <= '9') {
                    if (cheatMode || (locked && locked[curY][curX] == 0))
                        gameBoard[curY][curX] = ch - '0';
                }
                break;
        }
        //check for win condition in normal mode and cheat mode
        if (checkValidBoard(gameBoard)) {
            drawBoard(stdscr, gameBoard, board, cheatMode, curY, curX);
            mvwprintw(stdscr, 19, 2, "Congratulations! You solved the puzzle. Press any key to exit.");
            refresh();
            getch();
            break;
        }
        drawBoard(stdscr, gameBoard, board, cheatMode, curY, curX);
    }
    endwin();
    freeArr(board);
    freeArr(gameBoard);
    if (locked) {  // Ensure `locked` was allocated before freeing
        freeArr(locked);
    }
    return 0;
  }

/*
This function uses ncurses to display the game board and instructions.
The instructions are stored in an array and printed using a loop. They are centered both 
vertically (using max_y) and horizontally (using max_x and the string lengths).
The sudoku board is printed below the instructions.
Parameters:
   win        - The ncurses window where the board is drawn.
   gameBoard  - The current game board state.
   solution   - The solution board (used in cheat mode to show correct numbers).
   cheatMode  - Flag indicating if cheat mode is active.
   curY, curX - The current cursor coordinates.
 */
void drawBoard(WINDOW *win, int **gameBoard, int **solution, int cheatMode, int curY, int curX) {
    int max_y, max_x;
    getmaxyx(win, max_y, max_x);
    werase(win);
    box(win, 0, 0);

    char *instructions[] = {
        "Sudoku Instructions:",
        "Objective: Fill the grid so each row, col, and",
        "3x3 subgrid contains numbers 1-9 exactly once.",
        "Controls: Arrow keys = move, 1-9 = enter,",
        "Backspace = clear, q = quit.",
        "Indicators: Cheat hints show the correct number if empty.",
        "Winning: Solve the puzzle with no duplicate numbers.",
        "Note: Duplicate numbers in a row or column are noted."
    };
    int num_instr = sizeof(instructions) / sizeof(instructions[0]);
    int start_instr_row = (max_y - (num_instr + ROWS + 1)) / 2;
    if (start_instr_row < 1)
        start_instr_row = 1;
    for (int i = 0; i < num_instr; i++) {
        int instr_x = (max_x - strlen(instructions[i])) / 2;
        mvwprintw(win, start_instr_row + i, instr_x, "%s", instructions[i]);
    }
    int boardWidth = COLS * 4;
    int boardStartRow = start_instr_row + num_instr + 1;
    int boardStartCol = (max_x - boardWidth) / 2;
    if (boardStartCol < 0)
        boardStartCol = 0;

    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            int screenY = boardStartRow + i;
            int screenX = boardStartCol + j * 4;
            if (i == curY && j == curX) {
                wattron(win, A_REVERSE);
            }
            if (cheatMode && gameBoard[i][j] == 0) {
                wattron(win, COLOR_PAIR(2));
                mvwprintw(win, screenY, screenX, " %d ", solution[i][j]);
                wattroff(win, COLOR_PAIR(2));
            } else {
                if (gameBoard[i][j] == 0) {
                    mvwprintw(win, screenY, screenX, "   ");
                } else {
                    int isDuplicate = !checkRowCol(gameBoard, j, i);
                    if (isDuplicate) {
                        wattron(win, COLOR_PAIR(3));
                        mvwprintw(win, screenY, screenX, " %d ", gameBoard[i][j]);
                        wattroff(win, COLOR_PAIR(3));
                    } else {
                        mvwprintw(win, screenY, screenX, " %d ", gameBoard[i][j]);
                    }
                }
            }
            if (i == curY && j == curX) {
                wattroff(win, A_REVERSE);
            }
        }
    }
    wrefresh(win);
}

/*
allocates the board, reads a random solved board form the txt file, and initalizes values
Initialize a new 2D arraw and allocate appropriate memory
Reading the file with sudoku boards and grabbing a random one
All file techniques learned from stackoverflow:
https://stackoverflow.com/questions/61568058/how-to-read-a-file-with-numbers-on-each-line-in-c 
or OpenAI. ChatGPT, March 2025, https://chat.openai.com.
Read lines until the randomly chosen line
initializes the values of the array
*/
int **makeBoard(void){
    int **board = initializeArr();
    FILE *file = fopen("sudokuboards.txt","r");
    if (!file) {
        perror("Error opening sudokuboards.txt");
        exit(EXIT_FAILURE);
    }
    int randomLine = rand() % NUM_PUZZLES;
    char line[LINE_LEN]; 
    for (int i = 0; i < randomLine; i++){
        if(!fgets(line, sizeof(line), file)) {
            fprintf(stderr, "Error reading board from file.\n");
            exit(EXIT_FAILURE);
        }
    }
    if (!fgets(line, sizeof(line), file)) {
        fprintf(stderr, "Error reading board from file.\n");
        exit(EXIT_FAILURE);
    }
    fclose(file);
    initializeVals(board, line);
    return board;
}

/*
initializing all values of the sudoku board
Function inspired by OpenAI. ChatGPT, March 2025, https://chat.openai.com.
*/
void initializeVals(int **board, char line[]){
    int index = 0;
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            board[i][j] = line[index++] - '0';
        }
    }
}


//function for printing all vals of a 2d array
void printBoard(int **board){
    for (int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/*
puts empty spaces in the game board for the user to play
generate a random row and col to edit
fill non-zero cells with 0
*/
int **makeGameBoard(int **board){
    int **gameBoard = initializeArr();
    copyVals(gameBoard, board);
    int empty = 0;
    while (empty < TOTAL_EMPTY_CELLS){
        int r = rand() % ROWS;
        int c = rand() % COLS;
        if(gameBoard[r][c] != 0){
            gameBoard[r][c] = 0;
            empty++;
        }
    }   
    return gameBoard;
}

//copies values of one 2d array to another
void copyVals(int **dest, int **src){ 
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            dest[i][j] = src[i][j];
        }
    }
}

//initalizes array with appropriate memory
int **initializeArr(){ 
    int **arr = malloc(sizeof(int *) * ROWS);
    if (!arr) {
        perror("Failed to allocate memory for board rows");
        exit(EXIT_FAILURE);
    }
    for (int i = 0;i < ROWS;i++){
        arr[i] = malloc(sizeof(int) * COLS);
        if (!arr[i]) {
            perror("Failed to allocate memory for board columns");
            exit(EXIT_FAILURE);
        }
    }
    return arr;
}

/*
checks if the user solved the sudoku board correctly
checking every row
incomplete row
duplicate in row
check every column
incomplete column
duplicate in column
*/
int checkValidBoard(int **board){
    for (int i = 0; i < ROWS; i++) {
        int line[9] = {0};
        for (int j = 0; j < COLS; j++) {
            int num = board[i][j];
            if (num == 0)
                return 0;
            if (line[num - 1] != 0)
                return 0;
            line[num - 1] = num;
        }
    }
    for (int j = 0; j < COLS; j++) {
        int line[9] = {0};
        for (int i = 0; i < ROWS; i++) {
            int num = board[i][j];
            if (num == 0)
                return 0;
            if (line[num - 1] != 0)
                return 0;
            line[num - 1] = num;
        }
    }
    return 1;
}

//checks that there are all numbers 1-9 in each row and column
int checkArr(int inputArr[]){
    for(int i = 0; i < ROWS; i++){
        if(inputArr[i] == 0)
            return 0;
    }
    return 1;
}

//frees allocated memory for 2D array
void freeArr(int **board){
    for(int i = ROWS - 1; i >= 0; i--){
        free(board[i]);
    }
    free(board);
}

/*
check if a number is duplicated in a row or column
no duplication to check for an empty cell
check the row
if count > 1, we found duplicates in the row
check the column
if count > 1, we found duplicates in the column
no duplicates
*/
int checkRowCol(int **board, int x, int y) {
    int num = board[y][x];
    if (num == 0) {
        return 1;
    }
    int count = 0;
    for(int j = 0; j < COLS; j++){
        if (board[y][j] == num) {
            count++;
        }
    }
    if (count > 1) return 0;
    count = 0;
    for(int i = 0; i < ROWS; i++){
        if (board[i][x] == num){
            count++;
        }
    }
    if (count > 1) return 0;
    return 1;
}

/*
creates a 2d mask for gameBoard cells
inspired from stackoverflow:
https://stackoverflow.com/questions/2595392/what-does-the-question-mark-and-the-colon-ternary-operator-mean-in-objectiv
*/
int **makeLockedMask(int **gameBoard) {
    int **mask = initializeArr();
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            mask[i][j] = (gameBoard[i][j] != 0) ? 1 : 0;
        }
    }
    return mask;
} 