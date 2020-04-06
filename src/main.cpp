#include <SDL.h>

#include <string>
#include <ctime>
#include <vector>

#include "window.h"
#include "texture.h"
#include "action.h"

int Vel = 13;
int Vel_x = Vel;
int Vel_y = 0;

int Cor_x = 0;
int Cor_y = 0;


std::vector<SDL_Rect> snake;
/// [0 - cabeza, n - cola]

void reset(int SCREEN_WIDTH, int SCREEN_HEIGHT){
    snake.clear();

    for(int i=0;i<5;i++){
        snake.push_back({0, 0});
    }

    Vel_x = Vel;
    Vel_y =   0;

    snake[0].x = SCREEN_WIDTH/2;
    snake[0].y = SCREEN_HEIGHT/2;
    snake[0].h = 10;
    snake[0].w = 10;
    for(int i=1;i<snake.size();i++){
        snake[i].x = snake[i-1].x + Vel;
        snake[i].y = SCREEN_HEIGHT/2;
        snake[i].h = 10;
        snake[i].w = 10;
    }

}


bool Verificar_collision(SDL_Rect A, SDL_Rect A){
    /// PARA A
    int A_IZQ = A.x;
    int A_DER = A.x + A.w;
    int A_ARR = A.y;
    int A_ABJ = A.y + A.h;
    /// PARA B
    int B_IZQ = B.x;
    int B_DER = B.x + B.w;
    int B_ARR = B.y;
    int B_ABJ = B.y + B.h;

    if(
        (A_ABJ>=B_ARR) &&
        (A_ARR<=B_ABJ) &&
        (A_DER>=B_IZQ) &&
        (A_IZQ<=B_DER)
    ){
        return true;
    }
    return false;
}

bool Colision(int Dim, int SCREEN_WIDTH, int SCREEN_HEIGHT){

    if(snake[snake.size()-1].x>(SCREEN_WIDTH-Dim)){
        return true;
    }
    if(snake[snake.size()-1].x<0){
        return true;
    }
    if(snake[snake.size()-1].y>(SCREEN_HEIGHT-Dim)){
        return true;
    }
    if(snake[snake.size()-1].y<0){
        return true;
    }
    for(int i=1;i<snake.size()-2;i++){
        if(Verificar_collision(snake[snake.size()-1], {snake[i].x, snake[i].y, Dim, Dim}) == true){
            reset(SCREEN_WIDTH, SCREEN_HEIGHT);
        }
    }
    if(Verificar_collision(snake[snake.size()-1], {Cor_x, Cor_y, Dim, Dim}) == true){
        Cor_x = 10 + rand () % (SCREEN_WIDTH-(4*Dim));
        Cor_y = 10 + rand () % (SCREEN_HEIGHT-(4*Dim));
        snake.push_back({0, 0});

        for(int i = snake.size() - 2; i>=0; i--){
            snake[i+1].x = snake[i].x;
            snake[i+1].y = snake[i].y;
        }
        snake[0].x = snake[1].x;
        snake[0].y = snake[1].y;
    }
    return false;
}

int main( int argc, char* args[]){
    srand(time(0));
    int SCREEN_WIDTH  = 800;
    int SCREEN_HEIGHT = 600;
    int FONT_SIZE     =  20;

    int MOVE_DELAY =  0;
    int Dimension  = 10;

    bool exit = false;
  
    std::string PATH(SDL_GetBasePath());
    std::string PATH_FONT = PATH + "asset/font/LiberationMono-Regular.ttf";
    std::string PATH_ICON = PATH + "asset/icon.bmp";
    std::string GAME_NAME = "SNAKE";
    
    SDL_Color COLOR_BLACK = {0x00, 0x00, 0x00, 0xFF};
    SDL_Color COLOR_RED   = {0xFF, 0x00, 0x00, 0xFF};
    SDL_Color COLOR_GREEN = {0x00, 0xFF, 0x00, 0xFF};
    SDL_Color COLOR_BLUE  = {0x00, 0x00, 0xFF, 0xFF};
    SDL_Color COLOR_WHITE = {0xFF, 0xFF, 0xFF, 0xFF};

    Window window(GAME_NAME, SCREEN_WIDTH, SCREEN_HEIGHT, COLOR_BLACK);
    window.set_icon(PATH_ICON);

    TextureText text_white(
        window.get_render(),
        PATH_FONT,
        COLOR_WHITE,
        FONT_SIZE
    );

    Action* action = Action::get_instance();
    action->init(
        SDL_SCANCODE_Z,
        SDL_SCANCODE_X,
        SDL_SCANCODE_RETURN,
        SDL_SCANCODE_UP,
        SDL_SCANCODE_DOWN,
        SDL_SCANCODE_LEFT,
        SDL_SCANCODE_RIGHT
    );

    Cor_x = rand () % (SCREEN_WIDTH  + 1);
    Cor_y = rand () % (SCREEN_HEIGHT + 1);

    reset(SCREEN_WIDTH, SCREEN_HEIGHT);

    while(!exit){

        if(window.check_exit()){
            exit = true;
        }else{
            window.clear_screen();


            if(MOVE_DELAY != 0){
                MOVE_DELAY--;
            }

            if(action->get_state(action->BUTTON_MOVE_UP)){
                if(
                    (Vel_y != +Vel) &&
                    (MOVE_DELAY == 0)
                ){
                    Vel_x =    0;
                    Vel_y = -Vel;
                }
            }else if(action->get_state(action->BUTTON_MOVE_DOWN)){
                if(
                    (Vel_y != -Vel) &&
                    (MOVE_DELAY == 0)
                ){
                    Vel_x =    0;
                    Vel_y = +Vel;
                }
            }else if(action->get_state(action->BUTTON_MOVE_LEFT)){
                if(
                    (Vel_x != +Vel) &&
                    (MOVE_DELAY == 0)
                ){
                    Vel_x = -Vel;
                    Vel_y =    0;
                }
            }else if(action->get_state(action->BUTTON_MOVE_RIGHT)){
                if(
                    (Vel_x != -Vel) &&
                    (MOVE_DELAY == 0)
                ){
                    Vel_x = +Vel;
                    Vel_y =    0;
                }
            }

            if(MOVE_DELAY==0){
                for(int i=0;i<snake.size()-1;i++){
                    snake[i].x = snake[i+1].x;
                    snake[i].y = snake[i+1].y;
                }
                snake[snake.size()-1].x = snake[snake.size()-1].x + Vel_x;
                snake[snake.size()-1].y = snake[snake.size()-1].y + Vel_y;
                MOVE_DELAY = 6;
            }else if(MOVE_DELAY != 0){
                MOVE_DELAY--;
            }
            /// //////////////////////

            if(Colision(Dimension, SCREEN_WIDTH, SCREEN_HEIGHT) == true){
                reset(SCREEN_WIDTH, SCREEN_HEIGHT);
                MOVE_DELAY = 0;
            }

            // Draw
            for(int i=0; i < snake.size();i++){
                window.draw_rectangle(
                    snake[i],
                    {0xFF,0xFF,0xFF,0xFF}
                );
            }
            window.draw_rectangle(
                {
                    Cor_x,
                    Cor_y,
                    Dimension,
                    Dimension
                },
                {0xFF,0xFF,0xFF,0xFF}
            );

            printf("%i, %i\n", Cor_x, Cor_y);
            printf("%i\n", MOVE_DELAY);
            printf("%i, %i\n", Vel_x, Vel_y);

            window.update_screen();
        }


    }
    return 0;
}
