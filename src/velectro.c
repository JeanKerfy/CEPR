#include <raylib.h>
#include <stdbool.h>
#include "election.h"
#include "modules.h"

typedef enum
{
    ECRAN_BIENVENUE,
    ECRAN_MENU_GENERAL
} Ecran;

typedef struct
{
    Rectangle rect;
    const char* texte;
} Bouton;


static bool DessinerBouton(Bouton b)
{
    Vector2 souris = GetMousePosition();
    bool survole = CheckCollisionPointRec(souris, b.rect);

    Color couleurFond = survole ? SKYBLUE : LIGHTGRAY;
    DrawRectangleRec(b.rect, couleurFond);
    DrawRectangleLinesEx(b.rect, 2, DARKGRAY);

    int largeurTexte = MeasureText(b.texte, 20);
    int x = (int)(b.rect.x + (b.rect.width - largeurTexte) / 2);
    int y = (int)(b.rect.y + (b.rect.height - 20) / 2);
    DrawText(b.texte, x, y, 20, BLACK);

    return survole && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}


static void afficher_ecran_bienvenue(int screenWidth, int screenHeight, Ecran* ecran, bool* quitter)
{
    ClearBackground(RAYWHITE);

    const char* titre = "Conseil Electoral Provisoire";
    const char* soustitre = "Gestion des Elections Presidentielles";

    int titreLargeur = MeasureText(titre, 100);
    DrawText(titre, (screenWidth - titreLargeur) / 2, 120, 100, DARKBLUE);

    int soustitreLargeur = MeasureText(soustitre, 55);
    DrawText(soustitre, (screenWidth - soustitreLargeur) / 2, 230, 55, GRAY);

    int largeurBouton = 500;
    int hauteurBouton = 95;
    int espace = 150;
    int totalLargeur = largeurBouton * 2 + espace;
    int xDepart = (screenWidth - totalLargeur) / 2;
    int y = screenHeight / 2 + 40;

    Bouton boutonContinuer = {
        { (float)xDepart, (float)y, (float)largeurBouton, (float)hauteurBouton },
        "Continuer"
    };
    Bouton boutonQuitter = {
        { (float)(xDepart + largeurBouton + espace), (float)y, (float)largeurBouton, (float)hauteurBouton },
        "Quitter"
    };

    if (DessinerBouton(boutonContinuer))
    {
        *ecran = ECRAN_MENU_GENERAL;
    }

    if (DessinerBouton(boutonQuitter))
    {
        *quitter = true;
    }
}


static void afficher_menu_general(void)
{
    ClearBackground(RAYWHITE);

    DrawText("CEP - Gestion des Elections Presidentielles", 40, 40, 24, DARKGRAY);

    const char* options[] = {
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
    const int screenWidth = 1700;
    const int screenHeight = 950;

    InitWindow(screenWidth, screenHeight, "CEPR - Conseil Electoral Provisoire");
    SetTargetFPS(60);

    Ecran ecranCourant = ECRAN_BIENVENUE;
    bool quitter = false;

    while (!WindowShouldClose() && !quitter)
    {
        BeginDrawing();
        switch (ecranCourant)
        {
        case ECRAN_BIENVENUE:
            afficher_ecran_bienvenue(screenWidth, screenHeight, &ecranCourant, &quitter);
            break;
        case ECRAN_MENU_GENERAL:
            afficher_menu_general();
            break;
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}