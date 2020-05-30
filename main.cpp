#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

using namespace std;

//GAME VARIABLES
int PIECE_LENGTH = 20;
int BODY_COUNT = 0;
int MOVEMENT_SPEED = 10;
int GAME_SPEED = 100;
bool THERE_IS_BAIT_UP = false;
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;
bool IS_RUNNING = true;
enum DIRECTIONS {UP, DOWN, LEFT, RIGHT};
DIRECTIONS SNAKE_MOVE_DIRECTION = RIGHT;
vector<SDL_Rect> snake;
SDL_Rect bait;

//SDL related stuff
SDL_Event event;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

SDL_Rect create_snake_piece(){
    SDL_Rect snake_piece;
    snake_piece.w = PIECE_LENGTH;
    snake_piece.h = PIECE_LENGTH;

    if(BODY_COUNT){
        switch (SNAKE_MOVE_DIRECTION) {
    case UP:
        snake_piece.x = snake[snake.size()-1].x;
        snake_piece.y = snake[snake.size()-1].y - PIECE_LENGTH;
        break;
    case DOWN:
        snake_piece.x = snake[snake.size()-1].x;
        snake_piece.y = snake[snake.size()-1].y + PIECE_LENGTH;
        break;
    case LEFT:
        snake_piece.y = snake[snake.size()-1].y;
        snake_piece.x = snake[snake.size()-1].x - PIECE_LENGTH;
        break;
    case RIGHT:
        snake_piece.x = snake[snake.size()-1].x + PIECE_LENGTH;
        snake_piece.y = snake[snake.size()-1].y;
        break;
    }
    } else {
        snake_piece.x = 10*PIECE_LENGTH;
        snake_piece.y = 10*PIECE_LENGTH;
    }

    BODY_COUNT += 1;
    return snake_piece;
}
void initialize(){
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow (
        "Snake Game", 
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
    
    //AND GOD SAID
    snake.push_back(create_snake_piece());
    snake.push_back(create_snake_piece());
    snake.push_back(create_snake_piece());
    snake.push_back(create_snake_piece());
    
}
void create_bait(){
    
    if(!THERE_IS_BAIT_UP){
        bool is_bait_safe = true;
        bait.x = (rand() % ((SCREEN_WIDTH/PIECE_LENGTH)));
        bait.y = (rand() % ((SCREEN_HEIGHT/PIECE_LENGTH)));
        for(auto &piece:snake){
            if(piece.x == bait.x && piece.y == bait.y){
                is_bait_safe = false;
            }
        }
        if(!is_bait_safe){
            create_bait();
        } else {
            bait.x *= PIECE_LENGTH;
            bait.y *= PIECE_LENGTH;
            bait.w = bait.h =  PIECE_LENGTH;
            THERE_IS_BAIT_UP = true;
        }
    }
}
void move_snake(){
     snake.push_back(create_snake_piece());
}
void render_screen(){
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
    SDL_RenderFillRect(renderer, &bait);
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
    SDL_RenderFillRects(renderer, &snake[0], snake.size());
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderDrawRects(renderer, &snake[0], snake.size());
    SDL_RenderPresent(renderer);
}
void dispose(){
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void increase_snake_body(){
    snake.push_back(create_snake_piece());
}
void check_snake_bait_collision(){
    if(snake[snake.size() -1 ].x == bait.x && snake[snake.size() -1 ].y == bait.y){
        THERE_IS_BAIT_UP= false;
        increase_snake_body();
    }
}
void erase_tail(){
    BODY_COUNT-=1;
    snake.erase(snake.begin());
}
bool check_wall_collision(){

    if(snake[snake.size()-1].x > SCREEN_WIDTH - PIECE_LENGTH){
        return false;
    }
    if(snake[snake.size()-1].x < 0){
        return false;
    }
    if(snake[snake.size()-1].y > SCREEN_HEIGHT - PIECE_LENGTH){
        return false;
    }
    if(snake[snake.size()-1].y < 0){
        return false;
    }
    return true;
}
void game_loop(){
    while(IS_RUNNING){
        SDL_Delay(GAME_SPEED);
        while(SDL_PollEvent(&event)){

            if(event.type == SDL_QUIT){
                IS_RUNNING = false;
            }

            if(event.window.event == SDL_WINDOWEVENT_RESIZED){
                    SDL_GetRendererOutputSize(renderer, &SCREEN_WIDTH, &SCREEN_HEIGHT);
            }
           
           if(event.type == SDL_KEYDOWN){
               SDL_Scancode key = event.key.keysym.scancode;
               if(key == SDL_SCANCODE_W){SNAKE_MOVE_DIRECTION = UP;}
               if(key == SDL_SCANCODE_S){SNAKE_MOVE_DIRECTION = DOWN;}
               if(key == SDL_SCANCODE_A){SNAKE_MOVE_DIRECTION = LEFT;}
               if(key == SDL_SCANCODE_D){SNAKE_MOVE_DIRECTION = RIGHT;}
               if(key == SDL_SCANCODE_SPACE){increase_snake_body();}
               if(key == SDL_SCANCODE_BACKSPACE){erase_tail();}
           }
        }

        create_bait();
        move_snake();
        erase_tail();
        check_snake_bait_collision();
        
        render_screen(); 
        if(!check_wall_collision()){break;}
    }
}

int main (int argc, char** argv) {

    initialize();
    game_loop();
    dispose();
   
    return EXIT_SUCCESS;
}