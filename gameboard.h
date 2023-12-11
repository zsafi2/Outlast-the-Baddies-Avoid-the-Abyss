/*-------------------------------------------
Program 7: Outlast the Baddies & Avoid the Abyss
Course: CS 211, Fall 2023, UIC
System: Visual Studio Code
Author: Zaheer Safi
------------------------------------------- */

#ifndef _GAMEBOARD_H
#define _GAMEBOARD_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <stdexcept>

#include "boardcell.h"
#include "grid.h"

using namespace std;

class GameBoard {
	private: 
	    Grid<BoardCell*> board;
        size_t numRows;
        size_t numCols;
        int HeroRow; // Hero's position row
	    int HeroCol; // Hero's position column
        int numMonsters;
        int numSuperMonsters;
        int numAbysses;
        int numBats;
        bool wonGame; // false, unless the Hero reached the exit successfully

		
	public: 
		/* default constructor */
        GameBoard() {
            numMonsters = 4;
            numSuperMonsters = 2;
            numAbysses = 50;
            numBats = 2;
            wonGame = false;
            
            this -> numRows = 15;
            this -> numCols = 40;
            
            Grid<BoardCell*> boardnew(numRows, numCols);
            board = boardnew;
            
            blankBoard();
        }
        
        /* param constructor */
        GameBoard(size_t numRows, size_t numCols) {
            numMonsters = 4;
            numSuperMonsters = 2;
            numAbysses = 20;
            numBats = 3;
            wonGame = false;
            
            this -> numRows = numRows;
            this -> numCols = numCols;
            
            Grid<BoardCell*> boardnew(numRows, numCols);
            board = boardnew;
            
            blankBoard();
        }
        
        /* destructor */
        virtual ~GameBoard() {
            for (size_t row = 0; row < board.numrows(); row++) {
                for (size_t col = 0; col < board.numcols(row); col++) {
                    delete board(row, col);
                }
            }
        }

        void blankBoard() {
            for (size_t row = 0; row < board.numrows(); row++) {
                for (size_t col = 0; col < board.numcols(row); col++) {
                    board(row, col) = new Nothing(row,col);
                }
            }
        }

        char getCellDisplay(size_t r, size_t c) {
            return board(r,c)->display();
        }

        void setCell(BoardCell* myCell, size_t r, size_t c) {
            board(r,c) = myCell;
        }
    
        void freeCell(size_t r, size_t c) {
            delete board(r,c);
        }

        // fills board with by randomly placing...
        //  - Hero (H) in the first three columns
        //  - EscapeLadder (*) in last three columns
        //  - 3 vertical Walls (+), each 1/2 of board height, in middle segment
        //  - Abyss cells (#), quantity set by numAbysses, in middle segment
        //  - Baddies [Monsters (m), Super Monsters (M), & Bats (~)] in middle segment;
        //    number of Baddies set by numMonsters, numSuperMonsters, & numBats
        void setupBoard(int seed) {
            srand(seed);
            size_t r,c;

            r = rand() % numRows;
            c = rand() % 3;
            delete board(r,c);
            board(r,c) = new Hero(r,c);
            HeroRow = r;
            HeroCol = c;

            r = rand() % numRows;
            c = numCols - 1 - (rand() % 3);
            delete board(r,c);
            board(r,c) = new EscapeLadder(r,c);
            
            int sizeMid = numCols - 6;

            c = 3 + (rand() % sizeMid);
            for (r = 0; r < numRows/2; ++r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);
            }
            size_t topc = c;

            while (c == topc || c == topc-1 || c == topc+1) {
                c = 3 + (rand() % sizeMid);
            }
            for (r = numRows-1; r > numRows/2; --r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);           
            }
            size_t botc = c;

            while (c == topc || c == topc-1 || c == topc+1 || c == botc || c == botc-1 || c == botc+1) {
                c = 3 + (rand() % sizeMid);
            }
            for (r = numRows/4; r < 3*numRows/4; ++r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);
            }

            for (int i = 0; i < numMonsters; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Monster(r,c);  
                board(r,c)->setPower(1);        
            }

            for (int i = 0; i < numSuperMonsters; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Monster(r,c); 
                board(r,c)->setPower(2);               
            }

            for (int i = 0; i < numBats; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Bat(r,c); 
            }

            for (int i = 0; i < numAbysses; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Abyss(r,c);              
            }
        }

        // neatly displaying the game board 
		void display( ) {
            cout << '-';
            for (size_t col = 0; col < board.numcols(0); col++) {
                cout << '-';
            }
            cout << '-';
            cout << endl;
            for (size_t row = 0; row < board.numrows(); row++) {
                cout << '|';
                for (size_t col = 0; col < board.numcols(row); col++) {
                    cout << board(row,col)->display();
                }
                cout << '|';
                cout << endl;
            }
            cout << '-';
            for (size_t col = 0; col < board.numcols(0); col++) {
                cout << '-';
            }
            cout << '-';
            cout << endl;
            
        }
		
        bool getWonGame() {
            return wonGame;
        }
        
        // distributing total number of monsters so that 
        //  ~1/3 of num are Super Monsters (M), and
        //  ~2/3 of num are Regular Monsters (m)
        void setNumMonsters(int num) {
            numSuperMonsters = num/3;
            numMonsters = num - numSuperMonsters;
        }

        void setNumAbysses(int num) {
            numAbysses = num;
        }

        void setNumBats(int num) {
            numBats = num;
        }

        size_t getNumRows() {
            return numRows;
        }

        size_t getNumCols() {
            return numCols;
        }

        // getter function for the hero postion which are private member veriables
        void getHeroPosition(size_t& row, size_t& col) {
            
            row = HeroRow;  // change the row veriable to heroRow
            col = HeroCol;  // change the column veriable to heroCol
        }

        // setter function to set the function of the hero given the row and col parameters
        void setHeroPosition(size_t row, size_t col) {
            
            // change the private member data veriables according to the row and col parameters
            HeroRow = row;
            HeroCol = col;
        
        }

        // function to find the postion of the hero on the board
        void findHero() {
            
            // go through each row and column in the board 
            for (size_t row = 0; row < board.numrows(); row++) 
            {
                for (size_t col = 0; col < board.numcols(row); col++) 
                {
                    // if the display in the cell is H which means it is the hero change the veriable heroRow and heroCol
                    if (getCellDisplay(row, col) == 'H')
                    {
                        HeroRow = row;
                        HeroCol = col;
                        return;
                    }
                }
            }

            HeroRow = -1;
            HeroCol = -1;
        }

        // Function to handle the movment of hero and baddie on the game board and take care of all the illegal moves
        bool makeMoves(char HeroNextMove) 
        {
            // Variables to store the new row and column for Hero
            size_t newR, newC;

            // Set Hero's next move on the current board position
            board(HeroRow, HeroCol)->setNextMove(HeroNextMove);
            // Attempt to move Hero to a new position and get the new row and column
            board(HeroRow, HeroCol)->attemptMoveTo(newR, newC, HeroRow, HeroCol);

            // Check if Hero's new row is out-of-bounds
            try 
            {
                if (newR < 0 || newR >= numRows) { 
                    throw runtime_error("Hero trying to move out-of-bounds with an invalid row");
                } 
            }
            catch (runtime_error& excptRow) 
            {
                cout << excptRow.what() << endl;
                newR = HeroRow;
                cout << "Changing row for Hero position to stay in-bounds" << endl;
            }

            // Check if Hero's new column is out-of-bounds
            try 
            {
                if (newC < 0 || newC >= numCols) 
                { 
                    throw runtime_error("Hero trying to move out-of-bounds with an invalid column");
                } 
            }
            catch (runtime_error& excptCol) 
            {
                cout << excptCol.what() << endl;
                newC = HeroCol;
                cout << "Changing column for Hero position to stay in-bounds" << endl;
            }

            // Check if Hero is attempting to move into a wall
            try 
            {
                if (getCellDisplay(newR, newC) == '+') 
                { 
                    throw runtime_error("Hero trying to move into a wall");
                } 
            }
            catch (runtime_error& excptWall) 
            {
                cout << excptWall.what() << endl;
                if (HeroNextMove == 'd' || HeroNextMove == 'a' || HeroNextMove == 'x' || HeroNextMove == 'w' || HeroNextMove == 's')
                {
                    newC = HeroCol;
                    newR = HeroRow;
                }
                else
                {
                    newC = HeroCol;
                }
                cout << "Changing column for Hero position to not crash into a wall" << endl;
            }

            // Check if Hero has reached the goal ('*')
            if (getCellDisplay(newR, newC) == '*')
            {
                delete board(HeroRow, HeroCol);
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                wonGame = true;
                return false;
            }

            // Check if Hero has collided with an obstacle ('#', '~', 'm', 'M')
            if (getCellDisplay(newR, newC) == '#' || getCellDisplay(newR, newC) == '~' || getCellDisplay(newR, newC) == 'm' || getCellDisplay(newR, newC) == 'M')
            {
                delete board(HeroRow, HeroCol);
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                wonGame = false;
                return false;
            }

            // Update Hero's position on the board
            board(newR, newC)->setRow(newR);
            board(newR, newC)->setCol(newC);
            
            delete board(HeroRow, HeroCol);
            board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
            
            delete board(newR, newC);
            board(newR, newC) = new Hero(newR, newC);
            
            HeroRow = newR;
            HeroCol = newC;

            // Variables to store the new row and column for Baddies
            size_t newBadR, newBadC;
            // Flag to track if Hero crashed into a Baddie
            bool crashed_hero = false;

            // Loop through each cell on the board
            for (size_t row = 0; row < board.numrows(); row++) 
            {
                for (size_t col = 0; col < board.numcols(row); col++) 
                {
                    // Check if the current cell contains a Baddie that hasn't moved yet
                    bool check = board(row, col)->getMoved();
                    char baddie = getCellDisplay(row, col);
                    
                    if ((baddie == '~' || baddie == 'm' || baddie == 'M') && !check)
                    {
                        // Attempt to move the Baddie to a new position and get the new row and column
                        board(row, col)->attemptMoveTo(newBadR, newBadC, newR, newC);
                        
                        // Check if Baddie's new row is out-of-bounds
                        try 
                        {
                            if (newBadR < 0 || newBadR >= numRows) 
                            { 
                                throw runtime_error("Baddie trying to move out-of-bounds with an invalid row");
                            } 
                        }
                        catch (runtime_error& excptBadRow) 
                        {
                            cout << excptBadRow.what() << endl;
                            newBadR = row;
                            cout << "Changing row for Baddie position to stay in-bounds" << endl;
                        }

                        // Check if Baddie's new column is out-of-bounds
                        try 
                        {
                            if (newBadC < 0 || newBadC >= numCols) 
                            { 
                                throw runtime_error("Baddie trying to move out-of-bounds with an invalid column");
                            } 
                        }
                        
                        catch (runtime_error& excptBadCol) 
                        {
                            cout << excptBadCol.what() << endl;
                            newBadC = col;
                            cout << "Changing column for Baddie position to stay in-bounds" << endl;
                        }

                        // Check if Baddie is attempting to move into a wall
                        try 
                        {
                            if (getCellDisplay(newBadR, newBadC) == '+' || getCellDisplay(newBadR, newBadC) == '*') 
                            { 
                                throw runtime_error("Baddie trying to move into a wall");
                            } 
                        }
                        catch (runtime_error& excptBadWall) 
                        {
                            cout << excptBadWall.what() << endl;
                            if (HeroNextMove == 'd' || HeroNextMove == 'a' || HeroNextMove == 'x' || HeroNextMove == 'w' || HeroNextMove == 's')
                            {
                                newBadC = col;
                                newBadR = row;
                            }
                            else
                            {
                                newBadC = col;
                            }
                            cout << "Changing column for Baddie position to not crash into a wall" << endl;
                        }
                        
                        // Check if Baddie has collided with a wall ('#')
                        if (getCellDisplay(newBadR, newBadC) == '#')
                        {
                            delete board(row, col);
                            board(row, col) = new Nothing(row, col);
                        }
                        else
                        {
                            // Check if Baddie has collided with another Baddie ('~' or 'm' or 'M')
                            if (getCellDisplay(newBadR, newBadC) == '~' || getCellDisplay(newBadR, newBadC) == 'm' || getCellDisplay(newBadR, newBadC) == 'M')
                            {
                                newBadC = col;
                                newBadR = row;
                            }

                            // Check if Baddie has collided with Hero ('H')
                            if (getCellDisplay(newBadR, newBadC) == 'H')
                            {
                                crashed_hero = true;
                            }
                            
                            // Update Baddie's position on the board
                            board(newBadR, newBadC)->setRow(newBadR);
                            board(newBadR, newBadC)->setCol(newBadC);
                            
                            // Delete the old positions of Baddie and Hero
                            delete board(row, col);
                            board(row, col) = new Nothing(row, col);
                            delete board(newBadR, newBadC);
                            
                            // Create a new Baddie at the new position based on the Baddie type
                            if (baddie == '~')
                            {
                                board(newBadR, newBadC) = new Bat(newBadR, newBadC);
                            }
                            else if (baddie == 'm')
                            {
                                board(newBadR, newBadC) = new Monster(newBadR, newBadC);
                            }
                            else if (baddie == 'M')
                            {
                                board(newBadR, newBadC) = new Monster(newBadR, newBadC);
                                board(newBadR, newBadC)->setPower(2);
                            }
                            
                            // Set the moved flag for the Baddie to true
                            board(newBadR, newBadC)->setMoved(true);
                        }
                    }
                }
            }

            // Reset the moved flag for all Baddies on the board
            for (size_t row = 0; row < board.numrows(); row++) 
            {
                for (size_t col = 0; col < board.numcols(row); col++) 
                {
                    char baddie1 = getCellDisplay(row, col);
                    
                    if ((baddie1 == '~' || baddie1 == 'm' || baddie1 == 'M'))
                    {
                        board(row, col)->setMoved(false);
                    }
                }
            }

            // If Hero crashed into a Baddie, return false
            if (crashed_hero)
            {
                return false;
            }

            // If no collision occurred, return true
            return true;
        }


};

#endif //_GAMEBOARD_H