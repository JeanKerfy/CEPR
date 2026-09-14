/*------------------------------------------------------------------------*/
/* FICHIER:                         velectro.c                            */
/*AUTEUR:                           EMPR                                  */
/*DATE DE CREATION:                 02/09/2026                            */
/*DATE DE MODIFICATION:             02/09/2026                            */
/*DESCRIPTION:                      menu général, sous-menus et fenêtre   */
/*                                  Raylib ; contient le main             */
/*------------------------------------------------------------------------*/
#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "election.h"

Texture2D fond;
Music musique;

#define LARGEUR_REF 1920
#define HAUTEUR_REF 1080
float echelle_x = 1.0f;
float echelle_y = 1.0f;

int px(int valeur) { return (int)(valeur * echelle_x); }
int py(int valeur) { return (int)(valeur * echelle_y); }
int pf(int taille) { return (int)(taille * fminf(echelle_x, echelle_y)); }

// --- Écrans du programme ---
typedef enum {
    ECRAN_ACCUEIL,
    ECRAN_MENU_PARTI,ECRAN_AJOUT_PARTI,ECRAN_PARTI_RECHERCHE,ECRAN_LISTE_PARTI,ECRAN_AFFICHAGE_PARTI, ECRAN_MODIF_PARTI,
    ECRAN_MENU_BV, ECRAN_AJOUT_BV,ECRAN_BV_RECHERCHE, ECRAN_BV_RECHERCHEDEPART, ECRAN_LISTE_BV, ECRAN_AFFICHAGE_BV, ECRAN_MODIF_BV,
    ECRAN_MENU_ELECTEURS, ECRAN_AJOUT_ELECTEUR, ECRAN_ELECTEUR_RECHERCHE, ECRAN_AFFICHAGE_ELECTEUR, ECRAN_MODIF_ELECTEUR,
    ECRAN_MENU_CANDIDATS, ECRAN_AJOUT_CANDIDAT, ECRAN_CANDIDAT_RECHERCHE, ECRAN_AFFICHAGE_CANDIDAT, ECRAN_MODIF_CANDIDAT,
    ECRAN_MENU_VOTES, ECRAN_ENREGISTRER_VOTE,
    ECRAN_MENU_RAPPORTS, ECRAN_EL_RECHERCHEDEPART, ECRAN_LISTE_ELECTEUR, ECRAN_LISTE_CANDIDAT, ECRAN_LISTE_RAPPBV, ECRAN_RESULTATS
} Ecran;

Ecran ecran_actuel = ECRAN_ACCUEIL;
int champ_actif = -1; // aucun champ actif

// Couleurs personnalisées
Color BLEU_FONCE = (Color){15, 40, 70, 255};
Color BLEU = (Color){30, 80, 150, 255};
Color GRIS_CLAIR = (Color){230, 230, 230, 255};
Color ROUGE_ERREUR = (Color){200, 30, 30, 255};
Color GRIS_INFO = (Color){ 120, 120, 120, 255 };

// 10 Départements d'Haïti
const char* NOMS_DEPARTEMENTS[10] = {
    "ARTIBONITE", "CENTRE", "GRANDE-ANSE", "NIPPES", "NORD",
    "NORD-EST", "NORD-OUEST", "OUEST", "SUD", "SUD-EST"
};

// Buffers du formulaire "parti politique"
char pp_nom[50] = "", pp_jour[5] = "", pp_mois[5] = "", pp_annee[5] = "";
char pp_no_rue[10] = "", pp_rue[50] = "", pp_ville[30] = "", pp_departement[50] = "";
char pp_responsable[50] = "", pp_telephone[20] = "", pp_email[50] = "";
char pp_erreur_nom[60] = "", pp_erreur_date[60] = "";
char pp_erreur_no_rue[60] = "", pp_erreur_rue[60] = "", pp_erreur_ville[60] = "", pp_erreur_departement[60] = "";
char pp_erreur_responsable[60] = "",pp_erreur_telephone[60] = "", pp_erreur_email[60] = "", pp_erreur_generale[100] = "";
char recherche_id[15] = "", recherche_erreur[60] = "";
int pp_id_modif = 0;
int modif_pp, affich_pp;

// --- Buffers BV ---
char bv_no_rue[10] = "", bv_rue[50] = "", bv_ville[30] = "", bv_departement[50] = "";
char bv_president[50] = "", bv_vpresident[50] = "", bv_secret[50] = "";
char bv_err_no_rue[60] = "", bv_err_rue[60] = "", bv_err_ville[60] = "", bv_err_departement[60] = "";
char bv_err_president[60] = "", bv_err_vpresident[60] = "", bv_err_secret[60] = "", bv_err_generale[100] = "";
char bv_filtre_dept[3] = "";
int bv_id_modif = 0, idx_departement =0;
int modif_bv, affich_bv, bv;

// --- Buffers Electeurs ---
char el_ninu[15] = "", el_nom[50] = "", el_prenom[50] = "";
char el_no_rue[10] = "", el_rue[50] = "", el_ville[30] = "", el_departement[50] = "", el_id_bv[10] = "";
char el_err_ninu[60] = "", el_err_nom[60] = "", el_err_prenom[60] = "";
char el_err_no_rue[60] = "", el_err_rue[60] = "", el_err_ville[60] = "", el_err_departement[60] = "", el_err_id_bv[60] = "";
char el_err_generale[100] = "";
int el_id_modif = 0;
int modif_electeur, affich_electeur;

// --- Buffers Candidats ---
char cd_id[5] = "", cd_nom[50] = "", cd_prenom[50] = "";
char cd_jour[5] = "", cd_mois[5] = "", cd_annee[5] = "";
char cd_no_rue[10] = "", cd_rue[50] = "", cd_ville[30] = "", cd_departement[50] = "";
char cd_email[50] = "", cd_telephone[20] = "", cd_type[10] = "";
char cd_err_nom[60] = "", cd_err_prenom[60] = "", cd_err_date[60] = "";
char cd_err_no_rue[60] = "", cd_err_rue[60] = "", cd_err_ville[60] = "", cd_err_departement[60] = "";
char cd_err_email[60] = "", cd_err_telephone[60] = "", cd_err_type[60] = "", cd_err_generale[100] = "";
int cd_id_modif = 0;
int modif_cd, affich_cd;

// --- Buffers Votes ---
char vt_ninu[15] = "", vt_id_candid[10] = "";
char vt_err_ninu[60] = "", vt_err_generale[100] = "";
char vt_confirmation[100] = "";

int rapp;

/*--------------------------------------------------------------------------------*/
/* FONCTION:              saisir_champ                                            */
/* DESCRIPTION:           Dessine un rectangle de saisie de texte, capture les    */
/*                        touches tapées si ce champ est actif, affiche un        */
/*                        message d'erreur sous le champ si non vide                */
/* PARAMETRES:            rect, buffer, taille_max, index (position de ce champ   */
/*                        dans le formulaire, comparé à champ_actif), erreur      */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      02/09/2026                                              */
/* DATE DE MODIFICATION:  02/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void saisir_champ(Rectangle rect, char *buffer, int taille_max, int index, char *erreur) {
    int longueur = strlen(buffer);
    int actif = (champ_actif == index);

    Rectangle clic = {rect.x - px(20),rect.y - py(20),rect.width + px(40),rect.height + py(40)};

    if (CheckCollisionPointRec(GetMousePosition(), clic) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        champ_actif = index;
        actif = 1;
    }

    if (actif) {
        int touche = GetCharPressed();
        while (touche > 0) {
            if (longueur < taille_max - 1 && touche >= 32 && (touche <= 126 || touche >= 160) && touche <= 255) {
                buffer[longueur] = (char)touche;
                buffer[longueur + 1] = '\0';
                longueur++;
            }
            touche = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && longueur > 0)
            buffer[longueur - 1] = '\0';
    }

    DrawRectangleRec(rect, WHITE);
    DrawRectangleLinesEx(rect, 2, actif ? BLEU : GRIS_INFO);
    DrawText(buffer, (int)rect.x + px(8), (int)rect.y + py(10), pf(20), BLACK);

    if (actif && ((int)(GetTime() * 2) % 2 == 0)) {
        int largeur_texte = MeasureText(buffer, pf(20));
        DrawText("|", (int)rect.x + px(8) + largeur_texte, (int)rect.y + py(10), pf(20), BLACK);
    }

    if (strlen(erreur) > 0)
        DrawText(erreur, (int)rect.x, (int)rect.y + (int)rect.height + py(4), pf(16), ROUGE_ERREUR);
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              bouton                                                  */
/* DESCRIPTION:           Dessine un bouton rectangulaire avec un texte centré,   */
/*                        retourne 1 s'il vient d'être cliqué                     */
/* PARAMETRES:            rect, texte, couleur de fond                            */
/* VALEUR DE RETOUR:      1 si cliqué ce frame, 0 sinon                           */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      02/09/2026                                              */
/* DATE DE MODIFICATION:  02/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int bouton(Rectangle rect, char *texte, Color couleur) {
    int clique = 0;
    int largeur_texte = MeasureText(texte, pf(20));

    DrawRectangleRec(rect, couleur);
    DrawText(texte, (int)(rect.x + rect.width / 2 - largeur_texte / 2), (int)(rect.y + rect.height / 2 - py(10)), pf(20), WHITE);

    if (CheckCollisionPointRec(GetMousePosition(), rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        clique = 1;

    return clique;
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              bouton_retour                                           */
/* DESCRIPTION:           Dessine l'icône/bouton "< Retour" en haut à gauche      */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      1 si cliqué ce frame, 0 sinon                           */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      02/09/2026                                              */
/* DATE DE MODIFICATION:  02/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int bouton_retour(void) {
    Rectangle rect = {(float)px(20), (float)py(20), (float)px(110), (float)py(40)};
    return bouton(rect, "< Retour", BLEU_FONCE);
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              logo_coin                                               */
/* DESCRIPTION:           Affiche le petit branding "CEP" en haut à droite       */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      02/09/2026                                              */
/* DATE DE MODIFICATION:  02/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void logo_coin(void) {
    int largeur = GetScreenWidth();
    DrawText("CEP", largeur - px(120), py(20), pf(30), BLEU_FONCE);
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_accueil                                        */
/* DESCRIPTION:           Dessine l'écran d'accueil : titre CEP et boutons vers  */
/*                        chaque module                                           */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  07/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_accueil(void) {
    int largeur = GetScreenWidth();
    int titre_largeur = MeasureText("CEP", pf(70));

    DrawRectangle(0, 0, largeur, py(90), BLEU_FONCE);
    DrawText("CEP", largeur / 2 - titre_largeur / 2, py(150), pf(70), BLEU_FONCE);
    DrawText("Conseil Electoral Provisoire", largeur / 2 - MeasureText("Conseil Electoral Provisoire", pf(24)) / 2, py(230), pf(24), GRIS_INFO);

    int y = py(320);
    if (bouton((Rectangle) { largeur / 2 - px(200), y, px(400), py(55) }, "Partis Politiques", BLEU)) {
        ecran_actuel = ECRAN_MENU_PARTI;
    }
    y += py(70);
    if (bouton((Rectangle) { largeur / 2 - px(200), y, px(400), py(55) }, "Bureaux de Vote", BLEU)) {
        ecran_actuel = ECRAN_MENU_BV;
    }
    y += py(70);
    if (bouton((Rectangle) { largeur / 2 - px(200), y, px(400), py(55) }, "Electeurs", BLEU)) {
        ecran_actuel = ECRAN_MENU_ELECTEURS;
    }
    y += py(70);
    if (bouton((Rectangle) { largeur / 2 - px(200), y, px(400), py(55) }, "Candidats", BLEU)) {
        ecran_actuel = ECRAN_MENU_CANDIDATS;
    }
    y += py(70);
    if (bouton((Rectangle) { largeur / 2 - px(200), y, px(400), py(55) }, "Votes", BLEU)) {
        ecran_actuel = ECRAN_MENU_VOTES;
    }
    y += py(70);
    if (bouton((Rectangle) { largeur / 2 - px(200), y, px(400), py(55) }, "Rapports et Resultats", BLEU)) {
        ecran_actuel = ECRAN_MENU_RAPPORTS;
    }
    y += py(70);
    if (bouton((Rectangle) { largeur / 2 - px(200), y, px(400), py(55) }, "Quitter", ROUGE_ERREUR)) {
        UnloadTexture(fond);
        CloseWindow();
        exit(0);
    }
}


// ============================== **PARTIS POLITIQUES** ==============================

/*-------------------------------------------------------------------------------*/
/* FONCTION:              vider_form_pp                                          */
/* DESCRIPTION:           Réinitialise tous les buffers et erreurs du formulaire */
/*                        parti politique                                        */
/* PARAMETRES:            Aucun                                                  */
/* VALEUR DE RETOUR:      Aucune                                                 */
/* AUTEUR:                EMPR                                                   */
/* DATE DE CREATION:      02/09/2026                                             */
/* DATE DE MODIFICATION:  02/09/2026                                             */
/*-------------------------------------------------------------------------------*/
void vider_form_pp(void) {
    pp_nom[0] = '\0';
    pp_jour[0] = '\0';
    pp_mois[0] = '\0';
    pp_annee[0] = '\0';
    pp_no_rue[0] = '\0';
    pp_rue[0] = '\0';
    pp_ville[0] = '\0';
    pp_departement[0] = '\0';
    pp_responsable[0] = '\0';
    pp_telephone[0] = '\0';
    pp_email[0] = '\0';
    pp_id_modif = 0;

    pp_erreur_nom[0] = '\0';
    pp_erreur_date[0] = '\0';
    pp_erreur_no_rue[0] = '\0';
    pp_erreur_rue[0] = '\0';
    pp_erreur_ville[0] = '\0';
    pp_erreur_departement[0] = '\0';
    pp_erreur_responsable[0] = '\0';
    pp_erreur_telephone[0] = '\0';
    pp_erreur_email[0] = '\0';
    pp_erreur_generale[0] = '\0';

    champ_actif = -1;
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              departement_valide                                      */
/* DESCRIPTION:           Vérifie que le champ Departement est valide             */
/* PARAMETRES:            Chaine de caracteres saisi par l'utilisateur            */
/* VALEUR DE RETOUR:      1 si le departement est valide, 0 sinon                 */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  12/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int departement_valide(char* dept) {
    for (int i = 0; i < 10; i++) {
        if (strcmp(dept, NOMS_DEPARTEMENTS[i]) == 0) return 1;
    }
    return 0;
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_menu_parti                                     */
/* DESCRIPTION:           Dessine le sous-menu du module Partis Politiques        */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  07/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_menu_parti(void) {
    int largeur = GetScreenWidth();

    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_ACCUEIL;

    DrawText("Partis Politiques", largeur / 2 - MeasureText("Partis Politiques", pf(40)) / 2, py(110), pf(40), BLEU_FONCE);

    int y = py(220);
    if (bouton((Rectangle){largeur / 2 - px(180), y, px(360), py(55)}, "Ajouter un parti", BLEU)) {
        vider_form_pp();
        ecran_actuel = ECRAN_AJOUT_PARTI;
    }
    y += py(70);
    if (bouton((Rectangle){largeur / 2 - px(180), y, px(360), py(55)}, "Voir la liste des partis", BLEU)) {
        ecran_actuel = ECRAN_LISTE_PARTI;
    }
    y += py(70);
    if (bouton((Rectangle){largeur / 2 - px(180), y, px(360), py(55)}, "Rechercher un parti", BLEU)) {
        recherche_id[0] = '\0'; recherche_erreur[0] = '\0';
        champ_actif = -1;
        modif_pp = 0; affich_pp = 1;
        ecran_actuel = ECRAN_PARTI_RECHERCHE;
    }
    y += py(70);
    if (bouton((Rectangle) { largeur / 2 - px(180), y, px(360), py(55) }, "Modifier un parti", BLEU)) {
        recherche_id[0] = '\0'; recherche_erreur[0] = '\0';
        champ_actif = -1;
        modif_pp = 1; affich_pp = 0;
        ecran_actuel = ECRAN_PARTI_RECHERCHE;
    }
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_form_pp                                        */
/* DESCRIPTION:           Dessine les 11 champs du formulaire parti politique     */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  07/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_form_pp(void) {
    int x = px(300);
    int y = py(130);

    DrawText("Nom du parti", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(400), py(36)}, pp_nom, sizeof(pp_nom), 0, pp_erreur_nom);
    y += py(80);

    DrawText("Date de creation (jour / mois / annee)", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(80), py(36)}, pp_jour, sizeof(pp_jour), 1, pp_erreur_date);
    saisir_champ((Rectangle){x + px(100), y + py(24), px(80), py(36)}, pp_mois, sizeof(pp_mois), 2, "");
    saisir_champ((Rectangle){x + px(200), y + py(24), px(100), py(36)}, pp_annee, sizeof(pp_annee), 3, "");
    y += py(80);

    DrawText("No de rue", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(150), py(36)}, pp_no_rue, sizeof(pp_no_rue), 4, pp_erreur_no_rue);
    y += py(80);

    DrawText("Rue", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(400), py(36)}, pp_rue, sizeof(pp_rue), 5, pp_erreur_rue);
    y += py(80);

    DrawText("Ville", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(300), py(36)}, pp_ville, sizeof(pp_ville), 6, pp_erreur_ville);
    y += py(80);

    DrawText("Departement (EN MAJUSCULE)", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(300), py(36)}, pp_departement, sizeof(pp_departement), 7, pp_erreur_departement);
    y += py(80);

    DrawText("Responsable", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(400), py(36)}, pp_responsable, sizeof(pp_responsable), 8, pp_erreur_responsable);
    y += py(80);

    DrawText("Telephone", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(250), py(36)}, pp_telephone, sizeof(pp_telephone), 9, pp_erreur_telephone);
    y += py(80);

    DrawText("Email", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(400), py(36)}, pp_email, sizeof(pp_email), 10, pp_erreur_email);
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              valider_champs_pp                                       */
/* DESCRIPTION:           Vérifie que chaque champ obligatoire est rempli         */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      1 si tous les champs sont remplis, 0 sinon              */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  07/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int valider_champs_pp(void) {
    int tout_rempli = 1;
    char *msg = "Case vide! Entrer l'information appropriee.";

    pp_erreur_nom[0] = '\0';
    pp_erreur_date[0] = '\0';
    pp_erreur_no_rue[0] = '\0';
    pp_erreur_rue[0] = '\0';
    pp_erreur_ville[0] = '\0';
    pp_erreur_departement[0] = '\0';
    pp_erreur_responsable[0] = '\0';
    pp_erreur_telephone[0] = '\0';
    pp_erreur_email[0] = '\0';

    if (strlen(pp_nom) == 0) { strcpy(pp_erreur_nom, msg); tout_rempli = 0; }
    if (strlen(pp_jour) == 0 || strlen(pp_mois) == 0 || strlen(pp_annee) == 0) { strcpy(pp_erreur_date, msg); tout_rempli = 0; }
    if (strlen(pp_no_rue) == 0) { strcpy(pp_erreur_no_rue, msg); tout_rempli = 0; }
    if (strlen(pp_rue) == 0) { strcpy(pp_erreur_rue, msg); tout_rempli = 0; }
    if (strlen(pp_ville) == 0) { strcpy(pp_erreur_ville, msg); tout_rempli = 0; }
    if (strlen(pp_departement) == 0) { strcpy(pp_erreur_departement, msg); tout_rempli = 0; }
    else if (!departement_valide(pp_departement)) {
        strcpy(pp_erreur_departement, "Departement invalide.");
        tout_rempli = 0;
    }
    if (strlen(pp_responsable) == 0) { strcpy(pp_erreur_responsable, msg); tout_rempli = 0; }
    if (strlen(pp_telephone) == 0) { strcpy(pp_erreur_telephone, msg); tout_rempli = 0; }
    if (strlen(pp_email) == 0) { strcpy(pp_erreur_email, msg); tout_rempli = 0; }

    return tout_rempli;
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_ajout_parti                                    */
/* DESCRIPTION:           Écran d'ajout d'un parti politique                      */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  07/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_ajout_parti(void) {
    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_MENU_PARTI;

    char titre[50];
    sprintf(titre, "Ajouter le parti PP-%03d", obtenir_dernier_id_PP(PP_FILE) + 1);
    DrawText(titre, px(650), py(70), pf(40), BLEU_FONCE);

    dessiner_form_pp();

    if (strlen(pp_erreur_generale) > 0)
        DrawText(pp_erreur_generale, px(650), py(880), pf(18), ROUGE_ERREUR);

    if (bouton((Rectangle){px(300), py(920), px(200), py(50)}, "Valider", BLEU)) {
        pp_erreur_generale[0] = '\0';

        if (valider_champs_pp()) {
            PartiPolitique pp;

            strcpy(pp.Nom, pp_nom);
            pp.Date_creat.jour = atoi(pp_jour);
            pp.Date_creat.mois = atoi(pp_mois);
            pp.Date_creat.annee = atoi(pp_annee);
            strcpy(pp.Adr.no_rue, pp_no_rue);
            strcpy(pp.Adr.rue, pp_rue);
            strcpy(pp.Adr.ville, pp_ville);
            strcpy(pp.Adr.departement, pp_departement);
            strcpy(pp.Responsable, pp_responsable);
            strcpy(pp.Telephone, pp_telephone);
            strcpy(pp.Email, pp_email);

            if (ins_PP(&pp)) {
                vider_form_pp();
                ecran_actuel = ECRAN_MENU_PARTI;
            } else {
                strcpy(pp_erreur_generale, "Erreur : date de creation invalide.");
            }
        }
    }
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_liste_parti                                    */
/* DESCRIPTION:           Liste tous les partis politiques enregistrés            */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  07/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_liste_parti(void) {
    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_MENU_PARTI;

    DrawText("Informations des partis politiques", px(650), py(70), pf(40), BLEU_FONCE);

    int dernier_id = obtenir_dernier_id_PP(PP_FILE);
    int y = py(140);
    int id;
    PartiPolitique pp;

    DrawText("ID", px(20), y, pf(20), BLACK);
    DrawText("Nom", px(105), y, pf(20), BLACK);
	DrawText("Date creation", px(285), y, pf(20), BLACK);
	DrawText("Date inscription", px(475), y, pf(20), BLACK);
	DrawText("Adresse", px(656), y, pf(20), BLACK);
    DrawText("Responsable", px(1200), y, pf(20), BLACK);
    DrawText("Téléphone", px(1500), y, pf(20), BLACK);
    DrawText("Email", px(1640), y, pf(20), BLACK);
    char adresse[150];
	char identifiant[10];
    char date_creation[15];
    char date_ins[15];
	DrawLine(px(10), y + py(30), px(1900), y + py(30), BLACK);
	y += py(34);

    for (id = 1; id <= dernier_id; id++) {
        if (lire_PP(PP_FILE, id, &pp)) {
            snprintf(identifiant, sizeof(identifiant), "PP-%03d", pp.Id_PP);
            snprintf(adresse, sizeof(adresse), "%s, %s, %s, %s", pp.Adr.no_rue, pp.Adr.rue, pp.Adr.ville, pp.Adr.departement);
            snprintf(date_creation, sizeof(date_creation), "%02d/%02d/%04d", pp.Date_creat.jour, pp.Date_creat.mois, pp.Date_creat.annee);
            snprintf(date_ins, sizeof(date_ins), "%02d/%02d/%04d", pp.Date_ins.jour, pp.Date_ins.mois, pp.Date_ins.annee);
            DrawText(identifiant, px(20), y, pf(20), BLACK);
            DrawText(pp.Nom, px(105), y, pf(20), BLACK);
            DrawText(date_creation, px(285), y, pf(20), BLACK);
            DrawText(date_ins, px(475), y, pf(20), BLACK);
            DrawText(adresse, px(656), y, pf(20), BLACK);
            DrawText(pp.Responsable, px(1200), y, pf(20), BLACK);
            DrawText(pp.Telephone, px(1500), y, pf(20), BLACK);
            DrawText(pp.Email, px(1640), y, pf(20), BLACK);
            y += py(34);
        }
    }

    if (dernier_id == 0)
        DrawText("Aucun parti enregistre.", px(300), y, pf(20), GRIS_INFO);
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_parti_recherche                                */
/* DESCRIPTION:           Écran de recherche d'un parti par Id_PP                 */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  07/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_parti_recherche(void) {
    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_MENU_PARTI;

    DrawText("Rechercher un parti", px(300), py(100), pf(30), BLEU_FONCE);
    DrawText("ID du parti (numero seul)", px(300), py(200), pf(18), BLACK);
    saisir_champ((Rectangle){px(300), py(224), px(200), py(36)}, recherche_id, sizeof(recherche_id), 0, recherche_erreur);

    if (bouton((Rectangle){px(300), py(300), px(200), py(50)}, "Rechercher", BLEU)) {
        recherche_erreur[0] = '\0';

        if (strlen(recherche_id) == 0) {
            strcpy(recherche_erreur, "Case vide! Entrer l'information appropriee.");
        } else {
            PartiPolitique pp;
            int id = atoi(recherche_id);

            if (lire_PP(PP_FILE, id, &pp)) {
                strcpy(pp_nom, pp.Nom);
                sprintf(pp_jour, "%d", pp.Date_creat.jour);
                sprintf(pp_mois, "%d", pp.Date_creat.mois);
                sprintf(pp_annee, "%d", pp.Date_creat.annee);
                strcpy(pp_no_rue, pp.Adr.no_rue);
                strcpy(pp_rue, pp.Adr.rue);
                strcpy(pp_ville, pp.Adr.ville);
                strcpy(pp_departement, pp.Adr.departement);
                strcpy(pp_responsable, pp.Responsable);
                strcpy(pp_telephone, pp.Telephone);
                strcpy(pp_email, pp.Email);
                
                pp_id_modif = pp.Id_PP;
                pp_erreur_generale[0] = '\0';
                champ_actif = -1;
                if (affich_pp && !modif_pp)
					ecran_actuel = ECRAN_AFFICHAGE_PARTI;
				if (modif_pp && !affich_pp)
					ecran_actuel = ECRAN_MODIF_PARTI;
            } 
            else 
                strcpy(recherche_erreur, "Aucun parti trouve avec cet ID.");
            
        }
    }
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_modif_parti                                    */
/* DESCRIPTION:           Écran d'édition d'un parti (champs pré-remplis)         */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      02/09/2026                                              */
/* DATE DE MODIFICATION:  02/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_modif_parti(void) {
    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_MENU_PARTI;

    char titre[35];
    sprintf(titre, "Modifier le parti PP-%03d", pp_id_modif);
    DrawText(titre, px(650), py(70), pf(40), BLEU_FONCE);

    dessiner_form_pp();

    if (strlen(pp_erreur_generale) > 0)
        DrawText(pp_erreur_generale, px(300), py(880), pf(18), ROUGE_ERREUR);

    if (bouton((Rectangle){px(300), py(920), px(200), py(50)}, "Enregistrer", BLEU)) {
        pp_erreur_generale[0] = '\0';

        if (valider_champs_pp()) {
            PartiPolitique pp;

            pp.Id_PP = pp_id_modif;
            strcpy(pp.Nom, pp_nom);
            pp.Date_creat.jour = atoi(pp_jour);
            pp.Date_creat.mois = atoi(pp_mois);
            pp.Date_creat.annee = atoi(pp_annee);
            strcpy(pp.Adr.no_rue, pp_no_rue);
            strcpy(pp.Adr.rue, pp_rue);
            strcpy(pp.Adr.ville, pp_ville);
            strcpy(pp.Adr.departement, pp_departement);
            strcpy(pp.Responsable, pp_responsable);
            strcpy(pp.Telephone, pp_telephone);
            strcpy(pp.Email, pp_email);

            if (modif_PP(&pp)) {
                vider_form_pp();
                ecran_actuel = ECRAN_MENU_PARTI;
            } else {
                strcpy(pp_erreur_generale, "Erreur : date de creation invalide.");
            }
        }
    }
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_affichage_parti                                */
/* DESCRIPTION:           Écran d'affichage d'un parti                            */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      02/09/2026                                              */
/* DATE DE MODIFICATION:  07/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_affichage_parti(void) {
    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_MENU_PARTI;
    DrawText("Informations d'un parti", px(650), py(70), pf(40), BLEU_FONCE);
    int y = py(140);
    PartiPolitique pp;
    char adresse[150];
    char identifiant[10];
    char date_creat[25];
    char date_ins[25];

    DrawLine(px(10), y + py(30), px(1900), y + py(30), BLACK);
    DrawText("ID", px(20), y, pf(20), BLACK);
    DrawText("Nom", px(210), y, pf(20), BLACK);
    DrawText("Date creation", px(980), y, pf(20), BLACK);
    DrawText("Date inscription", px(1280), y, pf(20), BLACK);

    DrawLine(px(10), y + py(130), px(1900), y + py(130), BLACK);
    DrawText("Adresse", px(30), y + py(100), pf(20), BLACK);
    DrawText("Responsable", px(960), y + py(100), pf(20), BLACK);
    DrawText("Téléphone", px(1450), y + py(100), pf(20), BLACK);
    DrawText("Email", px(1650), y + py(100), pf(20), BLACK);
    y += py(34);

    if (lire_PP(PP_FILE, pp_id_modif, &pp)) {
        snprintf(identifiant, sizeof(identifiant), "PP-%03d", pp.Id_PP);
        snprintf(adresse, sizeof(adresse), "%s, %s, %s, %s", pp.Adr.no_rue, pp.Adr.rue, pp.Adr.ville, pp.Adr.departement);
        snprintf(date_creat, sizeof(date_creat), "%02d/%02d/%04d", pp.Date_creat.jour, pp.Date_creat.mois, pp.Date_creat.annee);
        snprintf(date_ins, sizeof(date_ins), "%02d/%02d/%04d", pp.Date_ins.jour, pp.Date_ins.mois, pp.Date_ins.annee);

        DrawText(identifiant, px(20), y, pf(20), BLACK);
        DrawText(pp.Nom, px(210), y, pf(20), BLACK);
        DrawText(date_creat, px(980), y, pf(20), BLACK);
        DrawText(date_ins, px(1280), y, pf(20), BLACK);

        DrawText(adresse, px(30), y + py(100), pf(20), BLACK);
		DrawText(pp.Responsable, px(960), y + py(100), pf(20), BLACK);
        DrawText(pp.Telephone, px(1450), y + py(100), pf(20), BLACK);
        DrawText(pp.Email, px(1650), y + py(100), pf(20), BLACK);
    }
}

// ============================== **BUREAUX DE VOTE** ==============================

/*-------------------------------------------------------------------------------*/
/* FONCTION:              vider_form_bv                                          */
/* DESCRIPTION:           Réinitialise tous les buffers et erreurs du formulaire */
/*                        bureau de vote                                         */
/* PARAMETRES:            Aucun                                                  */
/* VALEUR DE RETOUR:      Aucune                                                 */
/* AUTEUR:                EMPR                                                   */
/* DATE DE CREATION:      02/09/2026                                             */
/* DATE DE MODIFICATION:  05/09/2026                                             */
/*-------------------------------------------------------------------------------*/
void vider_form_bv(void) {
    bv_no_rue[0] = bv_rue[0] = bv_ville[0] = bv_departement[0] = '\0';
    bv_president[0] = bv_vpresident[0] = bv_secret[0] = '\0';
    bv_err_no_rue[0] = bv_err_rue[0] = bv_err_ville[0] = bv_err_departement[0] = '\0';
    bv_err_president[0] = bv_err_vpresident[0] = bv_err_secret[0] = bv_err_generale[0] = '\0';
    bv_id_modif = 0;
    champ_actif = -1;
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_menu_bv                                        */
/* DESCRIPTION:           Dessine le sous-menu du module Bureau de Votes          */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      01/09/2026                                              */
/* DATE DE MODIFICATION:  07/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_menu_bv(void) {
    int largeur = GetScreenWidth();
    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_ACCUEIL;
    DrawText("Bureaux de Vote", largeur / 2 - MeasureText("Bureaux de Vote", pf(40)) / 2, py(110), pf(40), BLEU_FONCE);
    int y = py(220);
    if (bouton((Rectangle) { largeur / 2 - px(180), y, px(360), py(55) }, "Ajouter un bureau de vote", BLEU)) {
        vider_form_bv();
        ecran_actuel = ECRAN_AJOUT_BV;
    }
    y += py(70);
    if (bouton((Rectangle) { largeur / 2 - px(180), y, px(360), py(55) }, "Voir la liste des 5 BV", BLEU)) {
        ecran_actuel = ECRAN_BV_RECHERCHEDEPART;
        rapp = 0;
        bv = 1;
    }
    y += py(70);
    if (bouton((Rectangle) { largeur / 2 - px(180), y, px(360), py(55) }, "Rechercher un bureau de vote", BLEU)) {
        vider_form_bv();
        recherche_id[0] = '\0'; recherche_erreur[0] = '\0';
		affich_bv = 1; modif_bv = 0;
        ecran_actuel = ECRAN_BV_RECHERCHE;
    }
    y += py(70);
    if (bouton((Rectangle) { largeur / 2 - px(180), y, px(360), py(55) }, "Modifier un bureau de vote", BLEU)) {
        vider_form_bv();
        recherche_id[0] = '\0'; recherche_erreur[0] = '\0';
		affich_bv = 0; modif_bv = 1;
        ecran_actuel = ECRAN_BV_RECHERCHE;
    }
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_form_bv                                        */
/* DESCRIPTION:           Dessine les 7 champs du formulaire bureau de vote      */
/*                        (utilisé pour l'ajout ET la modification)               */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      01/09/2026                                              */
/* DATE DE MODIFICATION:  07/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_form_bv(void) {
    int x = px(300), y = py(130);
    DrawText("No de rue", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(150), py(36)}, bv_no_rue, sizeof(bv_no_rue), 0, bv_err_no_rue);
    y += py(80);
    DrawText("Rue", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(400), py(36)}, bv_rue, sizeof(bv_rue), 1, bv_err_rue);
    y += py(80);
    DrawText("Ville", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(300), py(36)}, bv_ville, sizeof(bv_ville), 2, bv_err_ville);
    y += py(80);
    DrawText("Departement (EN MAJUSCULE)", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(300), py(36)}, bv_departement, sizeof(bv_departement), 3, bv_err_departement);
    y += py(80);
    DrawText("NINU du president", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(200), py(36)}, bv_president, sizeof(bv_president), 4, bv_err_president);
    if (strlen(bv_president) == 0 && strlen(bv_err_president) == 0)
        DrawText("Pas encore assigne", x, y + py(24) + py(36) + py(4), pf(16), GRIS_INFO);
    y += py(80);
    DrawText("NINU du vice-president", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(200), py(36)}, bv_vpresident, sizeof(bv_vpresident), 5, bv_err_vpresident);
    if (strlen(bv_vpresident) == 0 && strlen(bv_err_vpresident) == 0)
        DrawText("Pas encore assigne", x, y + py(24) + py(36) + py(4), pf(16), GRIS_INFO);
    y += py(80);
    DrawText("NINU du secretaire", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(200), py(36)}, bv_secret, sizeof(bv_secret), 6, bv_err_secret);
    if (strlen(bv_secret) == 0 && strlen(bv_err_secret) == 0)
        DrawText("Pas encore assigne", x, y + py(24) + py(36) + py(4), pf(16), GRIS_INFO);
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              valider_champs_bv                                       */
/* DESCRIPTION:           Vérifie que chaque champ obligatoire est rempli         */   
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      1 si tous les champs sont remplis, 0 sinon              */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  07/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int valider_champs_bv(void) {
    int ok = 1;
    char *msg = "Case vide! Entrer l'information appropriee.";

    bv_err_no_rue[0] = bv_err_rue[0] = bv_err_ville[0] = bv_err_departement[0] = '\0';

    if (strlen(bv_no_rue) == 0) { strcpy(bv_err_no_rue, msg); ok = 0; }
    if (strlen(bv_rue) == 0) { strcpy(bv_err_rue, msg); ok = 0; }
    if (strlen(bv_ville) == 0) { strcpy(bv_err_ville, msg); ok = 0; }
    if (strlen(bv_departement) == 0) { strcpy(bv_err_departement, msg); ok = 0; }
    else if (!departement_valide(bv_departement)) {
        strcpy(bv_err_departement, "Departement invalide.");
        ok = 0;
    }

    return ok;
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              verifier_champ_ninu_bv                                  */
/* DESCRIPTION:           Si la case de NINU President/VPresident/Secret est vide,*/
/*                        la case affiche 0. Sinon on vérifier que le membre est  */
/*                        un potentiel électeur                                   */
/* PARAMETRES:            buffer saisi, buffer d'erreur a remplir, pointeur sur   */
/*                        int Id_President/VPresident/Secret                      */
/* VALEUR DE RETOUR:      1 si vide ou NINU trouve, 0 si NINU invalide            */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      08/09/2026                                              */
/* DATE DE MODIFICATION:  08/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int verifier_champ_ninu_bv(char* buffer, char* err, int* id_saisie) {
    Electeur e;

    err[0] = '\0';

    if (strlen(buffer) == 0) {
        *id_saisie = 0;
        return 1;
    }

    *id_saisie = atoi(buffer);

    if (!lire_elect(ELECT_FILE, *id_saisie, &e)) {
        strcpy(err, "NINU introuvable parmi les electeurs.");
        return 0;
    }

    return 1;
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_ajout_bv                                       */
/* DESCRIPTION:           Écran d'ajout d'un bureau de vote                       */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  11/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_ajout_bv(void) {
    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_MENU_BV;
    char titre[50];
    sprintf(titre, "Ajouter le bureau de vote BV-%04d", obtenir_dernier_id_BV(BV_FILE)+1);
    DrawText(titre, px(450), py(70), pf(40), BLEU_FONCE);
    dessiner_form_bv();
    if (strlen(bv_err_generale) > 0) DrawText(bv_err_generale, px(650), py(880), pf(18), ROUGE_ERREUR);
    if (bouton( (Rectangle) { px(650), py(920), px(200), py(50) }, "Valider", BLEU)) {
        bv_err_generale[0] = '\0';
        if (valider_champs_bv()) {
            BureauVote bv, bv_verif;
			int ok_president, ok_vpresident, ok_secret, id_bv_verif, dernier_id_bv = obtenir_dernier_id_BV(BV_FILE), conflit=0;

            strcpy(bv.Adresse_BV.no_rue, bv_no_rue);
            strcpy(bv.Adresse_BV.rue, bv_rue);
            strcpy(bv.Adresse_BV.ville, bv_ville);
            strcpy(bv.Adresse_BV.departement, bv_departement);

			ok_president = verifier_champ_ninu_bv(bv_president, bv_err_president, &bv.Id_President);
			ok_vpresident = verifier_champ_ninu_bv(bv_vpresident, bv_err_vpresident, &bv.Id_VPresident);
			ok_secret = verifier_champ_ninu_bv(bv_secret, bv_err_secret, &bv.Id_Secret);

            for (id_bv_verif = 1; id_bv_verif <= dernier_id_bv && !conflit; id_bv_verif++) {
                if (!lire_BV(BV_FILE, id_bv_verif, &bv_verif)) continue;
				if (bv_verif.Id_BV == bv.Id_BV) continue;

                if ((bv.Id_President != 0 && (bv.Id_President == bv_verif.Id_President || bv.Id_President == bv_verif.Id_VPresident || bv.Id_President == bv_verif.Id_Secret)) ||
                    (bv.Id_VPresident != 0 && (bv.Id_VPresident == bv_verif.Id_President || bv.Id_VPresident == bv_verif.Id_VPresident || bv.Id_VPresident == bv_verif.Id_Secret)) ||
                    (bv.Id_Secret != 0 && (bv.Id_Secret == bv_verif.Id_President || bv.Id_Secret == bv_verif.Id_VPresident || bv.Id_Secret == bv_verif.Id_Secret))) {
                    conflit = 1;
                }
            }

            if (conflit) {
                strcpy(bv_err_generale, "Deja membre d'un bureau de vote.");
                return;
            }

            if ((bv.Id_President != 0 && bv.Id_President == bv.Id_VPresident) ||
                (bv.Id_President != 0 && bv.Id_President == bv.Id_Secret) ||
                (bv.Id_VPresident != 0 && bv.Id_VPresident == bv.Id_Secret)) {
                strcpy(bv_err_generale, "Membre occupant deja un role dans ce bureau de vote.");
                return;
            }

            if (ok_president && ok_vpresident && ok_secret) {
                if (ajout_BV(&bv)) {
                    vider_form_bv();
                    ecran_actuel = ECRAN_MENU_BV;
                }
                else
                    strcpy(bv_err_generale, "Si case non vide, President/Vice-President/Secretaire introuvable.");
            }
        }
    }
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_liste_bv                                       */
/* DESCRIPTION:           Liste tous les bureaux de vote enregistrés              */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  13/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_liste_bv(void) {
    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_MENU_BV;

    char depart[50];
    snprintf(depart, sizeof(depart), "Informations des bureaux de vote pour %s", NOMS_DEPARTEMENTS[idx_departement]);
    DrawText(depart, px(305), py(70), pf(40), BLEU_FONCE);
    int dernier_id = obtenir_dernier_id_BV(BV_FILE);
    int y = py(140), id;
    BureauVote bv;

    DrawText("ID", px(20), y, pf(20), BLACK);
    DrawText("Adresse", px(125), y, pf(20), BLACK);
    DrawText("President", px(1020), y, pf(20), BLACK);
    DrawText("Vice-President", px(1320), y, pf(20), BLACK);
    DrawText("Secretaire", px(1620), y, pf(20), BLACK);

    char identifiant[10];
    char adresse[150];
    int on_a_trouve = 0;
    DrawLine(px(10), y + py(30), px(1900), y + py(30), BLACK);
    y += py(34);
    for (id = 1; id <= dernier_id; id++) {
        if (lire_BV(BV_FILE, id, &bv) && strcmp(bv.Adresse_BV.departement, NOMS_DEPARTEMENTS[idx_departement]) == 0) {
            snprintf(identifiant, sizeof(identifiant), "BV-%04d", bv.Id_BV);
            snprintf(adresse, sizeof(adresse), "%s, %s, %s, %s", bv.Adresse_BV.no_rue, bv.Adresse_BV.rue, bv.Adresse_BV.ville, bv.Adresse_BV.departement);

            DrawText(identifiant, px(20), y, pf(20), BLACK);
            DrawText(adresse, px(125), y, pf(20), BLACK);
            if (bv.Id_President == 0)
                DrawText("Pas encore assigne", px(1020), y, pf(20), BLACK);
            else {
                snprintf(bv_president, sizeof(bv_president), "%10d", bv.Id_President);
                DrawText(bv_president, px(1020), y, pf(20), BLACK);
            }
            if (bv.Id_VPresident == 0)
                DrawText("Pas encore assigne", px(1320), y, pf(20), BLACK);
            else {
                snprintf(bv_vpresident, sizeof(bv_vpresident), "%10d", bv.Id_VPresident);
                DrawText(bv_vpresident, px(1320), y, pf(20), BLACK);
            }
            if (bv.Id_Secret == 0)
                DrawText("Pas encore assigne", px(1620), y, pf(20), BLACK);
            else {
                snprintf(bv_secret, sizeof(bv_secret), "%10d", bv.Id_Secret);
                DrawText(bv_secret, px(1620), y, pf(20), BLACK);
            }
            y += py(34);
            on_a_trouve = 1;
        }
    }
    if (!on_a_trouve)
		DrawText("Aucun bureau de vote enregistre pour ce departement.", px(300), y, pf(20), GRIS_INFO);
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_bv_recherche                                   */
/* DESCRIPTION:           Écran de recherche d'un bureau de vote par Id_BV avant  */
/*                        modification                                            */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      09/09/2026                                              */
/* DATE DE MODIFICATION:  09/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_bv_recherche(void) {
	logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_MENU_BV;

    DrawText("Rechercher un BV", px(300), py(100), pf(30), BLEU_FONCE);
    DrawText("ID du bureau de vote (numero seul)", px(300), py(200), pf(18), BLACK);
    saisir_champ((Rectangle) { px(300), py(224), px(200), py(36) }, recherche_id, sizeof(recherche_id), 0, recherche_erreur);
    if (bouton((Rectangle) { px(300), py(300), px(200), py(50) }, "Rechercher", BLEU)) {
        recherche_erreur[0] = '\0';

        if (strlen(recherche_id) == 0)
            strcpy(recherche_erreur, "Case vide! Entrer l'information appropriee.");

        else {
            BureauVote bv;
            Electeur e_tem;
            if (lire_BV(BV_FILE, atoi(recherche_id), &bv)) {
                strcpy(bv_no_rue, bv.Adresse_BV.no_rue);
                strcpy(bv_rue, bv.Adresse_BV.rue);
                strcpy(bv_ville, bv.Adresse_BV.ville);
                strcpy(bv_departement, bv.Adresse_BV.departement);
                if (bv.Id_President != 0 && lire_elect(ELECT_FILE, bv.Id_President, &e_tem))
                    snprintf(bv_president, sizeof(bv_president), "%d", bv.Id_President);
                else
                    bv_president[0] = '\0';
                if (bv.Id_VPresident != 0 && lire_elect(ELECT_FILE, bv.Id_VPresident, &e_tem))
                    snprintf(bv_vpresident, sizeof(bv_vpresident), "%d", bv.Id_VPresident);
                else
                    bv_vpresident[0] = '\0';
                if (bv.Id_Secret != 0 && lire_elect(ELECT_FILE, bv.Id_Secret, &e_tem))
                    snprintf(bv_secret, sizeof(bv_secret), "%d", bv.Id_Secret);
                else
                    bv_secret[0] = '\0';

                bv_id_modif = bv.Id_BV;
				bv_err_generale[0] = '\0';
                champ_actif = -1;
				if (affich_bv && !modif_bv)
					ecran_actuel = ECRAN_AFFICHAGE_BV;
				if (modif_bv && !affich_bv)
					ecran_actuel = ECRAN_MODIF_BV;
            }
            else
                strcpy(recherche_erreur, "Aucun bureau de vote trouve avec cet ID.");

        }
    }
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_bv_recherchedepart                             */
/* DESCRIPTION:           Filtre et affiche les bureaux de vote d'un departement  */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      10/09/2026                                              */
/* DATE DE MODIFICATION:  11/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_bv_recherchedepart(void) {
    int largeur = GetScreenWidth();

    logo_coin();
    if (bouton_retour() && bv) ecran_actuel = ECRAN_MENU_BV;
	if (bouton_retour() && rapp) ecran_actuel = ECRAN_MENU_RAPPORTS;

    DrawText("DEPARTEMENTS", largeur / 2 - MeasureText("DEPARTEMENTS", pf(40)) / 2, py(110), pf(40), BLEU_FONCE);

    int y = py(220);
    if (bouton((Rectangle) { largeur / 4 - px(180), y, px(360), py(55) }, "ARTIBONITE", BLEU)) {
		idx_departement = 0;
		if (bv)
			ecran_actuel = ECRAN_LISTE_BV;
        if (rapp)
            ecran_actuel = ECRAN_LISTE_RAPPBV;
    }
	if (bouton((Rectangle) { largeur / 4 - px(180), y + py(70), px(360), py(55) }, "CENTRE", BLEU)) {
        idx_departement = 1;
        if (bv)
            ecran_actuel = ECRAN_LISTE_BV;
		if (rapp)
			ecran_actuel = ECRAN_LISTE_RAPPBV;
	}
	if (bouton((Rectangle) { largeur / 4 - px(180), y + py(140), px(360), py(55) }, "GRAND-ANSE", BLEU)) {
		idx_departement = 2;
        if(bv)
            ecran_actuel = ECRAN_LISTE_BV;
        if(rapp)
            ecran_actuel = ECRAN_LISTE_RAPPBV;
	}
	if (bouton((Rectangle) { largeur / 4 - px(180), y + py(210), px(360), py(55) }, "NIPPES", BLEU)) {
		idx_departement = 3;
		if (bv)
			ecran_actuel = ECRAN_LISTE_BV;
		if (rapp)
			ecran_actuel = ECRAN_LISTE_RAPPBV;
	}
	if (bouton((Rectangle) { largeur / 4 - px(180), y + py(280), px(360), py(55) }, "NORD", BLEU)) {
		idx_departement = 4;
        if (bv)
			ecran_actuel = ECRAN_LISTE_BV;
		if (rapp)
			ecran_actuel = ECRAN_LISTE_RAPPBV;
	}
	if (bouton((Rectangle) { 3 * largeur / 4 - px(180), y, px(360), py(55) }, "NORD-EST", BLEU)) {
		idx_departement = 5;
        if (bv)
			ecran_actuel = ECRAN_LISTE_BV;
		if (rapp)
			ecran_actuel = ECRAN_LISTE_RAPPBV;
	}
	if (bouton((Rectangle) { 3 * largeur / 4 - px(180), y + py(70), px(360), py(55) }, "NORD-OUEST", BLEU)) {
		idx_departement = 6;
        if (bv)
			ecran_actuel = ECRAN_LISTE_BV;
		if (rapp)
			ecran_actuel = ECRAN_LISTE_RAPPBV;
	}
	if (bouton((Rectangle) { 3 * largeur / 4 - px(180), y + py(140), px(360), py(55) }, "OUEST", BLEU)) {
		idx_departement = 7;
        if (bv)
			ecran_actuel = ECRAN_LISTE_BV;
		if (rapp)
			ecran_actuel = ECRAN_LISTE_RAPPBV;
	}
	if (bouton((Rectangle) { 3 * largeur / 4 - px(180), y + py(210), px(360), py(55) }, "SUD", BLEU)) {
		idx_departement = 8;
        if (bv)
			ecran_actuel = ECRAN_LISTE_BV;
		if (rapp)
			ecran_actuel = ECRAN_LISTE_RAPPBV;
	}
	if (bouton((Rectangle) { 3 * largeur / 4 - px(180), y + py(280), px(360), py(55) }, "SUD-EST", BLEU)) {
		idx_departement = 9;
        if (bv)
			ecran_actuel = ECRAN_LISTE_BV;
		if (rapp)
			ecran_actuel = ECRAN_LISTE_RAPPBV;
	}
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_affichage_bv                                   */
/* DESCRIPTION:           Écran d'affichage du bureau de vote recherché pour      */
/*                        lecture uniquement                                      */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  10/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_affichage_bv(void) {
    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_MENU_BV;
    DrawText("Informations detaillees d'un bureau de vote", px(550), py(70), pf(40), BLEU_FONCE);

    int y = py(150);
    DrawText("ID", px(20), y, pf(20), BLACK);
    DrawText("Adresse", px(125), y, pf(20), BLACK);
    DrawText("Id_President", px(1025), y, pf(20), BLACK);
    DrawText("Id_VPresident", px(1330), y, pf(20), BLACK);
    DrawText("Id_Secret", px(1670), y, pf(20), BLACK);
	DrawLine(px(10), y + py(30), px(1900), y + py(30), BLACK);

	DrawText("President", px(35), y + py(100), pf(20), BLACK);
	DrawText("Vice-President", px(635), y + py(100), pf(20), BLACK);
	DrawText("Secretaire", px(1370), y + py(100), pf(20), BLACK);
	DrawLine(px(10), y + py(130), px(1900), y + py(130), BLACK);

    BureauVote bv;
    Electeur e;
    char adresse[150];
    char identifiant[15];
    char p[150], vp[150], sec[150];
    y += py(34);

    if (lire_BV(BV_FILE, bv_id_modif, &bv)) {
        snprintf(identifiant, sizeof(identifiant), "BV-%04d", bv.Id_BV);
        snprintf(adresse, sizeof(adresse), "%s, %s, %s, %s", bv.Adresse_BV.no_rue, bv.Adresse_BV.rue, bv.Adresse_BV.ville, bv.Adresse_BV.departement);
        snprintf(bv_president, sizeof(bv_president), "%010d", bv.Id_BV);
        DrawText(identifiant, px(20), y, pf(20), BLACK);
        DrawText(adresse, px(125), y, pf(20), BLACK);
        if (bv.Id_President == 0) {
            DrawText("Pas encore assigne", px(1025), y, pf(20), BLACK);
            DrawText("Pas encore assigne", px(35), y + py(100), pf(20), BLACK);
        }
        else {
            snprintf(bv_president, sizeof(bv_president), "%10d", bv.Id_President);
            DrawText(bv_president, px(1025), y, pf(20), BLACK);
            if (lire_elect(ELECT_FILE, bv.Id_President, &e)) {
                snprintf(p, sizeof(p), "%s, %s", e.Prenom, e.Nom);
                DrawText(p, px(35), y + py(100), pf(20), BLACK);
            }
        }
        if (bv.Id_VPresident == 0){
            DrawText("Pas encore assigne", px(1330), y, pf(20), BLACK);
            DrawText("Pas encore assigne", px(650), y + py(100), pf(20), BLACK);
        }
        else {
            snprintf(bv_vpresident, sizeof(bv_vpresident), "%10d", bv.Id_VPresident);
            DrawText(bv_vpresident, px(1330), y, pf(20), BLACK);
            if (lire_elect(ELECT_FILE, bv.Id_VPresident, &e)) {
                snprintf(vp, sizeof(vp), "%s, %s", e.Prenom, e.Nom);
                DrawText(vp, px(635), y + py(100), pf(20), BLACK);
            }
        }
        if (bv.Id_Secret == 0) {
            DrawText("Pas encore assigne", px(1670), y, pf(20), BLACK);
            DrawText("Pas encore assigne", px(1370), y + py(100), pf(20), BLACK);
        }
        else {
            snprintf(bv_secret, sizeof(bv_secret), "%10d", bv.Id_Secret);
            DrawText(bv_secret, px(1670), y, pf(20), BLACK);
            if (lire_elect(ELECT_FILE, bv.Id_Secret, &e)) {
                snprintf(sec, sizeof(sec), "%s, %s", e.Prenom, e.Nom);
                DrawText(sec, px(1370), y + py(100), pf(20), BLACK);
            }
        }
    }
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_modif_bv                                       */
/* DESCRIPTION:           Écran de modification d'un bureau de vote par Id_BV     */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  07/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_modif_bv(void) {
    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_MENU_BV;

    char titre[35];
    sprintf(titre, "Modifier le bureau BV-%04d", bv_id_modif);
    DrawText(titre, px(300), py(70), pf(30), BLEU_FONCE);
    dessiner_form_bv();

    if (strlen(bv_err_generale) > 0) 
        DrawText(bv_err_generale, px(300), py(700), pf(18), ROUGE_ERREUR);

    if (bouton((Rectangle) { px(300), py(740), px(200), py(50) }, "Enregistrer", BLEU)) {
        bv_err_generale[0] = '\0';

        if (valider_champs_bv()) {
            BureauVote bv, bv_verif;
            bv.Id_BV = bv_id_modif;
            int ok_president, ok_vpresident, ok_secret, id_bv_verif, dernier_id_bv = obtenir_dernier_id_BV(BV_FILE), conflit = 0;

            strcpy(bv.Adresse_BV.no_rue, bv_no_rue);
            strcpy(bv.Adresse_BV.rue, bv_rue);
            strcpy(bv.Adresse_BV.ville, bv_ville);
            strcpy(bv.Adresse_BV.departement, bv_departement);

            ok_president = verifier_champ_ninu_bv(bv_president, bv_err_president, &bv.Id_President);
            ok_vpresident = verifier_champ_ninu_bv(bv_vpresident, bv_err_vpresident, &bv.Id_VPresident);
            ok_secret = verifier_champ_ninu_bv(bv_secret, bv_err_secret, &bv.Id_Secret);

            for (id_bv_verif = 1; id_bv_verif <= dernier_id_bv && !conflit; id_bv_verif++) {
                if (!lire_BV(BV_FILE, id_bv_verif, &bv_verif)) continue;

                if ((bv.Id_President != 0 && (bv.Id_President == bv_verif.Id_President || bv.Id_President == bv_verif.Id_VPresident || bv.Id_President == bv_verif.Id_Secret)) ||
                    (bv.Id_VPresident != 0 && (bv.Id_VPresident == bv_verif.Id_President || bv.Id_VPresident == bv_verif.Id_VPresident || bv.Id_VPresident == bv_verif.Id_Secret)) ||
                    (bv.Id_Secret != 0 && (bv.Id_Secret == bv_verif.Id_President || bv.Id_Secret == bv_verif.Id_VPresident || bv.Id_Secret == bv_verif.Id_Secret))) {
                    conflit = 1;
                }
            }

            if (conflit) {
                strcpy(bv_err_generale, "Deja membre d'un bureau de vote.");
                return;
            }

            if ((bv.Id_President != 0 && bv.Id_President == bv.Id_VPresident) ||
                (bv.Id_President != 0 && bv.Id_President == bv.Id_Secret) ||
                (bv.Id_VPresident != 0 && bv.Id_VPresident == bv.Id_Secret)) {
                strcpy(bv_err_generale, "Membre occupant deja un role dans ce bureau de vote.");
                return;
            }

            if (ok_president && ok_vpresident && ok_secret) {
                if (modif_BV(&bv)) {
                    vider_form_bv();
                    ecran_actuel = ECRAN_MENU_BV;
                }
                else
                    strcpy(bv_err_generale, "Si case different de 0, President/Vice-President/Secretaire introuvable.");

            }

        }
    }
}


// ============================== **ELECTEURS** ==============================

/*-------------------------------------------------------------------------------*/
/* FONCTION:              vider_form_el                                          */
/* DESCRIPTION:           Réinitialise tous les buffers et erreurs du formulaire */
/*                        electeur                                               */
/* PARAMETRES:            Aucun                                                  */
/* VALEUR DE RETOUR:      Aucune                                                 */
/* AUTEUR:                EMPR                                                   */
/* DATE DE CREATION:      02/09/2026                                             */
/* DATE DE MODIFICATION:  10/09/2026                                             */
/*-------------------------------------------------------------------------------*/
void vider_form_el(void) {
    el_ninu[0] = el_nom[0] = el_prenom[0] = '\0';
    el_no_rue[0] = el_rue[0] = el_ville[0] = el_departement[0] = el_id_bv[0] = '\0';
    el_err_ninu[0] = el_err_nom[0] = el_err_prenom[0] = '\0';
    el_err_no_rue[0] = el_err_rue[0] = el_err_ville[0] = el_err_departement[0] = el_err_id_bv[0] = '\0';
    el_err_generale[0] = '\0';
    champ_actif = -1;
    el_id_modif = 0;
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_form_el                                        */
/* DESCRIPTION:           Dessine les 8 champs du formulaire electeur             */
/*                        (utilisé pour l'ajout ET la modification)               */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  10/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_form_el(void) {
    int x = px(300), y = py(130);
    DrawText("NINU", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(200), py(36)}, el_ninu, sizeof(el_ninu), 0, el_err_ninu);
    y += py(80);
    DrawText("Nom (en majuscule)", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(400), py(36)}, el_nom, sizeof(el_nom), 1, el_err_nom);
    y += py(80);
    DrawText("Prenom", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(400), py(36)}, el_prenom, sizeof(el_prenom), 2, el_err_prenom);
    y += py(80);
    DrawText("No de rue", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(150), py(36)}, el_no_rue, sizeof(el_no_rue), 3, el_err_no_rue);
    y += py(80);
    DrawText("Rue", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(400), py(36)}, el_rue, sizeof(el_rue), 4, el_err_rue);
    y += py(80);
    DrawText("Ville", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(300), py(36)}, el_ville, sizeof(el_ville), 5, el_err_ville);
    y += py(80);
    DrawText("Departement (EN MAJUSCULE)", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(300), py(36)}, el_departement, sizeof(el_departement), 6, el_err_departement);
    y += py(80);
    DrawText("ID du bureau de vote", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(200), py(36)}, el_id_bv, sizeof(el_id_bv), 7, el_err_id_bv);
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              valider_champs_el                                       */
/* DESCRIPTION:           Vérifie que chaque champ obligatoire est rempli         */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      1 si tous les champs sont remplis, 0 sinon              */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  10/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int valider_champs_el(void) {
    int ok = 1;
    char *msg = "Case vide! Entrer l'information appropriee.";
    el_err_ninu[0] = el_err_nom[0] = el_err_prenom[0] = '\0';
    el_err_no_rue[0] = el_err_rue[0] = el_err_ville[0] = el_err_departement[0] = el_err_id_bv[0] = '\0';
    if (strlen(el_ninu) == 0) { strcpy(el_err_ninu, msg); ok = 0; }
    if (strlen(el_nom) == 0) { strcpy(el_err_nom, msg); ok = 0; }
    if (strlen(el_prenom) == 0) { strcpy(el_err_prenom, msg); ok = 0; }
    if (strlen(el_no_rue) == 0) { strcpy(el_err_no_rue, msg); ok = 0; }
    if (strlen(el_rue) == 0) { strcpy(el_err_rue, msg); ok = 0; }
    if (strlen(el_ville) == 0) { strcpy(el_err_ville, msg); ok = 0; }
    if (strlen(el_departement) == 0) { strcpy(el_err_departement, msg); ok = 0; }
    else if (!departement_valide(el_departement)) {
        strcpy(el_err_departement, "Departement invalide.");
        ok = 0;
    }
    if (strlen(el_id_bv) == 0) { strcpy(el_err_id_bv, msg); ok = 0; }
    return ok;
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_menu_electeurs                                 */
/* DESCRIPTION:           Dessine le sous-menu du module Electeurs                */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  10/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_menu_electeurs(void) {
    int largeur = GetScreenWidth();
    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_ACCUEIL;
    DrawText("Electeurs", largeur / 2 - MeasureText("Electeurs", pf(36)) / 2, py(110), pf(36), BLEU_FONCE);
    int y = py(220);
    if (bouton((Rectangle){largeur / 2 - px(180), y, px(360), py(55)}, "Ajouter un electeur", BLEU)) {
        vider_form_el();
        ecran_actuel = ECRAN_AJOUT_ELECTEUR;
    }
    y += py(70);
    if (bouton((Rectangle){largeur / 2 - px(180), y, px(360), py(55)}, "Rechercher un electeur", BLEU)) {
        recherche_erreur[0] = '\0'; recherche_id[0] = '\0';
        ecran_actuel = ECRAN_ELECTEUR_RECHERCHE;
        affich_electeur = 1; modif_electeur = 0;
    }
    y += py(70);
    if (bouton((Rectangle){largeur / 2 - px(180), y, px(360), py(55)}, "Modifier un electeur", BLEU)) {
        vider_form_el();
        recherche_id[0] = '\0'; recherche_erreur[0] = '\0';
        ecran_actuel = ECRAN_ELECTEUR_RECHERCHE;
        modif_electeur = 1; affich_electeur = 0;
    }
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_ajout_electeur                                 */
/* DESCRIPTION:           Écran d'ajout d'un electeur                             */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  10/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_ajout_electeur(void) {
    logo_coin();
    if (bouton_retour()) 
        ecran_actuel = ECRAN_MENU_ELECTEURS;
    DrawText("Ajouter un electeur", px(300), py(70), pf(30), BLEU_FONCE);
    dessiner_form_el();
    if (strlen(el_err_generale) > 0) 
        DrawText(el_err_generale, px(300), py(780), pf(18), ROUGE_ERREUR);
    if (bouton((Rectangle){px(300), py(820), px(200), py(50)}, "Valider", BLEU)) {
        el_err_generale[0] = '\0';

        if (valider_champs_el()) {
            Electeur e;
            e.NINU = atoi(el_ninu);
            strcpy(e.Nom, el_nom);
            strcpy(e.Prenom, el_prenom);
            strcpy(e.Adr.no_rue, el_no_rue);
            strcpy(e.Adr.rue, el_rue);
            strcpy(e.Adr.ville, el_ville);
            strcpy(e.Adr.departement, el_departement);
            e.Id_BV = atoi(el_id_bv);
            if (ins_elect(&e)) {
                vider_form_el();
                ecran_actuel = ECRAN_MENU_ELECTEURS;
            } else
                strcpy(el_err_generale, "Erreur : NINU deja utilise ou Bureau de vote introuvable.");
            
        }
    }
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_electeur_recherche                             */
/* DESCRIPTION:           Écran de recherche d'un electeur par NINU               */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  10/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_electeur_recherche(void) {
    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_MENU_ELECTEURS;

    DrawText("Rechercher un electeur ", px(300), py(100), pf(30), BLEU_FONCE);
    DrawText("NINU de l'electeur", px(300), py(200), pf(18), BLACK);
    saisir_champ((Rectangle) { px(300), py(224), px(200), py(36) }, recherche_id, sizeof(recherche_id), 0, recherche_erreur);
    if (bouton((Rectangle) { px(300), py(300), px(200), py(50) }, "Rechercher", BLEU)) {
        recherche_erreur[0] = '\0';

        if (strlen(recherche_id) == 0)
            strcpy(recherche_erreur, "Case vide! Entrer l'information appropriee.");
        else {
            Electeur e;
            if (lire_elect(ELECT_FILE, atoi(recherche_id), &e)) {
                sprintf(el_ninu, "%d", e.NINU);
                strcpy(el_nom, e.Nom);
                strcpy(el_prenom, e.Prenom);
                strcpy(el_no_rue, e.Adr.no_rue);
                strcpy(el_rue, e.Adr.rue);
                strcpy(el_ville, e.Adr.ville);
                strcpy(el_departement, e.Adr.departement);
                sprintf(el_id_bv, "%d", e.Id_BV);

                el_err_generale[0] = '\0';
                el_id_modif = e.NINU;
                champ_actif = -1;
                if (modif_electeur && !affich_electeur)
                    ecran_actuel = ECRAN_MODIF_ELECTEUR;
                if (affich_electeur && !modif_electeur)
                    ecran_actuel = ECRAN_AFFICHAGE_ELECTEUR;
            }
            else
                strcpy(recherche_erreur, "Aucun electeur trouve avec ce NINU.");
        }
    }
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_affichage_electeur                             */
/* DESCRIPTION:           Écran d'affichage de l'électeur recherché pour          */
/*                        lecture uniquement                                      */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  10/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_affichage_electeur(void) {
    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_MENU_ELECTEURS;
    DrawText("Informations d'un electeur", px(650), py(70), pf(40), BLEU_FONCE);

    int y = py(150);
    DrawText("NINU", px(20), y, pf(20), BLACK);
    DrawText("Nom", px(175), y, pf(20), BLACK);
    DrawText("Prenom", px(525), y, pf(20), BLACK);
    DrawText("Adresse", px(930), y, pf(20), BLACK);
    DrawText("BV", px(1770), y, pf(20), BLACK);

    Electeur e;
    char adresse[150];
    char identifiant1[15];
    char identifiant2[15];
    DrawLine(px(10), y + py(30), px(1900), y + py(30), BLACK);
    y += py(34);

    if (lire_elect(ELECT_FILE, el_id_modif, &e)) {
        snprintf(identifiant1, sizeof(identifiant1), "%d", e.NINU);
        snprintf(adresse, sizeof(adresse), "%s, %s, %s, %s", e.Adr.no_rue, e.Adr.rue, e.Adr.ville, e.Adr.departement);
        snprintf(identifiant2, sizeof(identifiant2), "BV-%03d", e.Id_BV);
        DrawText(identifiant1, px(20), y, pf(20), BLACK);
        DrawText(e.Nom, px(175), y, pf(20), BLACK);
        DrawText(e.Prenom, px(525), y, pf(20), BLACK);
        DrawText(adresse, px(930), y, pf(20), BLACK);
        DrawText(identifiant2, px(1770), y, pf(20), BLACK);
    }
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_modif_electeur                                 */
/* DESCRIPTION:           Écran de modification d'un electeur par NINU            */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  10/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_modif_electeur(void) {
    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_MENU_ELECTEURS;

    char titre[60];
    sprintf(titre, "Modifier l'electeur EL-%010d", el_id_modif);
    DrawText(titre, px(300), py(70), pf(30), BLEU_FONCE);
    dessiner_form_el();
    if (strlen(el_err_generale) > 0) DrawText(el_err_generale, px(300), py(780), pf(18), ROUGE_ERREUR);
    if (bouton((Rectangle){px(300), py(820), px(200), py(50)}, "Enregistrer", BLEU)) {
        el_err_generale[0] = '\0';

        if (valider_champs_el()) {
            Electeur e;
            e.NINU = atoi(el_ninu);
            strcpy(e.Nom, el_nom);
            strcpy(e.Prenom, el_prenom);
            strcpy(e.Adr.no_rue, el_no_rue);
            strcpy(e.Adr.rue, el_rue);
            strcpy(e.Adr.ville, el_ville);
            strcpy(e.Adr.departement, el_departement);
            e.Id_BV = atoi(el_id_bv);
            if (modif_elect(&e)) {
                vider_form_el();
                ecran_actuel = ECRAN_MENU_ELECTEURS;
            }
            else
                strcpy(el_err_generale, "Erreur : Electeur introuvable.");
        }
    }
}


// ============================== **CANDIDATS** ==============================

/*-------------------------------------------------------------------------------*/
/* FONCTION:              vider_form_cd                                          */
/* DESCRIPTION:           Réinitialise tous les buffers et erreurs du formulaire */
/*                        candidat                                               */
/* PARAMETRES:            Aucun                                                  */
/* VALEUR DE RETOUR:      Aucune                                                 */
/* AUTEUR:                EMPR                                                   */
/* DATE DE CREATION:      02/09/2026                                             */
/* DATE DE MODIFICATION:  11/09/2026                                             */
/*-------------------------------------------------------------------------------*/
void vider_form_cd(void) {
    cd_nom[0] = cd_prenom[0] = '\0';
    cd_jour[0] = cd_mois[0] = cd_annee[0] = '\0';
    cd_no_rue[0] = cd_rue[0] = cd_ville[0] = cd_departement[0] = '\0';
    cd_email[0] = cd_telephone[0] = cd_type[0] = '\0';
    cd_err_nom[0] = cd_err_prenom[0] = cd_err_date[0] = '\0';
    cd_err_no_rue[0] = cd_err_rue[0] = cd_err_ville[0] = cd_err_departement[0] = '\0';
    cd_err_email[0] = cd_err_telephone[0] = cd_err_type[0] = cd_err_generale[0] = '\0';
    cd_id_modif = 0;
    champ_actif = -1;
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_form_cd                                        */
/* DESCRIPTION:           Dessine les 12 champs du formulaire candidat            */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  11/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_form_cd(void) {
    int x = px(300), y = py(130);
    DrawText("Nom (en majuscule)", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(300), py(36)}, cd_nom, sizeof(cd_nom), 0, cd_err_nom);
    y += py(80);
    DrawText("Prenom", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(300), py(36)}, cd_prenom, sizeof(cd_prenom), 1, cd_err_prenom);
    y += py(80);
    DrawText("Date de naissance (jour / mois / annee)", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(80), py(36)}, cd_jour, sizeof(cd_jour), 2, cd_err_date);
    saisir_champ((Rectangle){x + px(100), y + py(24), px(80), py(36)}, cd_mois, sizeof(cd_mois), 3, "");
    saisir_champ((Rectangle){x + px(200), y + py(24), px(100), py(36)}, cd_annee, sizeof(cd_annee), 4, "");
    y += py(80);
    DrawText("No de rue", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(150), py(36)}, cd_no_rue, sizeof(cd_no_rue), 5, cd_err_no_rue);
    y += py(80);
    DrawText("Rue", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(300), py(36)}, cd_rue, sizeof(cd_rue), 6, cd_err_rue);
    y += py(80);
    DrawText("Ville", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(300), py(36)}, cd_ville, sizeof(cd_ville), 7, cd_err_ville);
    y += py(80);
    DrawText("Departement (EN MAJUSCULE)", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(300), py(36)}, cd_departement, sizeof(cd_departement), 8, cd_err_departement);
    y += py(80);
    DrawText("Email", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(300), py(36)}, cd_email, sizeof(cd_email), 9, cd_err_email);
    y += py(80);
    DrawText("Telephone", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(250), py(36)}, cd_telephone, sizeof(cd_telephone), 10, cd_err_telephone);
    y += py(80);
    DrawText("Type (0 = independant, sinon ID du parti)", x, y, pf(18), BLACK);
    saisir_champ((Rectangle){x, y + py(24), px(150), py(36)}, cd_type, sizeof(cd_type), 11, cd_err_type);
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              valider_champs_bv                                       */
/* DESCRIPTION:           Vérifie que chaque champ obligatoire est rempli         */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      1 si tous les champs sont remplis, 0 sinon              */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  11/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int valider_champs_cd(void) {
    int ok = 1;
    char *msg = "Case vide! Entrer l'information appropriee.";
    cd_err_nom[0] = cd_err_prenom[0] = cd_err_date[0] = '\0';
    cd_err_no_rue[0] = cd_err_rue[0] = cd_err_ville[0] = cd_err_departement[0] = '\0';
    cd_err_email[0] = cd_err_telephone[0] = cd_err_type[0] = '\0';
    if (strlen(cd_nom) == 0) { strcpy(cd_err_nom, msg); ok = 0; }
    if (strlen(cd_prenom) == 0) { strcpy(cd_err_prenom, msg); ok = 0; }
    if (strlen(cd_jour) == 0 || strlen(cd_mois) == 0 || strlen(cd_annee) == 0) { strcpy(cd_err_date, msg); ok = 0; }
    if (strlen(cd_no_rue) == 0) { strcpy(cd_err_no_rue, msg); ok = 0; }
    if (strlen(cd_rue) == 0) { strcpy(cd_err_rue, msg); ok = 0; }
    if (strlen(cd_ville) == 0) { strcpy(cd_err_ville, msg); ok = 0; }
    if (strlen(cd_departement) == 0) { strcpy(cd_err_departement, msg); ok = 0; }
	else if (!departement_valide(cd_departement)) {
		strcpy(cd_err_departement, "Departement invalide.");
		ok = 0;
	}
    if (strlen(cd_email) == 0) { strcpy(cd_err_email, msg); ok = 0; }
    if (strlen(cd_telephone) == 0) { strcpy(cd_err_telephone, msg); ok = 0; }
    if (strlen(cd_type) == 0) { strcpy(cd_err_type, msg); ok = 0; }
    return ok;
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_menu_candidats                                 */
/* DESCRIPTION:           Dessine le sous-menu du module Candidats                */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  11/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_menu_candidats(void) {
    int largeur = GetScreenWidth();
    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_ACCUEIL;
    DrawText("Candidats", largeur / 2 - MeasureText("Candidats", pf(36)) / 2, py(110), pf(36), BLEU_FONCE);
    int y = py(220);
    if (bouton((Rectangle){largeur / 2 - px(180), y, px(360), py(55)}, "Ajouter un candidat", BLEU)) {
        vider_form_cd();
        ecran_actuel = ECRAN_AJOUT_CANDIDAT;
    }
    y += py(70);
    if (bouton((Rectangle){largeur / 2 - px(180), y, px(360), py(55)}, "Rechercher un candidat", BLEU)) {
        recherche_id[0] = '\0'; recherche_erreur[0] = '\0';
        ecran_actuel = ECRAN_CANDIDAT_RECHERCHE;
        modif_cd = 0; affich_cd = 1;
    }
    y += py(70);
    if (bouton((Rectangle){largeur / 2 - px(180), y, px(360), py(55)}, "Modifier un candidat", BLEU)) {
        vider_form_cd();
        recherche_id[0] = '\0'; recherche_erreur[0] = '\0';
        ecran_actuel = ECRAN_CANDIDAT_RECHERCHE;
        modif_cd = 1; affich_cd = 0;
    }
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_ajout_candidat                                 */
/* DESCRIPTION:           Écran d'ajout d'un candidat                             */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  11/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_ajout_candidat(void) {
    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_MENU_CANDIDATS;

    char titre[50];
    sprintf(titre, "Ajouter le candidat CD-%03d", obtenir_dernier_id_candid(CANDID_FILE) + 1);
    DrawText(titre, px(650), py(70), pf(40), BLEU_FONCE);
    dessiner_form_cd();
    if (strlen(cd_err_generale) > 0) 
        DrawText(cd_err_generale, px(300), py(945), pf(18), ROUGE_ERREUR);
    
    if (bouton((Rectangle){px(300), py(970), px(200), py(50)}, "Valider", BLEU)) {
        cd_err_generale[0] = '\0';
        
        if (valider_champs_cd()) {
            Candidat c;
            strcpy(c.Nom, cd_nom);
            strcpy(c.Prenom, cd_prenom);
			c.Dat_Nais.jour = atoi(cd_jour);
			c.Dat_Nais.mois = atoi(cd_mois);
			c.Dat_Nais.annee = atoi(cd_annee);
            strcpy(c.Adr.no_rue, cd_no_rue);
            strcpy(c.Adr.rue, cd_rue);
            strcpy(c.Adr.ville, cd_ville);
            strcpy(c.Adr.departement, cd_departement);
            strcpy(c.Email, cd_email);
            strcpy(c.Telephone, cd_telephone);
			c.type = atoi(cd_type);
            if (ins_cand(&c)) {
                vider_form_cd();
                ecran_actuel = ECRAN_MENU_CANDIDATS;
            } 
            else
                strcpy(cd_err_generale, "Erreur : Candidat de moins de 30 ans/Date invalide/Parti introuvable.");
        }
    }
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_affichage_candidat                             */
/* DESCRIPTION:           Écran d'affichage d'un candidat pour                    */
/*                        lecture uniquement                                      */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  11/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_affichage_candidat(void) {
    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_MENU_CANDIDATS;
    DrawText("Informations d'un candidat", px(650), py(70), pf(40), BLEU_FONCE);
    int y = py(140);
    Candidat c;
    PartiPolitique pp;
    char adresse[150];
    char identifiant[10];
    char date_nais[25];
    char date_ins[25];
    char type[60];
    char ind[25] = "Independant(e)";

    DrawLine(px(10), y + py(30), px(1900), y + py(30), BLACK);
    DrawText("ID", px(20), y, pf(20), BLACK);
    DrawText("Nom", px(210), y, pf(20), BLACK);
    DrawText("Prenom", px(510), y, pf(20), BLACK);
    DrawText("Date Nais", px(850), y, pf(20), BLACK);
    DrawText("Date inscription", px(1150), y, pf(20), BLACK);

    DrawLine(px(10), y + py(130), px(1900), y + py(130), BLACK);
    DrawText("Adresse", px(30), y + py(100), pf(20), BLACK);
    DrawText("Type", px(560), y + py(100), pf(20), BLACK);
    DrawText("Téléphone", px(850), y + py(100), pf(20), BLACK);
    DrawText("Email", px(1150), y + py(100), pf(20), BLACK);
	y += py(34);

    if (lire_candid(CANDID_FILE, cd_id_modif, &c)) {
        snprintf(identifiant, sizeof(identifiant), "CD-%03d", c.Id_candid);
        snprintf(adresse, sizeof(adresse), "%s, %s, %s, %s", c.Adr.no_rue, c.Adr.rue, c.Adr.ville, c.Adr.departement);
        snprintf(date_nais, sizeof(date_nais), "%02d/%02d/%04d", c.Dat_Nais.jour, c.Dat_Nais.mois, c.Dat_Nais.annee);
        snprintf(date_ins, sizeof(date_ins), "%02d/%02d/%04d", c.Date_ins.jour, c.Date_ins.mois, c.Date_ins.annee);
        if (lire_PP(PP_FILE, c.type, &pp))
            snprintf(type, sizeof(type), "%s", pp.Nom);
        else
            snprintf(type, sizeof(type), "%s", ind);
        
        DrawText(identifiant, px(20), y, pf(20), BLACK);
        DrawText(c.Nom, px(210), y, pf(20), BLACK);
        DrawText(c.Prenom, px(510), y, pf(20), BLACK);
        DrawText(date_nais, px(850), y, pf(20), BLACK);
        DrawText(date_ins, px(1150), y, pf(20), BLACK);

        DrawText(adresse, px(30), y + py(100), pf(20), BLACK);
        DrawText(type, px(560), y + py(100), pf(20), BLACK);
        DrawText(c.Telephone, px(850), y + py(100), pf(20), BLACK);
        DrawText(c.Email, px(1150), y +py(100), pf(20), BLACK);
    }
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_candidat_recherche                             */
/* DESCRIPTION:           Écran de recherche d'un candidat par ID                 */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      09/09/2026                                              */
/* DATE DE MODIFICATION:  11/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_candidat_recherche(void) {
    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_MENU_CANDIDATS;

    DrawText("Rechercher un candidat", px(300), py(100), pf(30), BLEU_FONCE);
    DrawText("ID du candidat", px(300), py(200), pf(18), BLACK);
    saisir_champ((Rectangle) { px(300), py(224), px(200), py(36) }, recherche_id, sizeof(recherche_id), 0, recherche_erreur);
    if (bouton((Rectangle) { px(300), py(300), px(200), py(50) }, "Rechercher", BLEU)) {
        recherche_erreur[0] = '\0';

        if (strlen(recherche_id) == 0)
            strcpy(recherche_erreur, "Case vide! Entrer l'information appropriee.");
        else {
            Candidat c;
            if (lire_candid(CANDID_FILE, atoi(recherche_id), &c)) {
				sprintf(cd_id, "%d", c.Id_candid);
                strcpy(cd_nom, c.Nom);
                strcpy(cd_prenom, c.Prenom);
				sprintf(cd_jour, "%d", c.Dat_Nais.jour);
				sprintf(cd_mois, "%d", c.Dat_Nais.mois);
				sprintf(cd_annee, "%d", c.Dat_Nais.annee);
                strcpy(cd_no_rue, c.Adr.no_rue);
                strcpy(cd_rue, c.Adr.rue);
                strcpy(cd_ville, c.Adr.ville);
                strcpy(cd_departement, c.Adr.departement);
                strcpy(cd_email, c.Email);
                strcpy(cd_telephone, c.Telephone);
				sprintf(cd_type, "%d", c.type);

                cd_err_generale[0] = '\0';
                cd_id_modif = c.Id_candid;
                champ_actif = -1;
                if (modif_cd && !affich_cd)
                    ecran_actuel = ECRAN_MODIF_CANDIDAT;
                if (affich_cd && !modif_cd)
                    ecran_actuel = ECRAN_AFFICHAGE_CANDIDAT;
            }
            else
                strcpy(recherche_erreur, "Aucun candidat trouve avec cet ID.");
        }
    }
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_modif_candidat                                 */
/* DESCRIPTION:           Écran de recherche d'un candidat par ID pour            */
/*                        modification                                            */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  11/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_modif_candidat(void) {
    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_MENU_CANDIDATS;

    char titre[30];
    sprintf(titre, "Modifier le candidat CD-%03d", cd_id_modif);
    DrawText(titre, px(300), py(70), pf(30), BLEU_FONCE);
    dessiner_form_cd();
    if (strlen(cd_err_generale) > 0) 
        DrawText(cd_err_generale, px(300), py(945), pf(18), ROUGE_ERREUR);
    if (bouton((Rectangle) { px(300), py(970), px(200), py(50) }, "Enregistrer", BLEU)) {
        cd_err_generale[0] = '\0';

        if (valider_champs_cd()) {
            Candidat c;
            c.Id_candid = atoi(cd_id);
            strcpy(c.Nom, cd_nom);
            strcpy(c.Prenom, cd_prenom);
			c.Dat_Nais.jour = atoi(cd_jour);
			c.Dat_Nais.mois = atoi(cd_mois);
			c.Dat_Nais.annee = atoi(cd_annee);
            strcpy(c.Adr.no_rue, cd_no_rue);
            strcpy(c.Adr.rue, cd_rue);
            strcpy(c.Adr.ville, cd_ville);
            strcpy(c.Adr.departement, cd_departement);
            strcpy(c.Email, cd_email);
            strcpy(c.Telephone, cd_telephone);
			c.type = atoi(cd_type);
            if (modif_cand(&c)) {
                vider_form_el();
                ecran_actuel = ECRAN_MENU_CANDIDATS;
            }
            else
                strcpy(cd_err_generale, "Erreur : Candidat de moins de 30 ans/Date invalide/Parti introuvable.");
        }
    }
}


// ============================== **VOTES** ==============================

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_menu_votes                                     */
/* DESCRIPTION:           Dessine le sous-menu du module Votes                    */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  07/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_menu_votes(void) {
    int largeur = GetScreenWidth();
    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_ACCUEIL;
    DrawText("Votes", largeur / 2 - MeasureText("Votes", pf(36)) / 2, py(110), pf(36), BLEU_FONCE);
    if (bouton((Rectangle){largeur / 2 - px(180), py(220), px(360), py(55)}, "Enregistrer un vote", BLEU)) {
        vt_ninu[0] = vt_id_candid[0] = '\0';
        vt_err_ninu[0] = vt_err_generale[0] = vt_confirmation[0] = '\0';
        champ_actif = -1;
        ecran_actuel = ECRAN_ENREGISTRER_VOTE;
    }
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_enregistrer_vote                               */
/* DESCRIPTION:           Dessine le formulaire de vote et enregistre le vote     */
/*                        d'un electeur                                           */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  07/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_enregistrer_vote(void) {
    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_MENU_VOTES;
    DrawText("Enregistrer un vote", px(300), py(70), pf(30), BLEU_FONCE);

    DrawText("NINU de l'electeur", px(300), py(150), pf(18), BLACK);
    saisir_champ((Rectangle){px(300), py(174), px(200), py(36)}, vt_ninu, sizeof(vt_ninu), 0, vt_err_ninu);

    DrawText("ID du candidat choisi (0 pour un vote blanc)", px(300), py(260), pf(18), BLACK);
    saisir_champ((Rectangle){px(300), py(284), px(200), py(36)}, vt_id_candid, sizeof(vt_id_candid), 1, "");

    if (strlen(vt_err_generale) > 0) DrawText(vt_err_generale, px(300), py(360), pf(18), ROUGE_ERREUR);
    if (strlen(vt_confirmation) > 0) DrawText(vt_confirmation, px(300), py(360), pf(18), DARKGREEN);

    if (bouton((Rectangle){px(300), py(400), px(200), py(50)}, "Voter", BLEU)) {
        vt_err_generale[0] = '\0';
        vt_confirmation[0] = '\0';
        vt_err_ninu[0] = '\0';

        if (strlen(vt_ninu) == 0) {
            strcpy(vt_err_ninu, "Case vide! Entrer l'information appropriee.");
        } else {
            Vote v;
            v.NINU = atoi(vt_ninu);
            v.Id_candid = (strlen(vt_id_candid) == 0) ? 0 : atoi(vt_id_candid);

            if (ins_vote(&v)) {
                sprintf(vt_confirmation, "Vote enregistre avec succes (VT-%010d).", v.Id_vote);
                vt_ninu[0] = vt_id_candid[0] = '\0';
            } else {
                strcpy(vt_err_generale, "Erreur : electeur introuvable/candidat introuvable/electeur ayant deja vote.");
            }
        }
    }
}

// ============================== RAPPORTS & RESULTATS ==============================

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_menu_rapports                                  */
/* DESCRIPTION:           Dessine le sous-menu du module Rapports et Resultats    */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  13/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_menu_rapports(void) {
    int largeur = GetScreenWidth();
    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_ACCUEIL;
    DrawText("Rapports et Resultats", largeur / 2 - MeasureText("Rapports et Resultats", pf(36)) / 2, py(110), pf(36), BLEU_FONCE);
    if (bouton((Rectangle){largeur / 2 - px(180), py(220), px(360), py(55)}, "Liste des Electeurs", BLEU)) {
        ecran_actuel = ECRAN_EL_RECHERCHEDEPART;
    }
	if (bouton((Rectangle) { largeur / 2 - px(180), py(300), px(360), py(55) }, "Liste des Candidats", BLEU)) {
		ecran_actuel = ECRAN_LISTE_CANDIDAT;
	}
	if (bouton((Rectangle) { largeur / 2 - px(180), py(380), px(360), py(55) }, "Liste des BV", BLEU)) {
        ecran_actuel = ECRAN_BV_RECHERCHEDEPART;
        rapp = 1;
        bv = 0;
	}
	if (bouton((Rectangle) { largeur / 2 - px(180), py(460), px(360), py(55) }, "Resultats", BLEU)) {
		ecran_actuel = ECRAN_RESULTATS;
	}
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_menu_resultats                                 */
/* DESCRIPTION:           Dessine le sous-menu du module Resultats                */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  13/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_resultats(void) {
    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_MENU_RAPPORTS;
    DrawText("Resultats", px(300), py(60), pf(30), BLEU_FONCE);

    ResultatDeptCandidat aggregat[MAX_RESULTATS];
    ResultatDept aggregat_dept[MAX_RESULTATS];
    int n_aggregat = 0, n_dept = 0;
    int i, j, trouve_dept, trouve_agg;
    Vote v;
    BureauVote bv;

    FILE *fp = fopen(VOTES_FILE, "r");
    if (fp != NULL) {
        while (fscanf(fp, "%d", &v.Id_vote) == 1) {
            fgetc(fp);
            fscanf(fp, "%d %d %d", &v.Date_vote.jour, &v.Date_vote.mois, &v.Date_vote.annee);
            fgetc(fp);
            fscanf(fp, "%d %d %d", &v.Id_BV, &v.NINU, &v.Id_candid);
            fgetc(fp);

            if (!lire_BV(BV_FILE, v.Id_BV, &bv)) continue;

            trouve_dept = 0;
            for (i = 0; i < n_dept; i++) {
                if (strcmp(aggregat_dept[i].departement, bv.Adresse_BV.departement) == 0) { trouve_dept = 1; break; }
            }
            if (!trouve_dept) {
                strcpy(aggregat_dept[n_dept].departement, bv.Adresse_BV.departement);
                aggregat_dept[n_dept].total_blancs = 0;
                aggregat_dept[n_dept].total_non_blancs = 0;
                i = n_dept;
                n_dept++;
            }

            if (v.Id_candid == 0) {
                aggregat_dept[i].total_blancs++;
            } else {
                aggregat_dept[i].total_non_blancs++;
                trouve_agg = 0;
                for (j = 0; j < n_aggregat; j++) {
                    if (strcmp(aggregat[j].departement, bv.Adresse_BV.departement) == 0 && aggregat[j].id_candid == v.Id_candid) { trouve_agg = 1; break; }
                }
                if (!trouve_agg) {
                    strcpy(aggregat[n_aggregat].departement, bv.Adresse_BV.departement);
                    aggregat[n_aggregat].id_candid = v.Id_candid;
                    aggregat[n_aggregat].total_votes = 0;
                    j = n_aggregat;
                    n_aggregat++;
                }
                aggregat[j].total_votes++;
            }
        }
        fclose(fp);
    }

    int y = py(110);
    Candidat c;
    char ligne[220];

    for (i = 0; i < n_dept; i++) {
        int total = aggregat_dept[i].total_non_blancs + aggregat_dept[i].total_blancs;
        sprintf(ligne, "Departement: %s (non blancs: %d, blancs: %d)",
                aggregat_dept[i].departement, aggregat_dept[i].total_non_blancs, aggregat_dept[i].total_blancs);
        DrawText(ligne, px(300), y, pf(20), BLEU_FONCE);
        y += py(30);

        for (j = 0; j < n_aggregat; j++) {
            if (strcmp(aggregat[j].departement, aggregat_dept[i].departement) == 0) {
                float pct = (total > 0) ? (100.0f * aggregat[j].total_votes / total) : 0.0f;
                if (lire_candid(CANDID_FILE, aggregat[j].id_candid, &c)) {
                    sprintf(ligne, "   %s %s : %d votes (%.2f%%)", c.Nom, c.Prenom, aggregat[j].total_votes, pct);
                    DrawText(ligne, px(300), y, pf(18), BLACK);
                    y += py(26);
                }
            }
        }
        y += py(10);
    }

    if (n_dept == 0) DrawText("Aucun vote enregistre.", px(300), y, pf(20), GRIS_INFO);
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_liste_rappbv                                   */
/* DESCRIPTION:           Écran d'affichage du bureau de vote recherché pour      */
/*                        lecture uniquement                                      */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  13/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_liste_rappbv(void) {
    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_MENU_RAPPORTS;
    char depart[50];
    snprintf(depart, sizeof(depart), "Informations des bureaux de vote pour %s", NOMS_DEPARTEMENTS[idx_departement]);
    DrawText(depart, px(305), py(70), pf(40), BLEU_FONCE);

    int y = py(150);
    int dernier_id = obtenir_dernier_id_BV(BV_FILE), on_a_trouve = 0;
    DrawText("ID", px(20), y, pf(20), BLACK);
    DrawText("Adresse", px(125), y, pf(20), BLACK);
    DrawText("President", px(835), y, pf(20), BLACK);
    DrawText("Vice-President", px(1135), y, pf(20), BLACK);
    DrawText("Secretaire", px(1470), y, pf(20), BLACK);
    DrawLine(px(10), y + py(30), px(1900), y + py(30), BLACK);

    BureauVote bv;
    Electeur e;
    char adresse[150];
    char identifiant[15];
    char p[150], vp[150], sec[150];
    y += py(34);

    for (int i = 1; i <= dernier_id; i++) {
        if (lire_BV(BV_FILE, i, &bv) && strcmp(bv.Adresse_BV.departement, NOMS_DEPARTEMENTS[idx_departement]) == 0) {
            snprintf(identifiant, sizeof(identifiant), "BV-%04d", bv.Id_BV);
            snprintf(adresse, sizeof(adresse), "%s, %s, %s, %s", bv.Adresse_BV.no_rue, bv.Adresse_BV.rue, bv.Adresse_BV.ville, bv.Adresse_BV.departement);
            DrawText(identifiant, px(20), y, pf(20), BLACK);
            DrawText(adresse, px(125), y, pf(20), BLACK);
            if (bv.Id_President == 0)
                DrawText("Pas encore assigne", px(835), y, pf(20), BLACK);
            else {
                if (lire_elect(ELECT_FILE, bv.Id_President, &e)) {
                    snprintf(p, sizeof(p), "%s, %s", e.Prenom, e.Nom);
                    DrawText(p, px(835), y, pf(20), BLACK);
                }
            }
            if (bv.Id_VPresident == 0)
                DrawText("Pas encore assigne", px(1135), y, pf(20), BLACK);
            else {
                if (lire_elect(ELECT_FILE, bv.Id_VPresident, &e)) {
                    snprintf(vp, sizeof(vp), "%s, %s", e.Prenom, e.Nom);
                    DrawText(vp, px(1135), y, pf(20), BLACK);
                }
            }
            if (bv.Id_Secret == 0)
                DrawText("Pas encore assigne", px(1470), y, pf(20), BLACK);
            else {
                if (lire_elect(ELECT_FILE, bv.Id_Secret, &e)) {
                    snprintf(sec, sizeof(sec), "%s, %s", e.Prenom, e.Nom);
                    DrawText(sec, px(1470), y, pf(20), BLACK);
                }
            }
            on_a_trouve = 1;
            y += py(34);
        }
    }
    if (!on_a_trouve)
        DrawText("Aucun bureau de vote pour ce departement", px(300), y, pf(20), GRIS_INFO);
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_liste_candidat                                 */
/* DESCRIPTION:           Écran d'affichage des candidats pour                    */
/*                        lecture uniquement                                      */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  13/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_liste_candidat(void) {
    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_MENU_RAPPORTS;
    DrawText("Informations des candidats", px(650), py(70), pf(40), BLEU_FONCE);
    int y = py(140), on_a_trouve = 0;
    Candidat c;
    PartiPolitique pp;
    char identifiant[10];
    char type[60];
    char ind[25] = "Independant(e)";
    int dernier_id = obtenir_dernier_id_candid(CANDID_FILE);

    DrawLine(px(10), y + py(30), px(1900), y + py(30), BLACK);
    DrawText("ID", px(20), y, pf(20), BLACK);
    DrawText("Nom", px(210), y, pf(20), BLACK);
    DrawText("Prenom", px(610), y, pf(20), BLACK);
    DrawText("Type", px(1150), y, pf(20), BLACK);
    y += py(34);

    for (int i = 1;i <= dernier_id;i++) {
        if (lire_candid(CANDID_FILE, i, &c)) {
            snprintf(identifiant, sizeof(identifiant), "CD-%03d", c.Id_candid);
            if (lire_PP(PP_FILE, c.type, &pp))
                snprintf(type, sizeof(type), "%s", pp.Nom);
            else
                snprintf(type, sizeof(type), "%s", ind);

            DrawText(identifiant, px(20), y, pf(20), BLACK);
            DrawText(c.Nom, px(210), y, pf(20), BLACK);
            DrawText(c.Prenom, px(610), y, pf(20), BLACK);
            DrawText(type, px(1150), y, pf(20), BLACK);
            on_a_trouve = 1;
            y += py(34);
        }
    }
    if(!on_a_trouve)
		DrawText("Aucun candidat enregistre.", px(300), y, pf(20), GRIS_INFO);
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_el_recherchedepart                             */
/* DESCRIPTION:           Filtre et affiche les électeurs d'un departement        */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      10/09/2026                                              */
/* DATE DE MODIFICATION:  14/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_el_recherchedepart(void) {
    int largeur = GetScreenWidth();

    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_MENU_RAPPORTS;

    DrawText("DEPARTEMENTS", largeur / 2 - MeasureText("DEPARTEMENTS", pf(40)) / 2, py(110), pf(40), BLEU_FONCE);

    int y = py(220);
    if (bouton((Rectangle) { largeur / 4 - px(180), y, px(360), py(55) }, "ARTIBONITE", BLEU)) {
        idx_departement = 0;
        ecran_actuel = ECRAN_LISTE_ELECTEUR;
    }
    if (bouton((Rectangle) { largeur / 4 - px(180), y + py(70), px(360), py(55) }, "CENTRE", BLEU)) {
        idx_departement = 1;
        ecran_actuel = ECRAN_LISTE_ELECTEUR;
    }
    if (bouton((Rectangle) { largeur / 4 - px(180), y + py(140), px(360), py(55) }, "GRAND-ANSE", BLEU)) {
        idx_departement = 2;
        ecran_actuel = ECRAN_LISTE_ELECTEUR;
    }
    if (bouton((Rectangle) { largeur / 4 - px(180), y + py(210), px(360), py(55) }, "NIPPES", BLEU)) {
        idx_departement = 3;
        ecran_actuel = ECRAN_LISTE_ELECTEUR;
    }
    if (bouton((Rectangle) { largeur / 4 - px(180), y + py(280), px(360), py(55) }, "NORD", BLEU)) {
        idx_departement = 4;
        ecran_actuel = ECRAN_LISTE_ELECTEUR;
    }
    if (bouton((Rectangle) { 3 * largeur / 4 - px(180), y, px(360), py(55) }, "NORD-EST", BLEU)) {
        idx_departement = 5;
        ecran_actuel = ECRAN_LISTE_ELECTEUR;
    }
    if (bouton((Rectangle) { 3 * largeur / 4 - px(180), y + py(70), px(360), py(55) }, "NORD-OUEST", BLEU)) {
        idx_departement = 6;
        ecran_actuel = ECRAN_LISTE_ELECTEUR;
    }
    if (bouton((Rectangle) { 3 * largeur / 4 - px(180), y + py(140), px(360), py(55) }, "OUEST", BLEU)) {
        idx_departement = 7;
        ecran_actuel = ECRAN_LISTE_ELECTEUR;
    }
    if (bouton((Rectangle) { 3 * largeur / 4 - px(180), y + py(210), px(360), py(55) }, "SUD", BLEU)) {
        idx_departement = 8;
        ecran_actuel = ECRAN_LISTE_ELECTEUR;
    }
    if (bouton((Rectangle) { 3 * largeur / 4 - px(180), y + py(280), px(360), py(55) }, "SUD-EST", BLEU)) {
        idx_departement = 9;
        ecran_actuel = ECRAN_LISTE_ELECTEUR;
    }
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_liste_electeur                                 */
/* DESCRIPTION:           Écran d'affichage des electeurs recherché pour          */
/*                        lecture uniquement                                      */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      07/09/2026                                              */
/* DATE DE MODIFICATION:  13/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_liste_electeur(void) {
    logo_coin();
    if (bouton_retour()) ecran_actuel = ECRAN_MENU_RAPPORTS;
    char depart[50];
    snprintf(depart, sizeof(depart), "Informations des electeurs pour %s", NOMS_DEPARTEMENTS[idx_departement]);
    DrawText(depart, px(305), py(70), pf(40), BLEU_FONCE);

    int y = py(150);
    int dernier_id = obtenir_dernier_id_BV(BV_FILE), on_a_trouve = 0;
    DrawText("NINU", px(20), y, pf(20), BLACK);
    DrawText("Nom", px(325), y, pf(20), BLACK);
    DrawText("Prenom", px(510), y, pf(20), BLACK);
    DrawText("Adresse", px(950), y, pf(20), BLACK);
	DrawText("BV", px(1700), y, pf(20), BLACK);
    DrawLine(px(10), y + py(30), px(1900), y + py(30), BLACK);

    BureauVote bv;
    Electeur e;
    char adresse[150];
    char identifiant[15];
    char id[15];
    y += py(34);

    for (int i = 1; i <= dernier_id; i++) {
        if (lire_BV(BV_FILE, i, &bv) && strcmp(bv.Adresse_BV.departement, NOMS_DEPARTEMENTS[idx_departement]) == 0){
            FILE* fp = fopen(ELECT_FILE, "r");
            if (fp == NULL) continue;
            while (fscanf(fp, "%d", &e.NINU) == 1) {
                fgetc(fp);

                fgets(e.Nom, sizeof(e.Nom), fp);
                e.Nom[strlen(e.Nom) - 1] = '\0';

                fgets(e.Prenom, sizeof(e.Prenom), fp);
                e.Prenom[strlen(e.Prenom) - 1] = '\0';

                fgets(e.Adr.no_rue, sizeof(e.Adr.no_rue), fp);
                e.Adr.no_rue[strlen(e.Adr.no_rue) - 1] = '\0';

                fgets(e.Adr.rue, sizeof(e.Adr.rue), fp);
                e.Adr.rue[strlen(e.Adr.rue) - 1] = '\0';

                fgets(e.Adr.ville, sizeof(e.Adr.ville), fp);
                e.Adr.ville[strlen(e.Adr.ville) - 1] = '\0';

                fgets(e.Adr.departement, sizeof(e.Adr.departement), fp);
                e.Adr.departement[strlen(e.Adr.departement) - 1] = '\0';

                (void)fscanf(fp, "%d", &e.Id_BV);
                fgetc(fp);

                if (e.Id_BV == i) {
					snprintf(identifiant, sizeof(identifiant), "EL-%010d", e.NINU);
					snprintf(adresse, sizeof(adresse), "%s, %s, %s, %s", e.Adr.no_rue, e.Adr.rue, e.Adr.ville, e.Adr.departement);
					snprintf(id, sizeof(id), "BV-%04d", e.Id_BV);
					DrawText(identifiant, px(20), y, pf(20), BLACK);
					DrawText(e.Nom, px(325), y, pf(20), BLACK);
					DrawText(e.Prenom, px(510), y, pf(20), BLACK);
					DrawText(adresse, px(950), y, pf(20), BLACK);
                    DrawText(id, px(1700), y, pf(20), BLACK);

                    y += py(34);
                    on_a_trouve = 1;
                }
            }
            fclose(fp);
        }
    }
    if (!on_a_trouve)
        DrawText("Aucun electeur pour ce departement", px(300), y, pf(20), GRIS_INFO);
}



/*--------------------------------------------------------------------------------*/
/* FONCTION:              dessiner_fond                                           */
/* DESCRIPTION:           Dessine l'image de fond                                 */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      03/09/2026                                              */
/* DATE DE MODIFICATION:  05/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void dessiner_fond(void) {
    int largeur_ecran = GetScreenWidth();
    int hauteur_ecran = GetScreenHeight();

    float scale = fmaxf(
        (float)largeur_ecran / fond.width,
        (float)hauteur_ecran / fond.height
    );

    float newWidth = fond.width * scale;
    float newHeight = fond.height * scale;
    float x = (largeur_ecran - newWidth) / 2;
    float y = (hauteur_ecran - newHeight) / 2;

    Rectangle src = {0.0f, 0.0f, (float)fond.width, (float)fond.height};
    Rectangle dest = {x, y, newWidth, newHeight};
    Vector2 origin = {0.0f, 0.0f};

    DrawTexturePro(fond, src, dest, origin, 0.0f, WHITE);
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              main                                                    */
/* DESCRIPTION:           Ouvre la fenêtre Raylib (taille = ecran du moniteur) et */
/*                        boucle principale : dessine l'écran courant             */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      0                                                       */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      02/09/2026                                              */
/* DATE DE MODIFICATION:  03/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int main(void) {
    InitWindow(800, 450, "CEP");
    int moniteur = GetCurrentMonitor();
    int largeur_ecran = GetMonitorWidth(moniteur);
    int hauteur_ecran = GetMonitorHeight(moniteur);
    CloseWindow();

    InitWindow(largeur_ecran, hauteur_ecran, "CEP - Conseil Electoral Provisoire");
    SetWindowPosition(0, 0);
    SetTargetFPS(60);

    echelle_x = largeur_ecran / (float)LARGEUR_REF;
    echelle_y = hauteur_ecran / (float)HAUTEUR_REF;

    fond = LoadTexture(BACKGROUND);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);
        dessiner_fond();

        switch (ecran_actuel) {
            case ECRAN_ACCUEIL:
                dessiner_accueil();
                break;
            case ECRAN_MENU_PARTI:
                dessiner_menu_parti();
                break;
            case ECRAN_AJOUT_PARTI:
                dessiner_ajout_parti();
                break;
            case ECRAN_LISTE_PARTI:
                dessiner_liste_parti();
                break;
            case ECRAN_PARTI_RECHERCHE:
                dessiner_parti_recherche();
                break;
            case ECRAN_AFFICHAGE_PARTI:
                dessiner_affichage_parti();
                break;
            case ECRAN_MODIF_PARTI:
                dessiner_modif_parti();
                break;
            case ECRAN_MENU_BV: 
                dessiner_menu_bv(); 
                break;
            case ECRAN_AJOUT_BV: 
                dessiner_ajout_bv(); 
                break;
			case ECRAN_BV_RECHERCHEDEPART:
				dessiner_bv_recherchedepart();
				break;
            case ECRAN_LISTE_BV:
                dessiner_liste_bv();
                break;
            case ECRAN_BV_RECHERCHE:
                dessiner_bv_recherche();
                break;
            case ECRAN_AFFICHAGE_BV:
                dessiner_affichage_bv();
                break;
            case ECRAN_MODIF_BV:
                dessiner_modif_bv(); 
                break;
            case ECRAN_MENU_ELECTEURS: 
                dessiner_menu_electeurs(); 
                break;
            case ECRAN_AJOUT_ELECTEUR: 
                dessiner_ajout_electeur(); 
                break;
            case ECRAN_ELECTEUR_RECHERCHE:
                dessiner_electeur_recherche();
                break;
            case ECRAN_AFFICHAGE_ELECTEUR:
                dessiner_affichage_electeur();
                break;
            case ECRAN_MODIF_ELECTEUR: 
                dessiner_modif_electeur(); 
                break;
            case ECRAN_MENU_CANDIDATS: 
                dessiner_menu_candidats();
                break;
            case ECRAN_AJOUT_CANDIDAT: 
                dessiner_ajout_candidat(); 
                break;
            case ECRAN_CANDIDAT_RECHERCHE:
                dessiner_candidat_recherche();
                break;
            case ECRAN_AFFICHAGE_CANDIDAT:
                dessiner_affichage_candidat();
                break;
            case ECRAN_MODIF_CANDIDAT: 
                dessiner_modif_candidat(); 
                break;
            case ECRAN_MENU_VOTES: 
                dessiner_menu_votes(); 
                break;
            case ECRAN_ENREGISTRER_VOTE: 
                dessiner_enregistrer_vote();
                break;
            case ECRAN_MENU_RAPPORTS: 
                dessiner_menu_rapports();
                break;
            case ECRAN_EL_RECHERCHEDEPART:
				dessiner_el_recherchedepart();
				break;
            case ECRAN_LISTE_ELECTEUR:
                dessiner_liste_electeur();
                break;
            case ECRAN_LISTE_CANDIDAT:
                dessiner_liste_candidat();
                break;
            case ECRAN_LISTE_RAPPBV:
                dessiner_liste_rappbv();
                break;
            case ECRAN_RESULTATS: 
                dessiner_resultats();
                break;
        }

        EndDrawing();
    }

    UnloadTexture(fond);
    CloseWindow();
    return 0;
}