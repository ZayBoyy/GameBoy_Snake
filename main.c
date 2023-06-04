#include <gb/gb.h>
#include <stdio.h>
#include <rand.h>
#include <time.h>
#include <stdbool.h>
#include "faceSprite.c"
#include "PlayerIdle.c"
#include "Pellet.c"
#include "SnakeTiles.c"

typedef struct 
{
    UINT8 position [2];
    UINT8 prevDirection;
    UINT8 counter;
    UINT8 direction;
    UINT8 sprite;
    UINT8 visual;
    
}snakeData;


bool valid, alive;
UINT8 idleCounter, toggle;
UINT8 coordinates[2] = {84, 88};
UINT8 pelletX, pelletY, headPosition, snakeSize, input;
snakeData snake[255];

void gameOver();
void spawnPellet();
void growSnake();
void drawSnake();
void headForward();
void bodyForward();
void playerMoveDetect();
void checkEaten();





void main () {
    alive = true;
    idleCounter = 0;
    toggle = 0;
    snakeSize = 3;
    snake[0].direction = 0;
    snake[0].sprite = 1;
    snake[0].visual = 3;
    snake[0].position[0] = 40;
    snake[0].position[1] = 40;
    snake[1].direction = 0;
    snake[1].sprite = 2;
    snake[1].visual = 6;
    snake[1].position[0] = 32;
    snake[1].position[1] = 40;
    snake[2].direction = 0;
    snake[2].sprite = 3;
    snake[2].visual = 13;
    snake[2].position[0] = 24;
    snake[2].position[1] = 40;

    
    
    SPRITES_8x8;
    set_sprite_data(0, 1, Pellet);
    set_sprite_data(1, 14, Snake);
    set_sprite_tile(0, 0);
    set_sprite_tile(1, 2);
    set_sprite_tile(2, 6);
    set_sprite_tile(3, 13);
    SHOW_SPRITES;

    spawnPellet();

    while (alive) {
        input = joypad();
        playerMoveDetect();
        headForward();
        bodyForward();
        drawSnake();
        checkEaten();
        
       
        wait_vbl_done();
        
    }
}

void growSnake() {
    snakeSize++;
    snake[snakeSize-1].visual = 13;
    snake[snakeSize-1].sprite = snakeSize;
    snake[snakeSize-1].visual = 6;
    snake[snakeSize-1].direction = snake[snakeSize-2].direction;
    snake[snakeSize-1].prevDirection = snake[snakeSize-2].direction;
    snake[snakeSize-1].position[0] =  snake[snakeSize-2].position[0];
    snake[snakeSize-1].position[1] =  snake[snakeSize-2].position[1];
    snake[snakeSize-1].counter = 8;
    set_sprite_tile(snakeSize, snake[snakeSize-1].visual);
    move_sprite(snakeSize, snake[snakeSize-1].position[0], snake[snakeSize-1].position[1]);
}

void checkEaten() {
    if (snake[0].position[0]+8 >= pelletX && snake[0].position[0] <= pelletX+8 && snake[0].position[1]+8 >= pelletY && snake[0].position[1] <= pelletY+8) {
        growSnake();
        pelletX = 255;
        pelletY = 255;
        //spawnPellet();
    }
}

void spawnPellet() {
        pelletX = 8 + (rand() % (153));
        pelletY = 16 + (rand() % (137));
        /* for(int i = 0; i < snakeSize; i++) {
            if ((snake[i].position[0] > pelletX && snake[i].position[0] + 8 < pelletX ) || (snake[i].position[1] > pelletY && snake[i].position[0] + 8 < pelletY)) {
                valid = false;
                break;
            }
        }*/
        valid = true;   
   move_sprite(0, pelletX, pelletY);

}

void drawSnake() {
    set_sprite_tile(snake[0].sprite, snake[0].visual);
    for(int i = 0; i < snakeSize; i++) {
        move_sprite(snake[i].sprite, snake[i].position[0], snake[i].position[1]);
    }
}

void headForward() {
    
    
    switch(snake[0].direction) {
        case 0:
            snake[0].position[0]++;
            snake[0].visual = 2;
        break;
        case 1:
            snake[0].position[1]--;
            snake[0].visual = 1;
        break;
        case 2:
            
            snake[0].position[0]--;
            snake[0].visual = 4;
        break;
        case 3:

            snake[0].position[1]++;
            snake[0].visual = 3;
        break;
    }
    if (snake[0].counter > 0) {
        snake[0].counter--;
    }
    
    if (snake[0].position[0] > 160 || snake[0].position[0] < 8 || snake[0].position[1] > 152 || snake[0].position[1] < 16) {
        gameOver();
    }
}

void bodyForward() {
    
    for (int i = 1; i < snakeSize; i++) {
        if (snake[i].counter > 0) {
            snake[i].counter--;
        }
        else {
            snake[i].prevDirection = snake[i].direction;
         
       snake[i].direction = snake[i-1].prevDirection;
       if (snake[i].direction != snake[i].prevDirection) {
            snake[i].counter = 7;
        }
        }
        switch(snake[i].direction) {
                case 0:
                    snake[i].position[0]++;
                    snake[i].visual = 2;
                break;
                case 1:
                    snake[i].position[1]--;
                    snake[i].visual = 1;
                break;
                case 2:
                    snake[i].position[0]--;
                    snake[i].visual = 4;
                break;
                case 3:
                    snake[i].position[1]++;
                    snake[i].visual = 3;
                break;
        }
        
            
    }  
}  



void playerMoveDetect() {
    
    if (snake[0].counter > 0) {
        return;
    }
    snake[0].prevDirection = snake[0].direction;  

    if ((input & J_LEFT) && snake[0].direction != 0) {
        snake[0].direction = 2;
    }
    if ((input & J_RIGHT) && snake[0].direction != 2) {
        snake[0].direction = 0;
    }
    if ((input & J_UP) && snake[0].direction != 3) {
        snake[0].direction = 1;
    }
    if ((input & J_DOWN) && snake[0].direction != 1) {
        snake[0].direction = 3;
    }
    
    if (snake[0].direction != snake[0].prevDirection) {
        snake[0].counter = 8;
    }
    
    
       
}

void gameOver() {
    alive = false;
    printf("uh oh");
}
