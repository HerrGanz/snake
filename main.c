#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "raylib.h"

#define PLAYER_HOR_SPD 100.0f  //Speed
#define CANVAS_WIDTH 820
#define CANVAS_HEIGHT 450
#define BORDER_SIZE 20

#define SNAKE_SIZE 30

#define BODY_WIDTH  CANVAS_WIDTH/SNAKE_SIZE
#define BODY_HEIGHT  CANVAS_HEIGHT/SNAKE_SIZE

// Player feature
//--------------------------------------------------------------
typedef struct Snake {
    Vector2 position;
    int lifetime;
    int dir_x ;
    int dir_y ;
    int length ;
    int head_x ;
    int head_y ;
    float speed;
} Snake;

Snake player = { 0 };


// Snake snake_body [BODY_WIDTH][BODY_HEIGHT];
// Snake snake = {
//                 .dir_x = 1,
//                 .dir_y = 0,
//                 .length = 2,
//                 .head_x = BODY_WIDTH/2,
//                 .head_y = BODY_HEIGHT/2,
//     };


// void NewSnake(){
    
//     for (int i = 0; i < snake.length; i++)
//     {
//         int x = snake.head_x - snake.dir_x * i;
//         int y = snake.head_y - snake.dir_y * i;
//         snake_body[x][y].lifetime = snake.length-i;
//     }
// }

// void Setup(){
//     for (int i = 0; i < BODY_WIDTH; i++)
//     {
//         for (int j = 0; j < BODY_HEIGHT; j++)
//         {
//             snake_body[i][j].lifetime = 0;
//         }
//     }
//     NewSnake();
// }

// void DrawSnakeBody(int i, int j){
//     DrawRectangle(i*SNAKE_SIZE, j*SNAKE_SIZE, SNAKE_SIZE, SNAKE_SIZE, WHITE);
// }

// void DrawSnake(){
//     for (int i = 0; i < BODY_WIDTH; i++)
//     {
//         for (int j = 0; j < BODY_HEIGHT; j++)
//         {
//             if(snake_body[i][j].lifetime > 0){
//                 DrawSnakeBody(i,j);
//             }
//         }
//     }
// }

// void MoveSnake(){
//     for (int i = 0; i < BODY_WIDTH; i++)
//     {
//         for (int j = 0; j < BODY_HEIGHT; j++)
//         {
//             if(snake_body[i][j].lifetime > 0){
//                 snake_body[i][j].lifetime --;
//             }
//         }
//     }
//     // snake.head_x += snake.dir_x;
//     // snake.head_y += snake.dir_y;

//     snake_body[snake.head_x][snake.head_y].lifetime = snake.length;
    
// }

// void Update(){
//     MoveSnake();
// }

// void Draw(){
//     DrawSnake();
    
// }

//--------------------------------------------------------------

typedef struct EnvItem {
    Rectangle rect;
    int blocking;
    Color color;
} EnvItem;

//Setup environment's items
    EnvItem envItems[] = {
        {{ 0, 0, CANVAS_WIDTH, BORDER_SIZE }, 1, GRAY },                            //top side border
        {{ 0, CANVAS_HEIGHT - BORDER_SIZE, CANVAS_WIDTH, BORDER_SIZE }, 1, GRAY },  //bottom side border
        {{ 0, 0, BORDER_SIZE, CANVAS_HEIGHT }, 1, GRAY },                           //left side border
        {{ CANVAS_WIDTH - BORDER_SIZE , 0, BORDER_SIZE, CANVAS_HEIGHT }, 1, GRAY }   //right side border
    };
    int envItemsLength = sizeof(envItems)/sizeof(envItems[0]); // Items number

 const Vector2 ZERO_VEC = {0, 0};

//Function to scale canvas
Rectangle GetCanvasTarget(){
    float sh = (float)GetScreenHeight();
    float sw = (float)GetScreenWidth();
    float scale = fminf(sh/CANVAS_HEIGHT, sw/CANVAS_WIDTH);
    Rectangle rec = {0,0, CANVAS_WIDTH * scale, CANVAS_HEIGHT * scale};
    return rec;
}

//Set and Output_Direction
//--------------------------------------------------------------
void Print_Direction(int key){
    switch(key){
        case KEY_UP: 
            DrawText("UP", 120, 220, 20, DARKGREEN);
            break; 
        case KEY_DOWN: 
            DrawText("DOWN", 120, 220, 20, DARKGREEN);
            break;
        case KEY_RIGHT: 
            DrawText("RIGHT", 120, 220, 20, DARKGREEN);
            break;
        case KEY_LEFT: 
            DrawText("LEFT", 120, 220, 20, DARKGREEN);
            break;
    }
}

int UpdatePlayer(Snake *player, float delta, int key)
{
    if (key == KEY_LEFT) {
        player->position.x -= PLAYER_HOR_SPD*delta;
        // snake.head_x -= PLAYER_HOR_SPD*delta   *0.01f;
    }
    if (key == KEY_RIGHT){
         player->position.x += PLAYER_HOR_SPD*delta;
        // snake.head_x += PLAYER_HOR_SPD*delta     *0.01f;   
    }
    if (key == KEY_UP){
        player->position.y -= PLAYER_HOR_SPD*delta;
        // snake.head_y -= PLAYER_HOR_SPD*delta     *0.01f;
    }
    if (key == KEY_DOWN){
        player->position.y += PLAYER_HOR_SPD*delta;
        // snake.head_y += PLAYER_HOR_SPD*delta     *0.01f;
    }

    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)){ 
        player->position.x -= PLAYER_HOR_SPD*delta;
        key = KEY_LEFT;

    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)){
        player->position.x += PLAYER_HOR_SPD*delta;
        key = KEY_RIGHT;
    }
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)){
        player->position.y -= PLAYER_HOR_SPD*delta;
        key = KEY_UP;
    }
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)){
        player->position.y += PLAYER_HOR_SPD*delta;
        key = KEY_DOWN;
    }
    
return key;
    
}
//--------------------------------------------------------------




int main() 
{   
//   Initialization
//-----------------------------------------------------------------------------------

    //Resizable window
    //-----------------------------------------------------------------------------------
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(CANVAS_WIDTH, CANVAS_HEIGHT, "Snake");
    SetTargetFPS(60); 

    // //Canvas setup
    RenderTexture2D canvas = LoadRenderTexture(CANVAS_WIDTH, CANVAS_HEIGHT);
    Rectangle canvas_field = {0, 0, (float)canvas.texture.width, (float)-canvas.texture.height};
    SetTextureFilter(canvas.texture, TEXTURE_FILTER_POINT);
    //-----------------------------------------------------------------------------------

    int key = 0;
    bool pause = 0;
    bool death = 0;
    int framesCounter = 0;
    player.position = (Vector2){ GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };

    // Setup();
  // Main game loop
    while(!WindowShouldClose()) {
        //UPDATE+
        //----------------------------------------------------------------------
         if (IsKeyPressed(KEY_R))    //Restart
        {
            player.position = (Vector2){ GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
            death = 0;
            key = 0;
            // Setup();
        }

        //Switch pause
        if (IsKeyPressed(KEY_SPACE)) pause = !pause; 

        //Pause and death
        if (!pause && !death){
            float deltaTime = GetFrameTime();
            key = UpdatePlayer(&player, deltaTime, key);
            Print_Direction(key);

            if ((player.position.x  >= ( CANVAS_WIDTH - SNAKE_SIZE/2 - BORDER_SIZE)) || (player.position.x  <= BORDER_SIZE + SNAKE_SIZE/2)) death = 1;
            if ((player.position.y  >= (CANVAS_HEIGHT - SNAKE_SIZE/2 - BORDER_SIZE)) || (player.position.y  <= BORDER_SIZE + SNAKE_SIZE/2)) death = 1;
        

        } else framesCounter++;
        
        //----------------------------------------------------------------------



        //DRAW
        //----------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);
        
            //CANVAS
            //----------------------------------------------------------------------
            BeginTextureMode(canvas);
            ClearBackground(GREEN);

            // Draw();

            Rectangle playerRect = {player.position.x - SNAKE_SIZE/2, player.position.y - SNAKE_SIZE/2, SNAKE_SIZE, SNAKE_SIZE };
            DrawRectangleRec(playerRect, BLUE);
            for (int i = 0; i < envItemsLength; i++) DrawRectangleRec(envItems[i].rect, envItems[i].color);
            
            
            EndTextureMode();
            DrawTexturePro(canvas.texture, canvas_field, GetCanvasTarget(), ZERO_VEC, (float) 0, WHITE);
            //----------------------------------------------------------------------
            
            // On pause, we draw a blinking message
            if (pause && ((framesCounter/30)%2)) DrawText("PAUSED", 350, 210, 30, GRAY);

            if (death) DrawText("      YOU ARE DEAD !!!  =(\n Press R to restart the game", 170, 170, 30, RED);

            // Update();
            Print_Direction(key);
        EndDrawing();
        //--------------------------------------------------------------------------
                    
            
    }
  
    CloseWindow();   
  
    return 0;
}