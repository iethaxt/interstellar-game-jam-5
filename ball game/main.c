#include "raylib.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    InitAudioDevice();
    
    Font largeFont = LoadFontEx("resources/supercomputer.ttf", 100, 0, 0);
    Font smallFont = LoadFontEx("resources/supercomputer.ttf", 20, 0, 0);
    Texture2D background = LoadTexture("resources/background 1.png");
    Texture2D paddle = LoadTexture("resources/paddle.png");
    Texture2D rainbow = LoadTexture("resources/rainbow.png");
    Sound hitSound = LoadSound("resources/hit.wav");
    Sound bonusSound = LoadSound("resources/bonus.wav");
    Sound resetSound = LoadSound("resources/reset.wav");
    
    Vector2 playerPosition = {-100.0f, -100.0f};
    Vector2 ballPosition = {screenWidth/2, screenHeight/2};
    Vector2 bonusPosition = {rand() % screenWidth, 150 + rand() % (screenHeight-150)};
    Vector2 prevPaddlePosition = {0.f, 0.f};
    
    Vector2 ballVelocity = {0.f, 0.f};
    Vector2 bonusVelocity = {1 + rand()%2, 1 + rand() % 2};
    Vector2 playerVelocity = {0.f, 0.f};
    
    float level = 1;    
    int bonusValue = (1 + rand()%9) * level;
    
    float score = 0;
    float levelScoreCounter = 0;
    int high_score = 0;
    float levelProgress = 0;

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        playerPosition = GetMousePosition();
        playerVelocity.x = playerPosition.x - prevPaddlePosition.x;
        playerVelocity.y = playerPosition.y - prevPaddlePosition.y;
        ballPosition.x += ballVelocity.x;
        ballPosition.y += ballVelocity.y;
        bonusPosition.x += bonusVelocity.x;
        bonusPosition.y += bonusVelocity.y;
        
        
        //ball bouncing
        //ball screen bouncing
        if (ballPosition.x > screenWidth || ballPosition.x < 0){
            score = 0;
            levelScoreCounter = 0;
            level = 1;
            
            background = LoadTexture("resources/background 1.png");
            
            //Teleporting the ball back a little to prevent the ball getting stuck
            ballPosition.x -= ballVelocity.x;
            
            ballVelocity.x = -ballVelocity.x;
            
            PlaySound(resetSound);
        }
        else if(ballPosition.y > screenHeight || ballPosition.y < 150){
            
            if (ballPosition.y > screenHeight){
                score += 1;
                levelScoreCounter += 1;
                PlaySound(hitSound);
            }
            else {
                score = 0;
                levelScoreCounter = 0;
                level = 1;
                background = LoadTexture("resources/background 1.png");
                PlaySound(resetSound);
            }
   
            //Teleporting the ball back a little to prevent the ball getting stuck
            ballPosition.y -= ballVelocity.y;
            
            ballVelocity.y = -ballVelocity.y;
        }
        
        //ball paddle bouncing
        if(CheckCollisionCircles(playerPosition, 40, ballPosition, 20)){
            float x_distance = playerPosition.x - ballPosition.x;
            float y_distance = playerPosition.y - ballPosition.y;
            
            ballVelocity = playerVelocity;            
            ballVelocity.x += -x_distance / 8;
            ballVelocity.y += -y_distance / 8;
            
            PlaySound(hitSound);
        }
        
        
        //bonus bouncing
        if(bonusPosition.x > screenWidth || bonusPosition.x < 0){
            bonusVelocity.x = -bonusVelocity.x;
        }
        if(bonusPosition.y > screenHeight || bonusPosition.y < 150){
            bonusVelocity.y = -bonusVelocity.y;
        }
        if (score > high_score){
            high_score = score;
        }
        
        //bonus collision
        if(CheckCollisionCircles(ballPosition, 20, bonusPosition, 20)){
            score += bonusValue;
            levelScoreCounter += bonusValue;
            bonusVelocity = (Vector2){1 + rand()%2, 1 + rand() % 2};
            bonusPosition = (Vector2){rand() % screenWidth, 150 + rand() % (screenHeight-150)};
            bonusValue = (1 + rand()%9) * level;
            
            PlaySound(bonusSound);
        }
        
        levelProgress = (levelScoreCounter/level)/pow(level+5, 1.5);
        if (levelProgress >= 1){
            level += 1;
            levelScoreCounter = 0;
            
            if (level <= 5){
                background = LoadTexture(TextFormat("resources/background %d%s", (int)level, ".png"));
            }
        }
        
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawTexture(background, 0, 0, WHITE);
            
            DrawRectangle(0, 120, screenWidth, 30, LIGHTGRAY);
            DrawTexturePro(rainbow, (Rectangle){ 0.0f, 0.0f, 1.f, 30.f }, (Rectangle){0, 120, screenWidth*levelProgress, 30}, (Vector2){0,0}, 0,WHITE);
            DrawTextEx(smallFont, TextFormat("Level: %0.f", level), (Vector2){(screenWidth/2)-MeasureText(TextFormat("Level: %d", level), 20)/2, 125}, 20, 2, BLACK);
            
            DrawTextEx(largeFont, TextFormat("%0.f", score), (Vector2){(screenWidth - MeasureText(TextFormat("%0.f", score), 100)) / 2, 0}, 100, 2, BLACK);
            DrawTextEx(smallFont, TextFormat("High Score: %d", high_score), (Vector2){(screenWidth - MeasureText(TextFormat("High Score: %d", high_score), 20)) / 2, 100}, 20, 2, BLACK);
            
            DrawTexture(paddle, playerPosition.x-40, playerPosition.y-40, WHITE);
            DrawCircleV(ballPosition, 20, WHITE);
            
            DrawCircleV(bonusPosition, 20, BLACK);
            DrawTextEx(smallFont, TextFormat("+%d", bonusValue), (Vector2){bonusPosition.x-MeasureText(TextFormat("+%d", bonusValue), 20)/2, bonusPosition.y-10}, 20, 2, WHITE);
        EndDrawing();
        
        prevPaddlePosition = playerPosition;
    }
    
    UnloadFont(largeFont);
    UnloadFont(smallFont);
    UnloadTexture(background);
    UnloadTexture(paddle);
    UnloadTexture(rainbow);
    UnloadSound(hitSound);
    UnloadSound(bonusSound);
    UnloadSound(resetSound);

    CloseWindow();

    return 0;
}