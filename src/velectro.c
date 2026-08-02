#include <raylib.h>
#include "election.h"
#include "modules.h"


static void afficher_menu_general(void)
{
    ClearBackground(RAYWHITE);

    DrawText("CEP - Gestion des Elections Presidentielles", 40, 40, 24, BLUE);

    const char *options[] = {
        "1. Partis Politiques",
        "2. Bureaux de Vote",
        "3. Electeurs",
        "4. Candidats",
        "5. Votes",
        "6. Rapports et Resultats",
        "ECHAP. Quitter"
    };

    int n = sizeof(options) / sizeof(options[0]);
    for (int i = 0; i < n; i++)
    {
        DrawText(options[i], 60, 100 + i * 35, 20, BLACK);
    }
}

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 500;

    InitWindow(screenWidth, screenHeight, "CEPR - Conseil Electoral Provisoire");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        /* TODO: gerer la selection des options (clavier/souris) et
           appeler les sous-menus de chaque module via modules.h */

        BeginDrawing();
            afficher_menu_general();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
