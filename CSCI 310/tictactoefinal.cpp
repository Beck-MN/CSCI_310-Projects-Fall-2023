#include <iostream>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <ctime>

using namespace std;

const int ROW_COLUMN = 3;
char board[ROW_COLUMN][ROW_COLUMN];
bool gameOver = false;
mutex mtx;
bool player1Turn = true;

bool checkWin(char player); // Check to see if the game is over. 
bool immediateWin(char player); // See if player can get immediate win.
bool isBoardFull();  // Check if board is full
void lockPlayer(char player); // lock mutex
void unlockPlayer(); // Unlock mutex
bool makeMove(char player, char opponent); // Logic for players to make decision
void randomMove(char player); // Random decision if immediate win or block is not an option
void startNewGame(); // Show gameboard reset with no X's and O's
void printBoard();  // Display gameboard
void player1();
void player2();

int main()
{
    startNewGame();

    thread p1(player1);
    thread p2(player2);

    p1.join();
    p2.join(); 

    if(isBoardFull())
        {
            gameOver = true;
            cout << "The game has ended with a tie" << endl;
        }

    return 0;
}

void startNewGame()
{
    for (int i = 0; i < ROW_COLUMN; ++i)
        for (int j = 0; j < ROW_COLUMN; ++j)
            board[i][j] = ' ';
    cout << "Tic Tac Toe\n"
         << endl;
    printBoard();
}

void printBoard()
{
    for (int i = 0; i < ROW_COLUMN; ++i)
    {
        for (int j = 0; j < ROW_COLUMN; ++j)
        {
            cout << " " << board[i][j] << " ";
            if (j != 2)
                cout << "|";
        }
        cout << endl;
        if (i != 2)
            cout << "---+---+---" << endl;
    }
    cout << endl;
}

bool isBoardFull()
{ // Loop through the 2d array to see if there are any open positions. 
    for (int i = 0; i < ROW_COLUMN; ++i)
    {
        for (int j = 0; j < ROW_COLUMN; ++j)
        {
            if (board[i][j] == ' ')
            {
                return false;
            }
        }
    }
    return true;
}
// Check to see if a player has won the game. 
bool checkWin(char player)
{
    // Check rows, columns, and diagonals for a win
    for (int i = 0; i < ROW_COLUMN; ++i)
    {
        if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
            (board[0][i] == player && board[1][i] == player && board[2][i] == player))
        {
            return true;
        }
    }
    if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
        (board[0][2] == player && board[1][1] == player && board[2][0] == player))
    {
        return true;
    }
    return false;
}
// If immediate win have the player choose that position. 
bool immediateWin(char player)
{
    for (int i = 0; i < ROW_COLUMN; ++i)
    {
        for (int j = 0; j < ROW_COLUMN; ++j)
        {
            if (board[i][j] == ' ')
            {
                board[i][j] = player;
                if (checkWin(player))
                {
                    return true;
                }
                board[i][j] = ' ';
            }
        }
    }
    return false;
}
// Choose a posistion to block your oppenent from win. 
bool blockPlayer(char player, char player2)
{
    bool moved = false;
    while (!moved)
    {
        for (int i = 0; i < ROW_COLUMN; ++i)
        {
            for (int j = 0; j < ROW_COLUMN; ++j)
            {
                if (board[i][j] == ' ')
                {
                    board[i][j] = player2;
                    if (checkWin(player2))
                    {
                        moved = true;
                        board[i][j] = player;
                        return true;
                    }
                    board[i][j] = ' ';
                }
            }
        }
        moved = true;
    }
    return false;
}
// Mutex lock 
void lockPlayer(char player)
{
    mtx.lock();
    cout << "Player " << player << "'s turn" << endl;
}
// Mutex unlock
void unlockPlayer()
{
    mtx.unlock();
}
// Logic to make decision of which position to select. 
bool makeMove(char player, char player2)
{
    if (immediateWin(player)) {
        gameOver = true;
        cout << "Player '" << player << "' wins!" << endl;
        return true;
    }

    if (blockPlayer(player, player2)) {
        return true;
    }
    for (int i = 0; i < ROW_COLUMN; ++i)
        {
            for (int j = 0; j < ROW_COLUMN; ++j)
            {
                if (board[i][j] == ' ')
                {
                    board[i][j] = player;
                    if (checkWin(player))
                    {
                        gameOver = true;
                        cout << "Player '" << player << "' wins!" << endl;
                        return true;
                    }
                    board[i][j] = ' ';
                }
            }
        } 
    return false;
}
// function to select a random available position on the board. 
void randomMove(char player)
{
    bool moved = false;

    while (!moved)
    {
        srand((unsigned)time(0));
        int row = rand() % 3;
        int column = rand() % 3;

        if (row >= 0 && row < ROW_COLUMN && column >= 0 && column < ROW_COLUMN && board[row][column] == ' ')
        {
            board[row][column] = player;
            moved = true;
        }
    }
}
// Function for brains of player 1
void player1()
{
    while (!gameOver)
    {
        if (player1Turn)
        {
            lockPlayer('X');

            if (!makeMove('X', 'O'))
            {
                randomMove('X');
            }

            printBoard();
            unlockPlayer();
            player1Turn = false;
        }
        this_thread::sleep_for(chrono::seconds(3));
          if(isBoardFull())
        {
            gameOver = true;
        
        }
      
    }
}
// Function for brains of player 2
void player2()
{
    while (!gameOver)
    {
        if (!player1Turn)
        {
            lockPlayer('O');

            if (!makeMove('O', 'X'))
            {
                randomMove('O');
            }

            printBoard();
            unlockPlayer();
            player1Turn = true;
        }
        this_thread::sleep_for(chrono::seconds(3));
        
         if(isBoardFull())
        {
            gameOver = true;
        
        }
    }
}
