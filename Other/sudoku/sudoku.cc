#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void printboard( int board[][9] )
{
  for ( int i = 0; i < 9; i++ ) {
    if ( i%3==0 )
      cout << "+---+---+---+" << endl;
    for ( int j = 0; j < 9; j++ ) {
      if ( j%3==0 )
        cout << "|";
      if ( board[i][j] == 0 )
        cout << ' ';
      else
        cout << board[i][j];
    }
    cout << "|" << endl;
  }
  cout << "+---+---+---+" << endl << endl;
}

bool checkRow( int board[][9], int row )
{
  for ( int i = 0; i < 9; i++ )
    if ( board[row][i] != 0 ) { // if not zero check for duplicate value
      int tot = 0;
      for ( int j = 0; j < 9; j++ ) {
        if ( board[row][j] == board[row][i] )
          tot++;
        if ( tot >= 2 )         // if there are 2 of this val row is bad
          return false;
      }
    }
  return true;
}

bool checkCol( int board[][9], int col )
{
  for ( int i = 0; i < 9; i++ )
    if ( board[i][col] != 0 ) {
      int tot = 0;
      for ( int j = 0; j < 9; j++ ) {
        if ( board[j][col] == board[i][col] )
          tot++;
        if ( tot >= 2 )
          return false;
      }
    }
  return true;
}

bool checkBox( int board[][9], int box )
{
  int startRow = box / 3 * 3, startCol = (box % 3) * 3;
  for ( int i = startRow; i < startRow+3; i++ )
    for ( int j = startCol; j < startCol+3; j++ ) {
      if ( board[i][j] != 0 ) {
        int tot = 0;
        for ( int k = startRow; k < startRow+3; k++ )
          for ( int l = startCol; l < startCol+3; l++ ) {
            if ( board[i][j] == board[k][l] )
              tot++;
            if ( tot >= 2 )
              return false;
          }
      }
    }
  return true;
}

bool solveBoard( int board[][9], bool dontchange[][9], int row, int col )
{
  do {  // increment the column until a changable value is reached
    col++;
    if ( row >= 8 && col >= 9 )
      return true;    // reached the end of the board DONE!
    if ( col >= 9 ) // wrap to next row
      col = 0, row++;
  } while ( !dontchange[row][col] );
  int box = col / 3 + row / 3 * 3;  // box index for row and column
  do { 
    board[row][col]++;            // increase value in that location
    if ( board[row][col] > 9 ) {  // if value > 9 then set back to zero and backtrack
      board[row][col] = 0;
      return false;
    }
    if ( checkRow( board, row ) && checkCol( board, col ) && checkBox( board, box ) )
      if ( solveBoard( board, dontchange, row, col ) )
        return true;
  } while ( true );
}

int main()
{
  int board[9][9];        // 2D array holding the board values
  bool dontchange[9][9];  // 2D array holding the locked locations
  ifstream fin("board.txt");
  ofstream fout("results.txt");
  string line;
  fin >> line;  // read the first line
  while ( fin.good() ) {
    for ( int i = 0; i < 9; i++ )   // read values into array
      for ( int j = 0; j < 9; j++ ) {
        board[i][j] = (int)(line[i*9+j])-(int)('0');    // convert from char 0-9 to int 0-9
        if ( board[i][j] == 0 )
          dontchange[i][j] = true;      // dont change this value!
        else
          dontchange[i][j] = false;     // dont not change :)
      }
    printboard(board);
    solveBoard( board, dontchange, 0, -1 );    // solve the board (start at 0,-1)
    printboard(board);
    for ( int i = 0; i < 81; i++ )  // print to file
      fout << board[i/9][i%9];      // i/9 = row, i/9 = col
    fout << endl;
    fin >> line;  // read the next line
  }
  fout.close();
  fin.close();
}

