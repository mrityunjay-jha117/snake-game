#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>

using namespace std;
//it is the maximum length of the snake
#define MAX_LENGTH 1000
const char dir_up = 'u';
const char dir_down = 'd';
const char dir_right = 'r';
const char dir_left = 'l';
//console dimensions
int consoleheight, consolewidth;
//initialise screen
void initscreen() {
    HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hconsole, &csbi);
    consoleheight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    consolewidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
}
//point structure here it has two coordinates
struct point {
    int xcoord;
    int ycoord;

    point() {}

    point(int x, int y) {
        xcoord = x;
        ycoord = y;
    }
};

class Snake {
    int length;
    int direction;

public:
    point body[MAX_LENGTH];

    Snake(int x, int y) {
        length = 1;
        body[0] = point(x, y);
        direction = dir_right;
    }

    int getlength() {
        return length;
    }

    void changedirection(char newdirection) {
        if (newdirection == dir_up && direction != dir_down) {
            direction = newdirection;
        } else if (newdirection == dir_down && direction != dir_up) {
            direction = newdirection;
        } else if (newdirection == dir_right && direction != dir_left) {
            direction = newdirection;
        } else if (newdirection == dir_left && direction != dir_right) {
            direction = newdirection;
        }
    }

    bool move(point food) {
        // Store the current head position
        point previous_head = body[0];

        // Update Head Position Based on Direction
        switch (direction) {
            case dir_up:
                body[0].ycoord--;
                break;
            case dir_down:
                body[0].ycoord++;
                break;
            case dir_right:
                body[0].xcoord++;
                break;
            case dir_left:
                body[0].xcoord--;
                break;
        }

        // Boundary Collision Check
        if (body[0].xcoord < 0 || body[0].xcoord >= consolewidth ||
            body[0].ycoord < 0 || body[0].ycoord >= consoleheight) {
            return false; // Game Over
        }

        // Snake Bites Itself Check
        for (int i = 1; i < length; i++) {
            if (body[0].xcoord == body[i].xcoord && body[0].ycoord == body[i].ycoord) {
                return false; // Game Over
            }
        }

        // Move Snake Body
        for (int i = length - 1; i > 0; i--) {
            body[i] = body[i - 1];
        }

        // Snake Eats Food
        if (food.xcoord == body[0].xcoord && food.ycoord == body[0].ycoord) {
            if (length < MAX_LENGTH) {
                body[length] = previous_head; // Grow the snake
                length++;
            }
            return true; // Snake is still alive
        }

        return true; // Snake has not eaten food
    }
};

// The rest of your code remains unchanged



class Board {
    Snake *snake;
    const char snake_body = 'O';
    point food;
    const char Food = 'o';
    int score;

public:
    Board() {
        snake = new Snake(100, 10);
        score = -1;
        spawnfood();
    }

    ~Board() {
        delete snake;
        snake = nullptr;
    }

    void spawnfood() {
        bool onSnake;
        do {
            onSnake = false;
            int x = rand() % consolewidth;
            int y = rand() % consoleheight;
            food = point(x, y);

            // Check if the food position matches any part of the snake's body
            for (int i = 0; i < snake->getlength(); i++) {
                if (snake->body[i].xcoord == food.xcoord && snake->body[i].ycoord == food.ycoord) {
                    onSnake = true;
                    break;
                }
            }
        } while (onSnake); // Repeat until food is not on the snake
    }

    void displayscores() {
        gotoxy(consolewidth / 2, 0);
        cout << "Current score: " << score;
    }

    int getscore() {
        return score;
    }

    void gotoxy(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void draw() {
        system("cls");
        for (int i = 0; i < snake->getlength(); i++) {
            gotoxy(snake->body[i].xcoord, snake->body[i].ycoord);
            cout << snake_body;
        }
        gotoxy(food.xcoord, food.ycoord);
        cout << Food;
        displayscores();
    }

    bool update() {
        bool isalive = snake->move(food);
        if (!isalive) {
            return false;
        }
        if (food.xcoord == snake->body[0].xcoord && food.ycoord == snake->body[0].ycoord) {
            score++;
            spawnfood();
        }
        return true;
    }

    void getinput() {
        if (kbhit()) {
            int key = getch();
            if (key == 'w' || key == 'W') {
                snake->changedirection(dir_up);
            } else if (key == 'a' || key == 'A') {
                snake->changedirection(dir_left);
            } else if (key == 's' || key == 'S') {
                snake->changedirection(dir_down);
            } else if (key == 'd' || key == 'D') {
                snake->changedirection(dir_right);
            }
        }
    }
};

int main() {
    srand(time(0));
    initscreen();
    Board *board = new Board();
    while (board->update()) {
        board->getinput();
        board->draw();
        Sleep(100); // Adjusted for better playability
    }
    cout << "Game Over" << endl;
    cout << "Final score is: " << board->getscore() << endl;
    delete board;
    board = nullptr;
    return 0;
}
