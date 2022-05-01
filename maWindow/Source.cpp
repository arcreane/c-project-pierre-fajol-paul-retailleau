#include "raylib.h"

#define PERSO_SIZE 20
#define FLOOR 650
#define JUMP_SIZE 200
#define NB_MAX_OBSTACLE 100

typedef struct Perso {
    Vector2 position;
    int radius;
    Color color;
} Perso;

typedef struct Obstacle {
    Rectangle rec;
    Color color;
    bool active;
} Obstacle;


static const int screenWidth = 1600;
static const int screenHeight = 900;


static bool gameOver = false;
static bool pause = false;
static int score = 0;
static int hiScore = 0;

//zone de texte
char text[10] = "\0";
int letterCount = 0;
static Rectangle textBox = { screenWidth / 2.0f - 100, 180, 225, 50 };

//floor est une ligne qui passe par le milieu du perso
static int VISUAL_FLOOR = FLOOR + PERSO_SIZE;

//initialisation des variables
static Perso perso = { 0 };
static Vector2 obsPosition[NB_MAX_OBSTACLE] = { 0 };
static Obstacle obstacle[NB_MAX_OBSTACLE] = { 0 };
static int speedX = 0;
static bool saut = false;

//signature des procédures
static void InitGame(void);         // Initialize game
static void UpdateGame(void);       // Update game (one frame)
static void DrawGame(void);         // Draw game (one frame)
static void UnloadGame(void);       // Unload game
static void UpdateDrawFrame(void);    // Update and Draw (one frame)

int main(void) {

    InitWindow(screenWidth, screenHeight, "Pierre & Paul présentent : Word Runner");

    InitGame();

    SetTargetFPS(60);

    while (!WindowShouldClose()) {   // Detect window close button or ESC key
        UpdateDrawFrame();
    }

    UnloadGame();         // Unload loaded data (textures, sounds, models...)

    CloseWindow();        // Close window and OpenGL context

    return 0;
}


void InitGame(void) {
    //initialisation du perso (taille, position)
    perso.radius = PERSO_SIZE;
    perso.position = (Vector2){ 100, FLOOR };    //screenHeight/2 - perso.radius};

    speedX = 2;

    score = 0;
    gameOver = false;
    pause = false;

    //initialisation des position des obstacle (top left corner)
    obsPosition[0].x = screenWidth / 2;
    for (int i = 1; i < NB_MAX_OBSTACLE; i++) {
        obsPosition[i].x = obsPosition[i - 1].x + GetRandomValue(400, 1000);
        obsPosition[i].y = GetRandomValue(VISUAL_FLOOR - 20, VISUAL_FLOOR - JUMP_SIZE / 2);
    }

    //initialisation des obstacles
    for (int i = 1; i < NB_MAX_OBSTACLE; i++) {
        obstacle[i].rec.x = obsPosition[i].x;         //coordonnées du coin haut gauche
        obstacle[i].rec.y = obsPosition[i].y;
        obstacle[i].rec.width = 40;
        obstacle[i].rec.height = VISUAL_FLOOR - obsPosition[i].y;
        obstacle[i].active = true;
    }


}


void UpdateDrawFrame(void) {
    UpdateGame();
    DrawGame();
}


void UpdateGame(void) {
    if (!gameOver) {
        if (IsKeyPressed(KEY_SPACE)) pause = !pause;

        if (!pause) {

            if ((GetCharPressed() >= KEY_A) && (GetCharPressed() <= KEY_Z) && !gameOver && perso.position.y >= FLOOR) {
                saut = !saut;

                int key = GetCharPressed();
                //controle du texte
                text[letterCount] = (char)key;
                text[letterCount + 1] = '\0'; // Add null terminator at the end of the string.
                letterCount++;
            }

            //controle du perso
            if (saut) {
                if (perso.position.y > FLOOR - JUMP_SIZE) {
                    perso.position.y -= (2 + 1.2 * speedX);
                }
                else saut = false;
            }
            else {
                if (perso.position.y < FLOOR) {
                    perso.position.y += (2 + 1.2 * speedX);
                }
            }
            if (perso.position.y >= FLOOR) perso.position.y = FLOOR;

            if (IsKeyPressed(KEY_BACKSPACE)) {
                letterCount--;
                if (letterCount < 0) letterCount = 0;
                text[letterCount] = '\0';
            }


            //controle des obstacles
                        //déplacement
            for (int i = 0; i < NB_MAX_OBSTACLE; i++) obsPosition[i].x -= speedX;
            for (int i = 0; i < NB_MAX_OBSTACLE; i++) obstacle[i].rec.x = obsPosition[i].x;

            //collision et points
            for (int i = 0; i < NB_MAX_OBSTACLE; i++) {
                if (CheckCollisionCircleRec(perso.position, perso.radius, obstacle[i].rec)) {
                    gameOver = true;
                    pause = false;
                }
                else if (!gameOver && obstacle[i].active && obstacle[i].rec.x < perso.position.x) {
                    score += 10;
                    speedX += 1;
                    obstacle[i].active = false;

                    if (score > hiScore) hiScore = score;
                }
            }

        }
    }
    else {
        if (IsKeyPressed(KEY_ENTER)) {
            InitGame();
            gameOver = false;
        }
    }
}

void DrawGame(void) {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    if (!gameOver) {
        //dessin du tuxt en haut à gauche
        DrawText(TextFormat("%04i", score), 20, 20, 40, GRAY);
        DrawText(TextFormat("HIGH SCORE: %04i", hiScore), 20, 70, 20, LIGHTGRAY);

        //dessin du perso et du sol
        DrawCircle(perso.position.x, perso.position.y, perso.radius, DARKGRAY);        //perso
        DrawRectangle(0, VISUAL_FLOOR, screenWidth, screenHeight - FLOOR, DARKGRAY);//sol

        //dessin des obstacles
        for (int i = 0; i < NB_MAX_OBSTACLE; i++) {
            DrawRectangle(obstacle[i].rec.x, obstacle[i].rec.y, obstacle[i].rec.width, obstacle[i].rec.height, BLACK);
        }

        //dessin du text
        DrawRectangleRec(textBox, LIGHTGRAY);
        DrawText(text, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);

        if (pause) DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, GRAY);
    }
    else DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);

    EndDrawing();
}











// Unload game variables
void UnloadGame(void)
{
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
}

