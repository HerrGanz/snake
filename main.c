#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "raylib.h"


#define CANVAS_WIDTH 800
#define CANVAS_HEIGHT 400
#define SNAKE_SIZE 40

#define FEILD_WIDTH  CANVAS_WIDTH/SNAKE_SIZE
#define FEILD_HEIGHT CANVAS_HEIGHT/SNAKE_SIZE


const Vector2 ZERO_VEC = {0, 0};

// Global declaration
//====================================================================================
typedef struct Snake {
    int direction_x;         //Direction on x scale
    int direction_y;         //Direction on y scale
    int head_x;              //Head coordination on x scale (in array blocks)
    int head_y;              //Head coordination on y scale (in array blocks)
    int length;              //Length of snake
    int lifetime;            //Lifetime of each part of snake - from head to tale
    float speed;             // In seconds till next movement
    float last_step_update;
    bool food;
} Snake;

    int food_x;
    int food_y;
    int key;
    bool pause = 0;
    bool death = 0;
    int ScoreCount = 0;

// Presentation canvas window as array blocks
Snake Feild [FEILD_WIDTH][FEILD_HEIGHT] = {0};

//Snake parametrs
Snake snake = {
    .direction_x = 1,
    .direction_y = 0,
    .head_x = FEILD_WIDTH/2,
    .head_y = FEILD_HEIGHT/2 - 1,
    .length = 2,
    .speed = 0.07f,
    .last_step_update = 0,
    .food = 0,
};
//====================================================================================





// Function declaration
//====================================================================================
void SetupSnake (void);

void CreateNewSnake(void);

void Draw_Snake_Body (int , int);

void Draw_Snake_Head (int, int);

void Draw_Snake (void);

void DrawFeild (void);

void DrawFood (void);

void MoveSnake(void);

void Check_Direction (void);

void Print_Direction (int);

void CheckRecicling (void);

void Spawn_And_Draw_Food(void);

void Check_Eat_Food (void);

void CheckCollision (void);

//====================================================================================



void Draw (void){

//    DrawFeild ();
    Spawn_And_Draw_Food();
    Draw_Snake ();
}
void Update (){
        Check_Direction ();
        MoveSnake ();
        
}


//Function to scale canvas
Rectangle GetCanvasTarget(){
    float sh = (float)GetScreenHeight();
    float sw = (float)GetScreenWidth();
    float scale = fminf(sh/CANVAS_HEIGHT, sw/CANVAS_WIDTH);
    Rectangle rec = {0,0, CANVAS_WIDTH * scale, CANVAS_HEIGHT * scale};
    return rec;
}

int main(void){
    //Resizable window
    //-----------------------------------------------------------------------------------
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(CANVAS_WIDTH, CANVAS_HEIGHT, "Snake");
        SetTargetFPS(60); 

        //Canvas setup
        RenderTexture2D canvas = LoadRenderTexture(CANVAS_WIDTH, CANVAS_HEIGHT);
        Rectangle canvas_field = {0, 0, (float)canvas.texture.width, -(float)canvas.texture.height};
        SetTextureFilter(canvas.texture, TEXTURE_FILTER_POINT);
    //-----------------------------------------------------------------------------------
    
    
    // Declarations
    //-----------------------------------------------------------------------------------
    srand(time(NULL));
    int framesCounter = 0;
    
    //-----------------------------------------------------------------------------------
     
    SetupSnake ();

    //Main loop
    //====================================================================================
    while (!WindowShouldClose()){
        // Update
        // -------------------------------------------------------------------------------
         if (IsKeyPressed(KEY_R))    //Restart
        {
            death = 0;
            SetupSnake ();
        }
        
        if (IsKeyPressed(KEY_SPACE) && !death) pause = !pause; 

        //Pause and death
        if (!pause && !death){
            Update(); 
        } else framesCounter++;

        // -------------------------------------------------------------------------------



        // Draw
        // -------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);
            //CANVAS
            //----------------------------------------------------------------------
            BeginTextureMode(canvas);
            ClearBackground(GREEN);

                Draw();

            EndTextureMode();
            DrawTexturePro(canvas.texture, canvas_field, GetCanvasTarget(), ZERO_VEC, (float) 0, WHITE);
            //----------------------------------------------------------------------
        
        
        // On pause, we draw a blinking message
        if (pause && ((framesCounter/30)%2)) DrawText("PAUSED", CANVAS_WIDTH/2 - 60, CANVAS_HEIGHT/2 - 20,  30, GRAY);

        if (death) {
            DrawText(TextFormat("SCORE: %i", ScoreCount), 280, 50, 40, MAROON);
            DrawText("      YOU ARE DEAD !!!  =(\n Press R to restart the game", CANVAS_WIDTH/2 - 220, CANVAS_HEIGHT/2 - 60, 30, RED);
        }
        
        Print_Direction(key);
        EndDrawing();
        // -------------------------------------------------------------------------------
    
     
        
    }   
    //====================================================================================

    CloseWindow();

    return 0;
}





//FUNCTIONS
//====================================================================================



void DrawFeild (void){
     for (int i = 0; i < FEILD_WIDTH; i++)
    {
        for (int j = 0; j < FEILD_WIDTH; j++){

            DrawRectangle (i * SNAKE_SIZE, j * SNAKE_SIZE, SNAKE_SIZE, SNAKE_SIZE, RED); 
            DrawRectangle (i * SNAKE_SIZE +1, j * SNAKE_SIZE + 1, SNAKE_SIZE -2 , SNAKE_SIZE - 2, GREEN); 
           
        }
    }

}


void Draw_Snake_Body (int i, int j){
    DrawRectangle (i * SNAKE_SIZE, j * SNAKE_SIZE, SNAKE_SIZE, SNAKE_SIZE, BLUE); 
}

void Draw_Snake_Head (int i, int j){
    DrawRectangle (i * SNAKE_SIZE, j * SNAKE_SIZE, SNAKE_SIZE, SNAKE_SIZE, RED); 
}


void Draw_Snake (void){
    for (int i = 0; i < FEILD_WIDTH; i++)
    {
        for (int j = 0; j < FEILD_WIDTH; j++){

            if (Feild[i][j].lifetime == snake.length) Draw_Snake_Head (i, j);
            else if (Feild[i][j].lifetime > 0) Draw_Snake_Body(i, j);
        }
    }
}



void SetupSnake (void){
    for (int i = 0; i < FEILD_WIDTH; i++)
    {
        for (int j = 0; j < FEILD_WIDTH; j++){

            Feild[i][j].lifetime = 0;
        }
    }
    CreateNewSnake();
}

void CreateNewSnake(void){
    
    snake.direction_x = 1;
    snake.direction_y = 0;
    snake .head_x = FEILD_WIDTH/2;
    snake.head_y = FEILD_HEIGHT/2 - 1;
    snake.length = 2;
    snake.last_step_update = 0;
    snake.food = 0;

    for (int i = 0; i < snake.length; i++)
    {
        int x = snake.head_x - snake.direction_x * i;
        int y = snake.head_y - snake.direction_y * i;
        Feild[x][y].lifetime = snake.length-i;
    }
}

void MoveSnake(void){
    snake.last_step_update += GetFrameTime();
    if (snake.last_step_update >= snake.speed)
        snake.last_step_update -= snake.speed;
    else {return;}

    for (int i = 0; i < FEILD_WIDTH; i++)
    {
        for (int j = 0; j < FEILD_HEIGHT; j++)
        {
            if(Feild[i][j].lifetime > 0){
                Feild[i][j].lifetime --;
            }
        }
    }
    Check_Direction();

    snake.head_x += snake.direction_x;
    snake.head_y += snake.direction_y;
    
    CheckRecicling();
    CheckCollision();
    
    Feild[snake.head_x][snake.head_y].lifetime = snake.length;
}

void CheckRecicling (void){
    if (snake.head_x > FEILD_WIDTH-1) snake.head_x = 0;
    if (snake.head_x < 0) snake.head_x = FEILD_WIDTH-1; 
    if (snake.head_y > FEILD_HEIGHT-1) snake.head_y = 0;
    if (snake.head_y < 0) snake.head_y = FEILD_HEIGHT-1; 
}


void Check_Direction (void){
    if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) && snake.direction_x != 1) {
        snake.direction_x = -1; 
        snake.direction_y = 0;
        key = KEY_LEFT;
    }
    if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) && snake.direction_x != -1) {
        snake.direction_x = 1; 
        snake.direction_y = 0;
        key = KEY_RIGHT;
    }
    if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && snake.direction_y != 1) {
        snake.direction_x = 0; 
        snake.direction_y = -1;
        key = KEY_UP;
    }
    if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && snake.direction_y != -1) {
        snake.direction_x = 0; 
        snake.direction_y = 1;
        key = KEY_DOWN;   
    }
}

void Print_Direction(int key){
    switch(key){
        case KEY_UP: 
            DrawText("UP", 60, CANVAS_HEIGHT/2 - 20, 20, DARKGREEN);
            break; 
        case KEY_DOWN: 
            DrawText("DOWN", 60, CANVAS_HEIGHT/2 - 20, 20, DARKGREEN);
            break;
        case KEY_RIGHT: 
            DrawText("RIGHT", 60, CANVAS_HEIGHT/2 - 20, 20, DARKGREEN);
            break;
        case KEY_LEFT: 
            DrawText("LEFT", 60, CANVAS_HEIGHT/2 - 20, 20, DARKGREEN);
            break;
    }
}

void Spawn_And_Draw_Food(void){

    Check_Eat_Food();
    
    if (snake.food == 0){
        do{
            food_x = rand() % FEILD_WIDTH;
            food_y = rand() % FEILD_HEIGHT;
        } while (Feild[food_x][food_y].lifetime > 0 );
        snake.food = 1;
    }

    DrawRectangle (food_x * SNAKE_SIZE + SNAKE_SIZE/4, food_y * SNAKE_SIZE + SNAKE_SIZE/4, SNAKE_SIZE/2, SNAKE_SIZE/2, MAGENTA);     
    
}

void Check_Eat_Food (void){
    if (food_x == snake.head_x && food_y == snake.head_y) {
        snake.food = 0;
        snake.length ++;
        ScoreCount +=10;
    }
}

void CheckCollision (void){
    if(Feild[snake.head_x][snake.head_y].lifetime > 0 ) 
        death = 1;
}