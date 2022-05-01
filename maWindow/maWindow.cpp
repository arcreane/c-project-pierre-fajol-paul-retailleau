#include "raylib.h"
#include "Item.h"
#include "Bonus.h"
#include "Malus.h"
#include "Personnage.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <queue>

using namespace std;

#define PERSO_SIZE 20
#define FLOOR 650
#define JUMP_SIZE 200
#define NB_MAX_OBSTACLE 100

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

//floor est une ligne qui passe par le milieu du perso
static int VISUAL_FLOOR = FLOOR + PERSO_SIZE;

//initialisation des variables
static Vector2 posPersonage = { 100, FLOOR };
static Personnage perso(posPersonage, PERSO_SIZE);
static Vector2 obsPosition[NB_MAX_OBSTACLE] = { 0 };
static Obstacle obstacle[NB_MAX_OBSTACLE] = { 0 };
static int speedX = 0;
static bool saut = false;

//File qui stocke les bonus/malus
static deque <Item> listeItem; //push_back pop_front

//----------Boite de texet---------
char name[10] = "\0";      // NOTE: One extra space required for null terminator char '\0'
int letterCount = 0;       //compteur de lettre pour la saisie
Rectangle textBox = { GetScreenWidth() / 2.0f - 100, 180, 225, 50 };
bool mouseOnText = false;       //boolen pour detecter la souris sur le texte
int framesCounter = 0;          // ???
//--------------------------------

//signature des procédures
static void InitGame(void);         // Initialize game
static void UpdateGame(void);       // Update game (one frame)
static void DrawGame(void);         // Draw game (one frame)
static void UnloadGame(void);       // Unload game
static void UpdateDrawFrame(void);	// Update and Draw (one frame)

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
    //test
    

    //initialisation du perso (taille, position)
   /* perso.radius = PERSO_SIZE;
    perso.position.x = 100;
    perso.position.y = FLOOR;*/
    //perso.position = (Vector2){ 100, FLOOR };	//screenHeight/2 - perso.radius};

    textBox = { screenWidth / 2.0f - 100, 180, 225, 50 };

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
        obstacle[i].rec.x = obsPosition[i].x; 		//coordonnées du coin haut gauche
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

            int key = GetCharPressed();
            if ((key >= 32) && (key <= 125) && !gameOver && perso.getYposition() >= FLOOR) {
                saut = !saut; 
            }

            //controle du perso
            if (saut) {
                if (perso.getYposition() > FLOOR - JUMP_SIZE) {
                    perso.setYposition(perso.getYposition() - (2 + 1.2 * speedX));
                }
                else saut = false;
            }
            else {
                if (perso.getYposition() < FLOOR) {
                    perso.setYposition(perso.getYposition() + (2 + 1.2 * speedX));
                }
            }
            if (perso.getYposition() >= FLOOR) perso.setYposition(FLOOR);

            

            
            while (key > 0)
            {
                if ((key >= 32) && (key <= 125) && (letterCount < 10))
                {
                    name[letterCount] = (char)key;
                    name[letterCount + 1] = '\0'; // Add null terminator at the end of the string.
                    letterCount++;
                }

                key = GetCharPressed();  // Check next character in the queue

            }
            if (IsKeyPressed(KEY_BACKSPACE)) {
                letterCount--;
                if (letterCount < 0) letterCount = 0;
                name[letterCount] = '\0';
                
                Malus malus = { "malus" };
                listeItem.push_back(malus);
            }


            //controle des obstacles
                        //déplacement
            for (int i = 0; i < NB_MAX_OBSTACLE; i++) obsPosition[i].x -= speedX;
            for (int i = 0; i < NB_MAX_OBSTACLE; i++) obstacle[i].rec.x = obsPosition[i].x;

            

            //collision et points
            for (int i = 0; i < NB_MAX_OBSTACLE; i++) {
                if (CheckCollisionCircleRec(perso.getPosition(), perso.getRadius(), obstacle[i].rec)) {
                    gameOver = true;
                    pause = false;
                }
                else if (!gameOver && obstacle[i].active && obstacle[i].rec.x < perso.getXposition()) {
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

        if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
        else mouseOnText = false;
        //dessin du perso et du sol
        //bref.getCenterX,bref.getCenterY,bref.getRadius,bref.getColor
        //while(liste)
        //DrawCircle(listeItem.front().getCenterX(), listeItem.front().getCenterY(), listeItem.front().getRadius(), listeItem.front().getColor());
        DrawCircle(perso.getXposition(), perso.getYposition(), perso.getRadius(), DARKGRAY);

        //Draw queue of items
        for (int i = 0; i < listeItem.size(); i++) {
            if (CheckCollisionCircles(listeItem[i].getPosition(), listeItem[i].getRadius(), perso.getPosition(), perso.getRadius())) {
                listeItem[i].setIsValid(false);
            }
            if (listeItem[i].getIsValid()) {
                DrawCircle(listeItem[i].getCenterX(), listeItem[i].getCenterY(), listeItem[i].getRadius(), listeItem[i].getColor());
            }
            if (listeItem[i].getCenterX() <= 0) listeItem.pop_front();
            else listeItem[i].setCenterX(listeItem[i].getCenterX()-speedX);
           
        }

        //affichage de la text box et bord rouge si souris dessus  if (mouseOnText) else
        DrawRectangleRec(textBox, LIGHTGRAY);
        if (mouseOnText)DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
        else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

        //Affcihage du texte dans la textBox
        DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);

        //Text pour dire cb de caractères dans la textBox
        DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount, 10), 710, 250, 20, DARKGRAY);

        //DrawCircle(perso.position.x, perso.position.y, perso.radius, DARKGRAY);		//perso
        DrawRectangle(0, VISUAL_FLOOR, screenWidth, screenHeight - FLOOR, DARKGRAY);		//sol

        //dessin des obstacles
        for (int i = 0; i < NB_MAX_OBSTACLE; i++) {
            DrawRectangle(obstacle[i].rec.x, obstacle[i].rec.y, obstacle[i].rec.width, obstacle[i].rec.height, BLACK);
        }

        //dessin du text
        DrawRectangleRec(textBox, LIGHTGRAY);
        DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);

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