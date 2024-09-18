#include <iostream>
#include <cstdlib>   // rand 
#include <conio.h>   // _getch and _kbhit
#include <windows.h> // For Sleep
#include <ctime>     // for time
#include <deque>
#include <unordered_set>

using namespace std;

// Snake game class
class Snake {
private:
  bool gameOver; // Flag to indicate game over
  int width, height, fruitX, fruitY, score, speed; // Game settings and score
  deque<pair<int, int>> snake; // Snake body positions
  unordered_set<int> snakeBody; // Set to store snake body positions for collision detection
  enum Direction { STOP, LEFT, RIGHT, UP, DOWN }; // Enum for snake direction
  Direction dir; // Current snake direction
  char directionSymbol; // Symbol to display for snake direction

  // Helper function to hide the cursor
  void hideCursor() {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(hStdOut, &curInfo);
    curInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hStdOut, &curInfo);
  }

  // Helper function to set cursor position
  void setCursorToXY(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
  }

public:
  Snake() {
    setup(); // Initialize game settings
  }
   void displayRules(); // Display game rules
  void setup(); // Initialize game settings
  void draw(); // Draw game board and snake
  void input(); // Handle user input
  void run(); // Update game state
  bool isGameOver() { return gameOver; } // Check if game is over
  int getSpeed() { return speed; } // Get current game speed
  int getHash(int x, int y) { return y * width + x; } // Calculate hash for snake body positions
  void addSnakeBody(int x, int y) { snakeBody.insert(getHash(x, y)); } // Add snake body position to set
  void removeSnakeBody(int x, int y) { snakeBody.erase(getHash(x, y)); } // Remove snake body position from set

  ~Snake() {} // Destructor
};

int main() {
  Snake showRules;
  showRules.displayRules(); // Display game rules
  char ch;
  do {
    Snake snake;
    system("cls"); // Clear console
    while (!snake.isGameOver()) {
      snake.draw(); // Draw game board and snake
      snake.input(); // Handle user input
      snake.run(); // Update game state
      Sleep(snake.getSpeed()); // Wait for a short period of time
    }
    cout << "Game Over!!!" << endl;
    cout << "Press 'r' to restart the game and any other key to exit." << endl;
    ch = tolower(_getch()); // Get user input
  } while (ch == 'r'); 
  return 0;
}

void Snake::displayRules() {
  system("cls");
  cout << "Snake Game Rules:" << endl;
  cout << "-----------------" << endl;
  cout << "1. Use 'W', 'A', 'S', 'D' keys to move the snake up, left, down, and right respectively." << endl;
  cout << "2. Eat the food (*) to increase your score and length of the snake." << endl;
  cout << "3. Avoid hitting the boundaries or the snake's own body." << endl;
  cout << "4. Press 'X' to quit the game." << endl;
  cout << "5. The game will speed up as you eat more food." << endl;
  cout << "Press any key to start the game..." << endl;
  _getch();// Wait for user input
}

void Snake::setup() {
  srand(time(0)); // Seed random number generator
  dir = RIGHT; // Initialize snake direction
  width = 60; // Set game board width
  height = 20; // Set game board height
  speed = 100; // Initialize game speed
  snake.push_front({width / 2, height / 2}); // Initialize snake position
  fruitX = rand() % width; // Randomly generate fruit position
  fruitY = rand() % height;
  score = 0; // Initialize score
  gameOver = 0; // Initialize game over flag
  hideCursor(); // Hide cursor
  directionSymbol = '>'; // Initialize direction symbol
  addSnakeBody(width / 2, height / 2); // Add initial snake body position
}

void Snake::draw() {
   setCursorToXY(0, 0); // Set cursor position
  // Draw the top boundary
  for (int i = 0; i < width + 2; i++) {
    cout << "#";
  }
  cout << endl;

  // Draw the middle Boundary
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      // Left boundary
      if (j == 0) {
        cout << "#";
      }

      // Snake Food
      if (i == fruitY && j == fruitX) {
        cout << "*"; 
      }
      else if (snakeBody.count(getHash(j, i))) {
        if (snake.front().first == j && snake.front().second == i) {
          cout << directionSymbol; // Snake head
        } else {
          cout << "o"; // Snake body
        }
      } else {
        cout << " ";
      }

      // Right boundary
      if (j == width - 1) {
        cout << "#";
      }
    }
    cout << endl;
  }

  // Draw the Bottom boundary
  for (int i = 0; i < width + 2; i++) {
    cout << "#";
  }
  cout << endl;

  // Show score and Level
  cout << "Score : " << score << "\tPress x to quit the game " << endl;
}

void Snake::input() {
  if (_kbhit()) {
    char key = tolower(_getch());
    switch (key) {
      case 'a': if (dir != RIGHT) dir = LEFT; directionSymbol = '<'; break;
      case 'd': if (dir != LEFT) dir = RIGHT;  directionSymbol = '>'; break;
      case 'w': if (dir != DOWN) dir = UP;  directionSymbol = '^'; break;
      case 's': if (dir != UP) dir = DOWN;  directionSymbol = 'v'; break;
      case 'x': gameOver = true; break;
    }
  }
}

void Snake::run()
{
  if (dir == STOP) return;
 // Move the head
  pair<int, int> newHead = snake.front();
  switch (dir) {
    case LEFT: newHead.first--; break;
    case RIGHT: newHead.first++; break;
    case UP: newHead.second--; break;
    case DOWN: newHead.second++; break;
    default: break;
  }

  snake.push_front(newHead); // Add new head position

  // Check for boundary collisions
  if (newHead.first >= width || newHead.first < 0 || newHead.second >= height || newHead.second < 0) {
    gameOver = true;
    return;
  }

   // Check for self-collision
  if (snakeBody.count(getHash(newHead.first, newHead.second))) {
    gameOver = true;
    return;
  }

  addSnakeBody(newHead.first, newHead.second); // Add to snake body set

  // Food regenration and increasing snake speed and length of snake
  if (newHead.first == fruitX && newHead.second == fruitY) {
    score += 10;
    fruitX = rand() % width;
    fruitY = rand() % height;
    speed = (speed > 0) ? speed - 5 : speed;    // Increase the speed
  } else {
    // If no food eaten, remove the last tail piece
    removeSnakeBody(snake.back().first, snake.back().second);
    snake.pop_back();
  }
}
