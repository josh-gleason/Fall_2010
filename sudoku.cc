#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

bool checkRow( int board[][9], int row );
bool checkCol( int board[][9], int col );
bool checkBox( int board[][9], int box );

bool solveBoard( int board[][9], bool dontchange[][9], int row, int col )
{
	if ( row >= 9 && col > 9 )
		return true;
	if ( col >= 9 )
	{
		col = 0;
		row++;
	}

	while ( dontchange[row][col] )
	{
		col++;
		if ( row >= 9 && col > 9 )
			return true;
		if ( col >= 9 )
		{
			col = 0;
			row++;
		}
	}

	int box = col % 3 + row / 3 * 3;

	do
	{ 
		board[row][col]++;
		if ( board[row][col] > 9 )
		{
			board[row][col] = 0;
			return false;
		}
	}
	while ( !checkRow( board, row ) || !checkCol( board, col ) || !checkBox( board, box ) );

}

void printboard( int board[][9] )
{
	for ( int i = 0; i < 9; i++ )
	{
		for ( int j = 0; j < 9; j++ )
		{
			if ( !board[i][j] )
				cout << "  ";
			else
				cout << board[i][j];
		}
		cout << endl;
	}
}

int main()
{
	int board[9][9];
	bool dontchange[9][9];

	string line;
	{
		ifstream fin("board.txt");
		fin >> line;
		fin.close();
	}
	for ( int i = 0; i < 9; i++ )
		for ( int j = 0; j < 9; j++ )
		{
			board[i][j] = (int)(line[i*9+j])-(int)('0');
			if ( !board[i][j] )
				dontchange[i][j] = true;
			else
				dontchange[i][j] = false;
		}

	cout << endl;
	printboard( board );
	cout << endl << endl;
	solveBoard( board, dontchange, 0, 0 );
	printboard( board );
	cout << endl << endl;
}

bool checkRow( int board[][9], int row )
{
	return false;
}

bool checkCol( int board[][9], int col )
{
	return false;
}

bool checkBox( int board[][9], int box )
{
	return false;
}
