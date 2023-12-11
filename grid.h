/*-------------------------------------------
Program 7: Outlast the Baddies & Avoid the Abyss
Course: CS 211, Fall 2023, UIC
System: Visual Studio Code
Author: Zaheer Safi
------------------------------------------- */
#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>
#include <algorithm>

using namespace std;

// Template class for a 2D grid
template<typename T>
class Grid {
private:
  // Inner struct representing a row in the grid
  struct ROW {
    T*  Cols;      // Array of column elements
    size_t NumCols; // Total number of columns (0..NumCols-1)
  };

  ROW* Rows;        // Array of ROWs
  size_t  NumRows;  // Total number of rows (0..NumRows-1)
    
public:
  
  // Default constructor: creates a 4x4 grid and initializes elements to default values
  Grid() {
    Rows = new ROW[4];  // 4 rows
    NumRows = 4;

    // Initialize each row to have 4 columns:
    for (size_t r = 0; r < NumRows; ++r) {
      Rows[r].Cols = new T[4];
      Rows[r].NumCols = 4;

      // Initialize the elements to their default value:
      for (size_t c = 0; c < Rows[r].NumCols; ++c) {
        Rows[r].Cols[c] = T();  // Default value for type T
      }
    }
  }

  // Parameterized constructor: creates an RxC grid and initializes elements to default values
  Grid(size_t R, size_t C) {
    // Validate input parameters
    if (R < 1) 
    {
        throw invalid_argument("Grid::constructor: # of rows must be greater than 0");
    }
    if (C < 1) 
    {
        throw invalid_argument("Grid::constructor: # of columns must be greater than 0");
    }
    
    // Allocate memory for Rows
    Rows = new ROW[R]; 
    NumRows = R;

    // Initialize each row to have C columns:
    for (size_t r = 0; r < NumRows; ++r) 
    {
        Rows[r].Cols = new T[C];
        Rows[r].NumCols = C;

        // Initialize the elements to their default value:
        for (size_t c = 0; c < Rows[r].NumCols; ++c) 
        {
            Rows[r].Cols[c] = T();  
        }
    }
  }
    
  // Destructor: deallocates memory
  virtual ~Grid() {
      // Deallocate memory for each row's columns
      for (size_t r = 0; r < NumRows; ++r) {
            delete[] Rows[r].Cols;
        }
        // Deallocate memory for Rows
        delete[] Rows;
  }

  // Copy constructor: creates a deep copy of another grid
  Grid(const Grid<T>& other) {
      // Allocate memory for Rows
      this->Rows = new ROW[other.NumRows];  
      this->NumRows = other.NumRows;

      // Copy each element from the other grid
      for (size_t r = 0; r < other.NumRows; ++r) 
      {
          size_t other_numcols = other.Rows[r].NumCols;

          // Allocate memory for each row's columns
          this->Rows[r].Cols = new T[other_numcols];
          this->Rows[r].NumCols = other_numcols;

          // Copy elements from the other grid
          for (size_t c = 0; c < other_numcols; ++c) 
          {
              this->Rows[r].Cols[c] = other.Rows[r].Cols[c];
          }
      }
  }

  // Copy assignment operator: assigns the values of another grid to this grid
  Grid& operator=(const Grid& other) {
    // Check for self-assignment
    if (this == &other) 
    {
        return *this;
    }
        
    // Deallocate memory for current Rows
    for (size_t r = 0; r < NumRows; ++r) 
    {
        delete[] Rows[r].Cols;
    }
    delete[] Rows;
    
    // Allocate memory for new Rows
    this->Rows = new ROW[other.NumRows];  
    this->NumRows = other.NumRows;
    
    // Copy each element from the other grid
    for (size_t r = 0; r < other.NumRows; ++r) 
    {
        size_t other_numcols = other.Rows[r].NumCols;
        
        // Allocate memory for each row's columns
        this->Rows[r].Cols = new T[other_numcols];
        this->Rows[r].NumCols = other_numcols;
        
        // Copy elements from the other grid
        for (size_t c = 0; c < other_numcols; ++c) 
        {
            this->Rows[r].Cols[c] = other.Rows[r].Cols[c];
        }
    }
    return *this;  
  }

  // Getter for the number of rows
  size_t numrows() const {
    return NumRows;
  }
  
  // Getter for the number of columns in a specific row
  size_t numcols(size_t r) const {
      // Validate row index
      if (r < 0 || r >= NumRows)
      {
          throw invalid_argument("Grid::numcols getter: Invalid row index");
      }
      
      return Rows[r].NumCols; 
  }

  // Getter for the total number of elements in the grid
  size_t size() const {
    size_t total = 0;
    // Calculate the total number of elements in the grid
    for (size_t i = 0; i < NumRows; i++)
    {
        total += Rows[i].NumCols;
    }
    return total; 
  }

  // Accessor for grid elements using parentheses notation
  T& operator()(size_t r, size_t c) {
      // Validate row and column indices
      if (r < 0 || r >= NumRows || c < 0 || c >= Rows[r].NumCols)
      {
          throw invalid_argument("Grid::operator(): Invalid row or column index");
      }
      return Rows[r].Cols[c];
  }

  // Output the grid elements
  void _output() {
    // Output the grid elements in a formatted manner
    for (size_t r = 0; r < NumRows; ++r) 
    {
          cout << "| ";
          for (size_t c = 0; c < Rows[r].NumCols; ++c) 
          {
              // Output elements in reverse order (bottom to top)
              cout << Rows[NumRows-r-1].Cols[c] << " | ";
          }
          cout << endl;
      }   
  }
};
