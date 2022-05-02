#include "raylib.h"
#include "Item.h"
#include "Bonus.h"
#include "Malus.h"
#include "AugmentationVitesse.h"
#include "AugmentationVie.h"
#include "RalentissementVitesse.h"
#include "BaisseVie.h"
#include "Personnage.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <queue>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>

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

typedef struct TabCaractere {
    string tabText[100];
    int iterator =0;
} TabCaractere;


static const int screenWidth = 1600;
static const int screenHeight = 900;


static bool gameOver = false;
static bool pause = false;
static int score = 0;
static int hiScore = 0;
static int iemeQuestion;

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
static int tmpCollision =0;
static TabCaractere questions;
static TabCaractere reponses;
//----------Boite de texet---------
char name[50] = "\0";      // NOTE: One extra space required for null terminator char '\0'
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

void load_data() {
    ifstream flux("question.txt");
    string line = "ligne";
    int line_number(1);


    if (!flux) {
        cout << "Erreur ouverture fichier ./bank_quizz.txt" << endl;;
    }
    else {

        while (getline(flux, line)) {
            if (line_number > 3) line_number = 1;
            if (line_number == 1) {
                questions.tabText[questions.iterator] = line;
                questions.iterator++;
            }

            if (line_number == 2) {
                reponses.tabText[reponses.iterator] = line;
                reponses.iterator++;
            }
            if (line_number == 3) {
                //                line.pop_back();
                //answerSize.push_back(std::stoi((string)line));

            }
            line_number++;
        }
    }

}

int main(void) {
    
    load_data();


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
 
    textBox = { screenWidth / 2.0f - 100, 180, 225, 50 };

    //speedX = 2;

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
                while (key > 0)
                {
                    if ((key >= 32) && (key <= 125) && (letterCount < (reponses.tabText[iemeQuestion]).size()))
                    {
                        name[letterCount] = (char)key;
                        name[letterCount + 1] = '\0'; // Add null terminator at the end of the string.
                        letterCount++;
                    }

                    key = GetCharPressed();  // Check next character in the queue

                }
            }

            //controle du perso
            if (saut) {
                if (perso.getYposition() > FLOOR - JUMP_SIZE) {
                    perso.setYposition(perso.getYposition() - (2 + 1.2 * perso.getVitesse()));
                }
                else saut = false;
            }
            else {
                if (perso.getYposition() < FLOOR) {
                    perso.setYposition(perso.getYposition() + (2 + 1.2 * perso.getVitesse()));
                }
            }
            if (perso.getYposition() >= FLOOR) perso.setYposition(FLOOR);

            

            
            
            if (IsKeyPressed(KEY_BACKSPACE)) {
                letterCount--;
                if (letterCount < 0) letterCount = 0;
                name[letterCount] = '\0';
                
            }


            //controle des obstacles
                        //déplacement
            for (int i = 0; i < NB_MAX_OBSTACLE; i++) obsPosition[i].x -= perso.getVitesse();
            for (int i = 0; i < NB_MAX_OBSTACLE; i++) obstacle[i].rec.x = obsPosition[i].x;

            if (perso.getVie() <= 0) {
                gameOver = true;
            }

            //collision et points
            for (int i = 0; i < NB_MAX_OBSTACLE; i++) {
                if (CheckCollisionCircleRec(perso.getPosition(), perso.getRadius(), obstacle[i].rec)) {
                    //gameOver = true;
                    perso.setVie(perso.getVie()-1);
                    //tmpCollision = tmpCollision + 1;
                    pause = false;
                }
                else if (!gameOver && obstacle[i].active && obstacle[i].rec.x < perso.getXposition()) {
                    score += 10;
                    perso.setVitesse(perso.getVitesse()+1) ;
                    obstacle[i].active = false;

                    if (score > hiScore) hiScore = score;
                }
            }
            tmpCollision = 0;
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
        DrawText(TextFormat("VIE: %02i", perso.getVie()), 20, 95, 20, LIGHTGRAY);
       
        if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
        else mouseOnText = false;

        DrawCircle(perso.getXposition(), perso.getYposition(), perso.getRadius(), perso.getColor());

        //Draw queue of items
        for (int i = 0; i < listeItem.size(); i++) {
            if (CheckCollisionCircles(listeItem[i].getPosition(), listeItem[i].getRadius(), perso.getPosition(), perso.getRadius())) {
                listeItem[i].setIsValid(false);
                perso + listeItem[i];
            }
            if (listeItem[i].getIsValid()) {
                DrawCircle(listeItem[i].getCenterX(), listeItem[i].getCenterY(), listeItem[i].getRadius(), listeItem[i].getColor());
            }
            if (listeItem[i].getCenterX() <= 0) listeItem.pop_front();
            else listeItem[i].setCenterX(listeItem[i].getCenterX() - perso.getVitesse());
        }
        //affichage de la text box et bord rouge si souris dessus  if (mouseOnText) else
        DrawRectangleRec(textBox, LIGHTGRAY);
        if (mouseOnText)DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
        else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

        //check answer
        if (letterCount >= (reponses.tabText[iemeQuestion]).size()) {
            cout << "JE SUIS UN HERO ET J'ATTAQUE" << std::endl;
            if (strcmp(reponses.tabText[iemeQuestion].c_str(), name) == 0) {
                if (GetRandomValue(0, 1)) {
                    AugmentationVie v = {};
                    listeItem.push_back(v);
                }
                else {
                    RalentissementVitesse v = {};
                    listeItem.push_back(v);
                }
            }
            else{
                if (GetRandomValue(0, 1)) {
                    AugmentationVitesse v = {};
                    listeItem.push_back(v);
                }
                else {
                    BaisseVie v = {};
                    listeItem.push_back(v);
                }
            }
            iemeQuestion++;
            letterCount = 0;
            name[0] = '\0';
        }

        //Text pour dire cb de caractères dans la textBox
        DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount,(reponses.tabText[iemeQuestion]).size()), 710, 250, 20, DARKGRAY);
        DrawText(TextFormat("INPUT CHARS: %s", questions.tabText[iemeQuestion].c_str()), 710, 350, 20, DARKGRAY);
        
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