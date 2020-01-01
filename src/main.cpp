#include <SDL2/SDL.h>

#include <string>
#include <ctime>


#include "window.h"
#include "texture.h"
#include "action.h"

int AnchoP = 800,LargoP = 600;

double Vel = 13;
int tamano = 5;
int **Pos_CBucle,Vel_x=Vel,Vel_y=0;
int cont = 0;
int Cor_x = 0;
int Cor_y = 0;
int Cool_D[2] = {0,0};
/// [0-cabeza , 1 -cola][0-x , 1-y]

void Fill(int x,int y){
    for(int i=0;i<x;i++){
        for(int j=0;j<y;j++){
            Pos_CBucle[i][j]=0;
        }
    }
}

void Array_Size(int old_x,int x,int y = 2){
    int **Temp;
    Temp = new int*[old_x];
    for(int i=0;i<old_x;i++){
        Temp[i] = new int[y];
    }
    for(int i=0;i<old_x;i++){
        for(int j=0;j<y;j++){
            Temp[i][j] = Pos_CBucle[i][j];
        }

    }
    Pos_CBucle = new int*[x];
    for(int i=0;i<x;i++){
        Pos_CBucle[i] = new int[y];
    }
    Fill(x,y);
    if(old_x<x){
        for(int i=0;i<old_x;i++){
            for(int j=0;j<y;j++){
                Pos_CBucle[i+1][j] = Temp[i][j];
            }
        }
    }else{
        for(int i=0;i<x;i++){
            for(int j=0;j<y;j++){
                Pos_CBucle[i+1][j] = Temp[i][j];
            }
        }
    }
}

void Reset(){
    Cool_D[0] = 0;
    Cool_D[1] = 0;
    const int temp = Vel;
    tamano = 5;
    Pos_CBucle = new int*[tamano];
    for(int i=0;i<tamano;i++){
        Pos_CBucle[i] = new int[2];
    }
    Fill(tamano,2);

    Vel_x=Vel,Vel_y=0;
    Pos_CBucle[0][0] = AnchoP/2;
    Pos_CBucle[0][1] = LargoP/2;
    for(int i=1;i<tamano;i++){
        for(int j=0;j<2;j++){
            if(j==0){
                Pos_CBucle[i][j] = Pos_CBucle[i-1][j] + Vel;
            }else{
                Pos_CBucle[i][j] = LargoP/2;;
            }
        }
    }
    Vel = temp;
}

void cargador(){

    Cor_x = rand () % (AnchoP+1);
    Cor_y = rand () % (LargoP+1);
}

void Impresor_C(){
    system("cls");
    for(int i =0;i<tamano;i++){
        for(int j=0;j<2;j++){
            printf("%i ,", Pos_CBucle[i][j]);
        }
        printf("\n");
    }
}

void SDL_Teclado(bool *Salir){
    /**
    SDL_Event Teclado;

    **/
    
}

void SDL_Cuadrado(int x,int y,int h,int w, SDL_Renderer* renderer){
    SDL_Rect Cuadrado;
    Cuadrado.x = x;
    Cuadrado.y = y;
    Cuadrado.h = h;
    Cuadrado.w = w;
    SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
    SDL_RenderFillRect(renderer,&Cuadrado);
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

bool Colision(int Dim,int x, int y ){

    if(Pos_CBucle[tamano-1][x]>(AnchoP-Dim)){
        return true;
    }
    if(Pos_CBucle[tamano-1][x]<0){
        return true;
    }
    if(Pos_CBucle[tamano-1][y]>(LargoP-Dim)){
        return true;
    }
    if(Pos_CBucle[tamano-1][y]<0){
        return true;
    }
    for(int i=1;i<tamano-2;i++){
        if(Verificar_collision(Pos_CBucle[tamano-1][x],Pos_CBucle[tamano-1][y],Pos_CBucle[i][x],Pos_CBucle[i][y],Dim)==true){
            Reset();
            printf("Entró\n");
        }
    }
    if(Verificar_collision(Pos_CBucle[tamano-1][x],Pos_CBucle[tamano-1][y],Cor_x,Cor_y,Dim)==true){
        Cor_x = 10 + rand () % (AnchoP-(4*Dim));
        Cor_y = 10 + rand () % (LargoP-(4*Dim));
        Array_Size(tamano,tamano+1);
        tamano++;
        Pos_CBucle[0][x] = Pos_CBucle[1][x];
        Pos_CBucle[0][y] = Pos_CBucle[1][y];
        //Vel = Vel + (1/2);
    }
    return false;
}

void Impresor_P(Window* window){
    int Dimension = 10;
    int x = 0,y = 1;
    if(Cool_D[0]==0){
        for(int i=0;i<tamano-1;i++){
            for(int j=0;j<2;j++){
                Pos_CBucle[i][j]=Pos_CBucle[i+1][j];
            }
        }
        Pos_CBucle[tamano-1][x] = Pos_CBucle[tamano-1][x] + Vel_x;
        Pos_CBucle[tamano-1][y] = Pos_CBucle[tamano-1][y] + Vel_y;
        Cool_D[0] = 6;
    }else if(Cool_D[0] != 0){
        Cool_D[0]--;
    }
    /// //////////////////////
    if(Colision(Dimension,x,y)==true){
        Reset();
    }
    /// //////////////
    for(int i=0;i<tamano;i++){
        SDL_Cuadrado(Pos_CBucle[i][x],Pos_CBucle[i][y],Dimension,Dimension, window->get_render());
    }

    SDL_Cuadrado(Cor_x,Cor_y,Dimension,Dimension, window->get_render());
}

int main( int argc, char* args[] ){
    int SCREEN_WIDTH  = 800;
    int SCREEN_HEIGHT = 600;
    int FONT_SIZE     =  20;

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

    cargador();
    Reset();
    srand(time(0));
    while(!exit){

        if(window.check_exit()){
            exit = true;
        }else{
            window.clear_screen();
            //SDL_Teclado(&exit);
            if(Cool_D[1]!=0){
                Cool_D[1]--;
            }


            if(action->get_state(action->BUTTON_MOVE_UP)){
                if(
                    (Vel_y != +Vel) &&
                    (Cool_D[1]==0)
                ){
                    Vel_x =    0;
                    Vel_y = -Vel;
                    Cool_D[1] = 0;
                }
            }else if(action->get_state(action->BUTTON_MOVE_DOWN)){
                if((Vel_y != -Vel)&&(Cool_D[1]==0)){
                    Vel_x =    0;
                    Vel_y = +Vel;
                    Cool_D[1] = 0;
                }
            }else if(action->get_state(action->BUTTON_MOVE_LEFT)){
                if(
                    (Vel_x != +Vel) &&
                    (Cool_D[1]==0)
                ){
                    Vel_x = -Vel;
                    Vel_y =    0;
                    Cool_D[1] = 0;
                }
            }else if(action->get_state(action->BUTTON_MOVE_RIGHT)){
                if(
                    (Vel_x != -Vel) &&
                    (Cool_D[1]==0)
                ){
                    Vel_x = +Vel;
                    Vel_y =    0;
                    Cool_D[1] = 0;
                }
            }


            Impresor_P(&window);
            //Impresor_C();

            printf("%i, %i\n", Cor_x, Cor_y);
            printf("%i, %i\n", Cool_D[0], Cool_D[1]);
            printf("%i, %i\n", Vel_x, Vel_y);

            window.update_screen();
        }


    }
    return 0;
}
