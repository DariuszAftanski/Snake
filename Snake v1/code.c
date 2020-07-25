
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

struct frame_size {
    int left_coordinate;
    int bottom_coordinate;
    int right_coordinate;
    int top_coordinate;
};

struct position_coordinates {
    int x;
    int y;
};

enum possible_moves {
    STOP,       // = 0
    LEFT,       // = 1
    RIGHT,      // = 2
    UP,         // = 3
    DOWN        // = 4
};

// Initialise the frame_size - frame struct:
struct frame_size frame;
// Initialise the position_coordinates - snake(with tail) struct:
struct position_coordinates snake[30];
// Initialise the position_coordinates - fruit struct:
struct position_coordinates fruit;

int game_over;      // Variable that decides whether to keep playing, or not.
int score;          // Variable that keeps the number of points we've scored playing the game.
int direction;      // Direction the snake moves.

// Generate_random - creates a 'pseudo' random number
int generate_random(int lower_bound, int upper_bound) {
    srand(time(0));
    return (rand() % (upper_bound - lower_bound - 1)) + lower_bound + 1;
}

// Setup - is a first function executed in the program. It initialises all the variables.
void setup(void) {
    // Start the game:
    int game_over = 0;

    // Initialise the score counter to 0.
    score = 0;
    // Default direction the snake moves:
    direction = STOP;

    // Initialise the frame variables:
    frame.left_coordinate   = 10;
    frame.bottom_coordinate = 2;
    frame.right_coordinate  = 30;
    frame.top_coordinate    = 15;

    // Initialise the position_coordinates - snake variables:
    snake[0].x = (frame.left_coordinate + frame.right_coordinate) / 2;
    snake[0].y = (frame.bottom_coordinate + frame.top_coordinate) / 2;

    // Initialise the position_coordinates - fruit variables:
    fruit.x = generate_random(frame.left_coordinate, frame.right_coordinate);
    fruit.y = generate_random(frame.bottom_coordinate, frame.top_coordinate);
}

// Draw - present the state of the game.
void draw(void) {
    // Clear the terminal:
    system("clear");

    // Draw the game's frame:
        // Go through space bottom to top:
        for (int i = 0; i <= frame.top_coordinate; i++ ) {
            // Start from coordinate bottom_coordinate:
            while (i < frame.bottom_coordinate) {printf("\n"); i++;}
            // Go through space left to right:
            for (int j = 0; j <= frame.right_coordinate; j++) {
                // Start from coordinate left_coordinate:
                while (j < frame.left_coordinate) {
                    printf(" ");
                    j++;
                }
                    if (i == frame.bottom_coordinate || i == frame.top_coordinate) { printf("#"); }
                    else if (j == frame.left_coordinate || j == frame.right_coordinate) { printf("#"); }
                    else if (j == snake[0].x && i == snake[0].y) { printf("O"); }
                    else if (j == fruit.x && i == fruit.y) { printf("F"); }
                    else if (score > 0) { 
                        for (int k = 1; k <= score; k++){
                            if (j == snake[k].x && i == snake[k].y) { 
                                printf("•"); 
                                break;}
                            else if (k == score) { printf(" "); }
                        }
                    }
                    else { printf(" "); }
            }
            printf("\n");
        }
        printf("\nScore: %d\n", score);
}

// Input - takes input from the user.
void input(void) {
    int s = getchar();
    switch(s) {
        case 68:
            direction = LEFT;   // Left Arrow.
            break;
        case 67:
            direction = RIGHT;  // Right Arrow.
            break;
        case 65:
            direction = UP;     // Up Arrow.
            break;
        case 66:
            direction = DOWN;   // Down Arrow.
            break;
        case 'Q':
            game_over = 1;
            break;
        case '\n':
            direction = STOP;
    }
}

// Logic - executes the rules of the game.
void logic(void) {
    // If the last move is not STOP, save it to the array:
    if (direction != STOP) {
        for (int i = score + 1; i > 0; i--) {
            snake[i].x = snake[i - 1].x;
            snake[i].y = snake[i - 1].y;
        }
    }
    // Execute the direction command given by the user.
    switch(direction) {
        case LEFT:
            snake[0].x = snake[1].x - 1;
            snake[0].y = snake[1].y;
            break;
        case RIGHT:
            snake[0].x = snake[1].x + 1;
            snake[0].y = snake[1].y;
            break;
        case UP:
            snake[0].x = snake[1].x;
            snake[0].y = snake[1].y - 1;
            break;
        case DOWN:
            snake[0].x = snake[1].x;
            snake[0].y = snake[1].y + 1;
            break;
    }
    // Generate new fruit, once the current one is eaten
    if (snake[0].x == fruit.x && snake[0].y == fruit.y) {
        fruit.x = generate_random(frame.left_coordinate, frame.right_coordinate);
        fruit.y = generate_random(frame.bottom_coordinate, frame.top_coordinate);  
        // Check if the fruit is not placed on the tail.       
        for (int k = 1; k <= score; k++) {
            while (fruit.x == snake[k].x && fruit.y == snake[k].y) {
                fruit.x = generate_random(frame.left_coordinate, frame.right_coordinate);
                fruit.y = generate_random(frame.bottom_coordinate, frame.top_coordinate);            
            }
        }
        // Reward the user with one point per fruit.
        score++;
    }
    // End the game if snake hits the wall.
    if (snake[0].x == frame.left_coordinate || snake[0].x == frame.right_coordinate || snake[0].y == frame.top_coordinate || snake[0].y == frame.bottom_coordinate)  {
        game_over = 1;
    }
    // End the game if snake hits his tail.
    for (int k = 1; k <= score; k++) {
        if (snake[0].x == snake[k].x && snake[0].y == snake[k].y) {
            game_over = 1;
        }
    }
}

int main(void) {
    // Initialise variables:
    setup();
    // Display the starting map:
    draw();

    while (game_over == 0) {    
        input();
        logic();
        draw();
    }

    // Print a message after the game is lost.
    printf("You lost! (Score: %d)\nDeath is not so bad after all!\n\n\n", score);
    return 0;
}
