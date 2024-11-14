#ifndef MINESWEEPER_TILEANDMINES_H
#define MINESWEEPER_TILEANDMINES_H
#endif //MINESWEEPER_TILEANDMINES_H
using namespace std;
#include <iostream>
#include <map>
#include <vector>
#include <time.h>

class Textures{
public:

};

class Tile{
public:
    bool is_mine;
    int neighbor_mines;
    bool is_revealed;
    bool is_flagged;


    void loadtextures(){
        cout << "LOADED" << endl;
    }


};

//recursion to test if adjecent mines are cleared
void revealAdjacentTiles(int row, int col, std::vector<std::vector<Tile>> &gameBoard, int numRows, int numCols) {
    // Base case: If the current tile is a mine or has already been revealed, return or if flagged dont reveal
    if(gameBoard[row][col].is_flagged){
        return;
    }
    if (row < 0 || row >= numRows || col < 0 || col >= numCols || gameBoard[row][col].is_mine || gameBoard[row][col].is_revealed) {
        gameBoard[row][col].is_revealed = true;
        return;
    }

    // Set the current tile as revealed
    gameBoard[row][col].is_revealed = true;

    // If the current tile has no neighboring mines, recursively reveal adjacent tiles
    if(gameBoard[row][col].neighbor_mines == 0){
    for (int row_off = -1; row_off <= 1; ++row_off) {
        for (int col_off = -1; col_off <= 1; ++col_off) {
            int neighrow = row + row_off;
            int neighcol = col + col_off;
            if (neighrow >= 0 && neighrow < numRows && neighcol >= 0 && neighcol < numCols) {
                revealAdjacentTiles(neighrow, neighcol, gameBoard, numRows, numCols);
                 }
            }
        }
    }
}




int random_num_gen(int lowerbound, int upperbound, int seeder){
    srand(time(0) + seeder);
    int random_number = (rand() % (upperbound - lowerbound)) + lowerbound;
    return random_number;
}




vector<vector<Tile>> initialize(int cols, int rows, int num_mines){
    vector<vector<Tile>> gameBoard(rows, std::vector<Tile>(cols));
    for (int cur_row = 0; cur_row < rows; ++cur_row) {
        for (int cur_col = 0; cur_col < cols; ++cur_col) {
            gameBoard[cur_row][cur_col].is_mine = false;
            gameBoard[cur_row][cur_col].neighbor_mines = 0;
            gameBoard[cur_row][cur_col].is_revealed = false;
            gameBoard[cur_row][cur_col].is_flagged = false;
            //gameBoard[cur_row][cur_col].loadtextures();
        }
    }

    //now we place the mines randomly
    int randomizer = 0;
    for (int i = 0; i < num_mines;) {
        int rowy = random_num_gen(0, rows, randomizer + i);
        int colx = random_num_gen(0, cols, randomizer * i);
        if (!gameBoard[rowy][colx].is_mine) {
            gameBoard[rowy][colx].is_mine = true;
            i++;
            randomizer++;
            cout << " col: " << colx << " row: " << rowy << endl;

        }
        //cout << "RANDOMIZER CLUTCH!!!" << endl;
        randomizer++;
    }

    for (int cur_row = 0; cur_row < rows; ++cur_row) {
        for (int cur_col = 0; cur_col < cols; ++cur_col) {
            if(!gameBoard[cur_row][cur_col].is_mine){
                for(int row_off = -1; row_off <= 1; row_off++) {
                    for (int col_off = -1; col_off <= 1; col_off++) {
                        int neighrow = cur_row + row_off;
                        int neighcol = cur_col + col_off;
                        if(neighrow >= 0 && neighrow < rows && neighcol >= 0 && neighcol < cols && gameBoard[neighrow][neighcol].is_mine){
                            gameBoard[cur_row][cur_col].neighbor_mines++;
                        }
                    }
                }
            }
        }
    }


    return gameBoard;
}


void printer(vector<vector<Tile>> myboard, int cols, int rows){
    for (int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(myboard[i][j].is_mine){
                cout << "X ";
            } else{
                cout << myboard[i][j].neighbor_mines << " ";
            }
        }
        cout << endl;
    }
}

bool wincheck(vector<vector<Tile>> myboard, int cols, int rows, int number_ofmines, int flagsss){

    int revealed_nonmines = 0;
    //check every mine that is revealed and is non mine add one
    for (int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++) {
            if (myboard[i][j].is_revealed && !myboard[i][j].is_mine) {
                revealed_nonmines++;
            }
        }
    }
    //returns true if all non mine tiles are revealed
    if(revealed_nonmines == (rows * cols) - number_ofmines){
        //sets all mines to flagged? Idk why the pdf said to do this
        for (int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++) {
                if(myboard[i][j].is_mine) {
                    myboard[i][j].is_flagged = true;
                }
            }
        }
        return true;
    }

    if(flagsss != 0){
        return false;
    }
    int counter = 0;
    //ensures that there are no flagged regular tiles
    for (int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(myboard[i][j].is_flagged && !myboard[i][j].is_mine){
                return false;
            }
        }
    }


    //ensures all mines are flagged
    for (int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(myboard[i][j].is_mine && myboard[i][j].is_flagged){
                counter++;
            }
        }
    }
    if(counter == number_ofmines){
        return true;
    }

    return false;
}


