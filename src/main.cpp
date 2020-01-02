#include <SDL2/SDL.h>

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


std::vector<SDL_Point> snake;
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
    for(int i=1;i<snake.size();i++){
        snake[i].x = snake[i-1].x + Vel;
        snake[i].y = SCREEN_HEIGHT/2;
    }

}


bool Verificar_collision(int A_x,int A_y,int B_x,int B_y,int Dim){
        int A_IZQ,A_DER,A_ARR,A_ABJ;
        int B_IZQ,B_DER,B_ARR,B_ABJ;

        /// PARA A
        A_IZQ = A_x;
        A_DER = A_x + Dim;
        A_ARR = A_y;
        A_ABJ = A_y + Dim;
        /// PARA B
        B_IZQ = B_x;
        B_DER = B_x + Dim;
        B_ARR = B_y;
        B_ABJ = B_y + Dim;
        /// Restriccones
        if(A_ABJ<=B_ARR){
            return false;
        }
        if(A_ARR>=B_ABJ){
            return false;
        }
        if(A_DER<=B_IZQ){
            return false;
        }
        if(A_IZQ>=B_DER){
            return false;
        }
    return true;
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
        if(Verificar_collision(snake[snake.size()-1].x, snake[snake.size()-1].y, snake[i].x, snake[i].y, Dim) == true){
            reset(SCREEN_WIDTH, SCREEN_HEIGHT);
        }
    }
    if(Verificar_collision(snake[snake.size()-1].x, snake[snake.size()-1].y, Cor_x, Cor_y, Dim) == true){
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

int main( int argc, char* args[] ){
    int SCREEN_WIDTH  = 800;
    int SCREEN_HEIGHT = 600;
    int FONT_SIZE     =  20;

    int MOVE_DELAY    =   0;
    int Dimension = 10;

    std::string PATH_FONT = "asset/font/LiberationMono-Regular.ttf";
    std::string PATH_ICON = "asset/icon.bmp";
    std::string GAME_NAME = "SNAKE";

    bool exit = false;
    
    SDL_Color COLOR_BLACK = {0x00, 0x00, 0x00, 0xFF};
    SDL_Color COLOR_RED   = {0xFF, 0x00, 0x00, 0xFF};
    SDL_Color COLOR_GREEN = {0x00, 0xFF, 0x00, 0xFF};
    SDL_Color COLOR_BLUE  = {0x00, 0x00, 0xFF, 0xFF};
    SDL_Color COLOR_WHITE = {0xFF, 0xFF, 0xFF, 0xFF};

    Window window(GAME_NAME, SCREEN_WIDTH, SCREEN_HEIGHT, COLOR_BLACK);
    window.set_icon(PATH_ICON);

    TextureText text_white(window.get_render(), PATH_FONT, COLOR_WHITE, FONT_SIZE);

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
    srand(time(0));
    while(!exit){

        if(window.check_exit()){
            exit = true;
        }else{
            window.clear_screen();
            //SDL_Teclado(&exit);
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
                    {
                        snake[i].x,
                        snake[i].y,
                        Dimension,
                        Dimension
                    },
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
