/*-------------------------------------------
Program 7: Outlast the Baddies & Avoid the Abyss
Course: CS 211, Fall 2023, UIC
System: Visual Studio Code
Author: Zaheer Safi
------------------------------------------- */

#ifndef _BOARDCELL_H
#define _BOARDCELL_H

using namespace std;

// First, the BoardCell abstract base class 

class BoardCell {
	
    public:  
		BoardCell() {} // default contstructor (do nothing)
        virtual ~BoardCell() {} // destructor (do nothing)
        
        virtual char display( ) = 0; // pure virtual function; this is an abstract base class

    	// parent boardcell class attempt move class which is a virutal funciton
        virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {
            
            // change the newR and newC to the current row and column in 2d grid
            newR = myRow; 
            newC = myCol; 
        }
        
        virtual bool isHero( ) {return false;}
    	virtual bool isBaddie( ) {return false;}
    	virtual bool isSpace( ) {return false;}
        virtual bool isBarrier( ) {return false;}
        virtual bool isHole( ) {return false;}
    	virtual bool isExit( ) {return false;}
    	virtual bool isStatic( ) {return true;}

        virtual void setPower(int num) {}
        virtual int getPower() {return 1;}
        virtual void setNextMove(char inChar) {}
        
        void setMoved(bool m) {moved = m;}
        bool getMoved() {return moved;}
        void setRow(size_t r) {myRow = r;}
        size_t getRow() {return myRow;}
        void setCol(size_t c) {myCol = c;}
        size_t getCol() {return myCol;}
        void setPos(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }

    private:
        size_t myRow; // current row for this board cell in a 2D grid
        size_t myCol; // current column for this board cell in a 2D grid
        bool moved;   // true = this board cell already moved in the current round

}; // BoardCell (abstract base class)


// Then, all the derived classes

class Hero: public BoardCell {
	
    public:
    	Hero(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        
        virtual bool isHero( ) {return true;}
        virtual bool isStatic( ) {return false;}
        virtual char display( ) {return 'H';}
        
        virtual void setNextMove(char inChar ) {
            nextMove = inChar;
        }
    	
       // overide the attemptMoveto function for the hero which depends on the user input
       virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {
            
            // Determine attempted new position based on nextMove and change accordingly
            switch (nextMove) 
            {
                // if the input is q which means move on the diagonal left up so change the current positon of hero accordinglu
                case 'q':
                    newR = hRow - 1;
                    newC = hCol - 1;
                    break;
                
                // continue in the same manner change the positon of the hero according to the user input
                
                case 'w':
                    newR = hRow - 1;
                    newC = hCol;
                    break;
                
                case 'e':
                    newR = hRow - 1;
                    newC = hCol + 1;
                    break;
                
                case 'a':
                    newR = hRow;
                    newC = hCol - 1;
                    break;
                
                case 's':
                    newR = hRow;
                    newC = hCol;
                    break;
                
                case 'd':
                    newR = hRow;
                    newC = hCol + 1;
                    break;
                
                case 'z':
                    newR = hRow + 1;
                    newC = hCol - 1;
                    break;
                
                case 'x':
                    newR = hRow + 1;
                    newC = hCol;
                    break;
                
                case 'c':
                    newR = hRow + 1;
                    newC = hCol + 1;
                    break;
                
                default:
                    newR = hRow;
                    newC = hCol;
                    break;
            }
        }
    private:
        char nextMove;

}; // Hero


class Monster: public BoardCell {
    
    public:
		Monster(size_t r, size_t c) {
            setRow(r);
            setCol(c);
            power = 1;
        }
        
        virtual bool isBaddie( ) {return true;}
        virtual bool isStatic( ) {return false;}
        
        virtual char display( ) {
            if (power == 2) {
                return 'M'; // Super Monster
            } else {
                return 'm'; // Monster
            } 
        }

        virtual void setPower(int num) {
            power = num;
        }

        virtual int getPower() {
            return power;
        }
        
        // another override function of the attemptMoveTo to move the monsters (small and big) according to the positon of the hero
    	virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {

            size_t verticalMove; // veriable to check the vertical movment of the hero
            size_t horizontalMove; // veriable to check the horizonal movment of the hero

            // check the positon of the hero with respect to its last positon and update the veriables above
            if (hRow > getRow()) 
            {
                verticalMove = 1;
            } 
            else if (hRow < getRow()) 
            {
                verticalMove = -1;
            } 
            else 
            {
                verticalMove = 0;
            }

            // again check the vertical movment of the hero
            if (hCol > getCol()) 
            {
                horizontalMove = 1;
            } else if (hCol < getCol()) 
            {
                horizontalMove = -1;
            } 
            else 
            {
                horizontalMove = 0;
            }

            // if it is the big monster meaning th power is 2
            if (power == 2) 
            {
                // Super Monster - make 2-step move in each direction
                newR = getRow() + 2 * verticalMove;
                newC = getCol() + 2 * horizontalMove;
            } 
            
            // if a regular monster
            else 
            {
                // Regular Monster - make 1-step move in each direction
                newR = getRow() + verticalMove;
                newC = getCol() + horizontalMove;
            }
        }
            
            
        
    
    private:
        int power; // power = 1 for Monster, power = 2 for SuperMonster

}; // Monster

class Bat: public BoardCell {
    public:
		Bat(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual bool isBaddie( ) {return true;}
        virtual bool isStatic( ) {return false;}
        virtual char display( ) {return '~';}
        
    	// another override function this time to move the bat according to the hero 
        virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {
            
            // bats only move to the same column as the hero their row does not change
            newR = getRow(); // stay on the same row
            newC = hCol;  // change the column ot the hero column           
        }

}; // Bat


class Abyss: public BoardCell {
	public:
    	Abyss(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual char display( ) {return '#';}
        virtual bool isHole( ) {return true;}
}; // Abyss


class Wall: public BoardCell {
	public:
    	Wall(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual char display( ) {return '+';}
    	virtual bool isBarrier( ) {return true;}
}; // Wall


class Nothing: public BoardCell {
	public:
    	Nothing(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual char display( ) {return ' ';}
    	virtual bool isSpace( ) {return true;}
}; // Nothing


class EscapeLadder: public BoardCell {
	public:
    	EscapeLadder(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual char display( ) {return '*';}
    	virtual bool isExit( ) {return true;}
}; // EscapeLadder


#endif //_BOARDCELL_H
