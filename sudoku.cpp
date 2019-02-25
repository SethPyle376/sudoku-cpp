/***********************************************************************
* Program:
*    Project 13, Sudoku
*    Brother Comeau, CS124
* Author:
*    Jason Pyle
* Summary:
*    This program allows the user to read, edit, and save sudoku files.
*    Essentially allowing him to play sudoku with commands in the prompt.
*    In addition, the program will prevent the user from inputting values
*    that are invalid, and will show the user possible values when prompted.
*
*    Estimated:  2.0 hrs
*    Actual:     2.0 hrs
*      Finding possible values was difficult, but i'm proud of it haha
************************************************************************/
#include <fstream>
#include <iostream>
using namespace std;

void promptUser(int board[][9]);
void editSquare(int board[][9]);
void saveBoard(int board[][9]);
void readBoard(int board[][9]);
void displayOptions(bool prompt, int board[][9]);
void writeLine(int line, int board[][9]);
void showPossibilities(int board[][9]);
void displayBoard(int board[][9]);
void processCommand(char command, int board[][9]);
void clearArray(int size, int array[]);
bool isPossible(bool possibilities[], int value);
void getPossibilities(int board[][9], int x, int y, bool possibilities[]);

/*************************************************************************
 * This function enables the user to edit individual members of the sudoku
 * array. A coordinate is prompted from the user, the letter coordinate is
 * changed to uppercase for readability, then the human readable coordinate
 * is changed to a coordinate that can be used to access the correct member
 * of the array, which is then changed to the value that the user was
 * prompted for. If the user enters a value that is invalid, an error
 * message will be displayed.
 ************************************************************************/
void editSquare(int board[][9])
{
   char coordinates[2];
   int actualx;
   int actualy;
   int newValue;
   bool possibilities[10];
   cout << "What are the coordinates of the square: ";
   cin >> coordinates;

   if (coordinates[0] > 90)
      coordinates[0] -= 32;

   actualx = coordinates[1] - 49;
   actualy = coordinates[0] - 65;

   if (board[actualx][actualy] != 0)
   {
      cout << "ERROR: Square '" << coordinates[0] << coordinates[1]
           << "' is filled" << endl;

   }
   else
   {
      cout << "What is the value at '" << coordinates[0]
           << coordinates[1] << "': ";
      cin >> newValue;
      getPossibilities(board, actualx, actualy, possibilities);
      if (isPossible(possibilities, newValue))
         board[actualx][actualy] = newValue;
      else
         cout << "ERROR: Value '" << newValue << "' in square '"
              << coordinates[0] << coordinates[1] << "' is invalid" << endl;
   }
   cout << endl;
   promptUser(board);

}

/****************************************************************************
 * This saves the board to a file which the user is prompted to name. A space
 * is given between each value and a newline after each row of values.
 ***************************************************************************/
void saveBoard(int board[][9])
{
   char fileName[32];
   cout << "What file would you like to write your board to: ";
   cin >> fileName;

   ofstream fout(fileName);

   for (int i = 0; i <= 8; i++)
   {
      for (int j = 0; j <= 8; j++)
      {
         fout << board[i][j];

         if (j == 8)
            fout << endl;
         else
            fout << ' ';
      }
   }
   cout << "Board written successfully" << endl;
}

/************************************************************************
 * This function takes the board as well as the x,y coordinates of the
 * value in question. The possible values for the square will be determined
 * and stored in an array of boolean values that is given as a variable to
 * the function. I.e. if 7 is a possibility, possible[7] will be true.
 ***********************************************************************/
void getPossibilities(int board[][9], int x, int y, bool possible[10])
{
   int peers[18];
   int quadrantpeers[9];
   int quadrantpeercount = 0;
   int quadrant;
   int missingx;
   int missingy;
   int xoffset;
   int yoffset;

   clearArray(18, peers);
   clearArray(9, quadrantpeers);

   if (x < 3 && y < 3)
      quadrant = 1;
   else if (x < 3 && y >= 3 && y >= 3 && y < 6)
      quadrant = 2;
   else if (x < 3 && y >= 6)
      quadrant = 3;
   else if (x >= 3 && x < 6 && y < 3)
      quadrant = 4;
   else if (x >= 3 && x < 6 && y >= 3 && y < 6)
      quadrant = 5;
   else if (x >= 3 && x < 6 && y >= 6)
      quadrant = 6;
   else if (x >= 6 && y < 3)
      quadrant = 7;
   else if (x >= 6 && y >= 3 && y < 6)
      quadrant = 8;
   else if (x >= 6 && y >= 6)
      quadrant = 9;


   //Get peers in same row
   for (int i = 0; i <= 9; i++)
   {
      if (i != y)
         peers[i] = board[x][i];
   }

   //Get peers in same column
   for (int i = 0; i <= 9; i++)
   {
      if (i != x)
         peers[i + 9] = board[i][y];
   }

   //Get peers from same quadrant
   if (quadrant == 1)
      for (int i = 0; i < 3; i++)
         for (int j = 0; j < 3; j++)
         {
            quadrantpeers[quadrantpeercount] = board[i][j];
            quadrantpeercount++;
         }

   if (quadrant == 2)
      for (int i = 0; i < 3; i++)
         for (int j = 3; j < 6; j++)
         {
            quadrantpeers[quadrantpeercount] = board[i][j];
            quadrantpeercount++;
         }

   if (quadrant == 3)
      for (int i = 0; i < 3; i++)
         for (int j = 0; j < 9; j++)
         {
            quadrantpeers[quadrantpeercount] = board[i][j];
            quadrantpeercount++;
         }

   if (quadrant == 4)
      for (int i = 3; i < 6; i++)
         for (int j = 0; j < 3; j++)
         {
            quadrantpeers[quadrantpeercount] = board[i][j];
            quadrantpeercount++;
         }

   if (quadrant == 5)
      for (int i = 3; i < 6; i++)
         for (int j = 3; j < 6; j++)
         {
            quadrantpeers[quadrantpeercount] = board[i][j];
            quadrantpeercount++;
         }

   if (quadrant == 6)
      for (int i = 3; i < 6; i++)
         for (int j = 6; j < 9; j++)
         {
            quadrantpeers[quadrantpeercount] = board[i][j];
            quadrantpeercount++;
         }

   if (quadrant == 7)
      for (int i = 6; i < 9; i++)
         for (int j = 0; j < 3; j++)
         {
            quadrantpeers[quadrantpeercount] = board[i][j];
            quadrantpeercount++;
         }

   if (quadrant == 8)
      for (int i = 6; i < 9; i++)
         for (int j = 3; j < 6; j++)
         {
            quadrantpeers[quadrantpeercount] = board[i][j];
            quadrantpeercount++;
         }

   if (quadrant == 9)
      for (int i = 6; i < 9; i++)
         for (int j = 6; j < 9; j++)
         {
            quadrantpeers[quadrantpeercount] = board[i][j];
            quadrantpeercount++;
         }

   for (int i = 0; i < 10; i++)
      possible[i] = true;

   for (int i = 0; i < 18; i++)
   {
      if (peers[i] != 0)
         possible[peers[i]] = false;
   }


   for (int i = 0; i < 9; i++)
   {
      if (quadrantpeers[i] != 0)
         possible[quadrantpeers[i]] = false;

   }

}

/*************************************************************************
 * This function compares the value in question against the array of
 * possibilities. If it indeed is a possibility, it will return true.
 ************************************************************************/
bool isPossible(bool possibilities[], int value)
{
   if (possibilities[value])
      return true;
   else
      return false;
}

/************************************************************************
 * This is a helper function that will set the values of a given array
 * of given size to 0.
 ***********************************************************************/
void clearArray(int size, int array[])
{
   for (int i = 0; i < size; i++)
      array[i] = 0;
}

/**************************************************************************
 * This function prompts the user for the filename of the sudoku puzzle,
 * then reads the values in that sudoku file into the 2D array "board".
 *************************************************************************/
void readBoard(int board[][9])
{
   char fileName[24];
   cout << "Where is your board located? ";
   cin >> fileName;
   ifstream fin(fileName);

   for (int i = 0; i <= 8; i++)
      for (int j = 0; j <= 8; j++)
         fin >> board[i][j];
}

/**************************************************************************
 * This function displays the available options to the user.
 *************************************************************************/
void displayOptions(bool prompt, int board[][9])
{
   cout << "Options:" << endl;
   cout << "   ?  Show these instructions" << endl;
   cout << "   D  Display the board" << endl;
   cout << "   E  Edit one square" << endl;
   cout << "   S  Show the possible values for a square" << endl;
   cout << "   Q  Save and Quit" << endl << endl;

   if (prompt)
   {
      cout << endl;
      promptUser(board);
   }
}

/**************************************************************************
 * This function writes an individual line of the sudoku puzzle. If a value
 * equals 0 a space will be placed instead. A space goes between each value
 * unless a | character is needed. Also if a horizontal divider is needed
 * underneath the line, the function will write it.
 *************************************************************************/
void writeLine(int line, int board[][9])
{
   cout << line + 1 << "  ";

   for (int i = 0; i <= 8; i++)
   {
      if (board[line][i] != 0)
         cout << board[line][i];
      else
         cout << " ";

      if (i != 2 && i != 5 && i != 8)
         cout << " ";
      else if (i != 8)
         cout << "|";
      else
         cout << endl;
   }
   if (line == 2 || line == 5)
      cout << "   -----+-----+-----" << endl;
   if (line == 8)
   {
      cout << endl;
      promptUser(board);
   }


}


/**************************************************************************
 * This function will get the possibilities for place on the board in question,
 * then display to the user the possible values of that square.
 *************************************************************************/
void showPossibilities(int board[][9])
{
   char coordinates[2];
   int actualx;
   int actualy;
   int newValue;
   bool possibilities[10];
   int possibilityCount = 0;
   cout << "What are the coordinates of the square: ";
   cin >> coordinates;

   if (coordinates[0] > 90)
      coordinates[0] -= 32;

   actualx = coordinates[1] - 49;
   actualy = coordinates[0] - 65;

   getPossibilities(board, actualx, actualy, possibilities);
   cout << "The possible values for '" << coordinates[0]
        << coordinates[1] << "' are: ";

   for (int i = 1; i < 10; i++)
   {
      if (possibilities[i])
         possibilityCount++;
   }

   for (int i = 1; i < 10; i++)
   {
      if (possibilities[i])
      {
         cout << i;
         possibilityCount--;
         if (possibilityCount)
            cout << ", ";
      }

   }
   cout << endl << endl;
   promptUser(board);
}

/**************************************************************************
 * This function uses the writeLine function for each line of the sudoku
 * puzzle to be written.
 *************************************************************************/
void displayBoard(int board[][9])
{
   cout << "   A B C D E F G H I" << endl;

   for (int i = 0; i <= 8; i++)
      writeLine(i,board);

}

/**************************************************************************
 * This function takes the command the user gave in the promptUser function
 * and calls the correct function for the command.
 *************************************************************************/
void processCommand(char command, int board[][9])
{
   if (command == '?')
      displayOptions(true, board);
   else if (command == 'D' || command == 'd')
      displayBoard(board);
   else if (command == 'E' || command == 'e')
      editSquare(board);
   else if (command == 'S' || command == 's')
      showPossibilities(board);
   else if (command == 'Q' || command == 'q')
      saveBoard(board);
   else
      cout << "Invalid command." << endl;
}

/**************************************************************************
 * This simply asks the user for a command and stores it in a char. Then
 * passes the char to processCommand to be executed.
 *************************************************************************/
void promptUser(int board[][9])
{
   char command;
   cout << "> ";
   cin >> command;
   processCommand(command,board);
}

/**********************************************************************
 * This is the main driver of the program, prompting the user for a file,
 * displaying the options and the board, then letting the user command
 * from there.
 ***********************************************************************/
int main()
{
   int board[9][9];
   readBoard(board);
   displayOptions(false, board);
   displayBoard(board);

   return 0;
}
