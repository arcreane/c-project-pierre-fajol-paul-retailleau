

#include "raylib.h"
#include <stdlib.h>     

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    //Créer la fenetre avant les blocks,meme si ils ne sont pas encore dessiné.
    InitWindow(screenWidth, screenHeight, "Projet");

    //-----------personnage------------
    const float sizePersoHeight = 100;
    const float sizePersoWidth = 50;

    Rectangle personnage = { 10, GetScreenHeight() - sizePersoHeight, sizePersoWidth, sizePersoHeight };
    //---------------------------------


    //-----------obstacle -------------
    const float sizeObsHeight = 20;
    const float sizeObsWidth = 20;
    int vitesseObstacle = 1;

    Rectangle obstacle = { GetScreenWidth() - sizeObsWidth, GetScreenHeight() - sizeObsHeight,sizeObsWidth , sizeObsHeight };
    //---------------------------------

    //----------Boite de texet---------
    char name[10] = "\0";      // NOTE: One extra space required for null terminator char '\0'
    int letterCount = 0;       //compteur de lettre pour la saisie
    Rectangle textBox = { GetScreenWidth() / 2.0f - 100, 180, 225, 50 };
    bool mouseOnText = false;       //boolen pour detecter la souris sur le texte
    int framesCounter = 0;          // ???
    //--------------------------------


    //------------header------------
    Rectangle boxCollision = { 0 }; // Collision rectangle
    int screenUpperLimit = 40;      // taille rectangle noir en haut 
    //------------------------------


    bool pause = false;             // Movement pause
    bool collision = false;         // Collision detection

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        //Detecte si la souris est dans la textbox
        if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
        else mouseOnText = false;

        // Check if more characters have been pressed on the same frame
        int key = GetCharPressed();
        while (key > 0)
        {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 32) && (key <= 125) && (letterCount < 10))
            {
                name[letterCount] = (char)key;
                name[letterCount + 1] = '\0'; // Add null terminator at the end of the string.
                letterCount++;
                personnage.y -= vitesseObstacle;// On bouge le perso vers le haut si une touche est pressé
            }

            key = GetCharPressed();  // Check next character in the queue
        }

        //Enlève du texte à chaque backspace
        if (IsKeyPressed(KEY_BACKSPACE))
        {
            letterCount--;
            if (letterCount < 0) letterCount = 0;
            name[letterCount] = '\0';
        }

        //affichage de la text box et bord rouge si souris dessus  if (mouseOnText) else
        DrawRectangleRec(textBox, LIGHTGRAY);
        if (mouseOnText)DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
        else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

        //Affcihage du texte dans la textBox
        DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);

        //Text pour dire cb de caractères dans la textBox
        DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount, 10), 315, 250, 20, DARKGRAY);

        //j'ai pas programmé la pause en sah mais sinon l'obstacle bouge sur l ecran
        if (!pause) obstacle.x -= vitesseObstacle;

        // Check boxes collision
        collision = CheckCollisionRecs(personnage, obstacle);

        // Get collision rectangle (only on collision)
        if (collision) boxCollision = GetCollisionRec(personnage, obstacle);

        //affichage du rectangle header qui change si collision
        DrawRectangle(0, 0, screenWidth, screenUpperLimit, collision ? RED : BLACK);

        //affichage du rectangle personnage et obstacle
        DrawRectangleRec(personnage, GOLD);
        DrawRectangleRec(obstacle, BLUE);

        if (collision)
        {
            // Draw collision message
            DrawText("COLLISION!", GetScreenWidth() / 2 - MeasureText("COLLISION!", 20) / 2, screenUpperLimit / 2 - 10, 20, BLACK);

            // Draw collision area
            DrawText(TextFormat("Collision Area: %i", (int)boxCollision.width * (int)boxCollision.height), GetScreenWidth() / 2 - 100, screenUpperLimit + 10, 20, BLACK);
        }

        DrawFPS(10, 10);

        EndDrawing();
        //-----------------------------------------------------
    }

    // De-Initialization
    //---------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //----------------------------------------------------------

    return 0;
}
