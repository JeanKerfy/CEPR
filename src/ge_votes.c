/*------------------------------------------------------------------------*/
/* FICHIER:                         ge_votes.c                            */
/*AUTEUR:                           EMPR                                  */
/*DATE DE CREATION:                 20/08/2026                            */
/*DATE DE MODIFICATION:             17/09/2026                            */
/*DESCRIPTION:                      fonctions de manipulation des données */
/*                                  et fichiers relatifs aux votes        */
/*------------------------------------------------------------------------*/
#include <stdio.h>
#include "election.h"

/*--------------------------------------------------------------------------------*/
/* FONCTION:              ins_vote                                                */
/* DESCRIPTION:           Enregistre le vote d'un électeur dans votes.dat         */
/* PARAMETRES:            pointeur sur le vote à enregistrer                      */
/* VALEUR DE RETOUR:      Succès 1 ; Échec 0                                      */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      20/08/2026                                              */
/* DATE DE MODIFICATION:  17/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int ins_vote(Vote* v) {
    Electeur e;
    Candidat c;

    if (!lire_elect(ELECT_FILE, v->NINU, &e))
        return 0;

    // Vérifier que cet électeur n'a pas déjà voté
    FILE* fp_check = fopen(VOTES_FILE, "r");
    if (fp_check != NULL) {
        Vote courant;
        while (fscanf(fp_check, "%d", &courant.Id_vote) == 1) {
            fgetc(fp_check);

            (void)fscanf(fp_check, "%d %d %d", &courant.Date_vote.jour, &courant.Date_vote.mois, &courant.Date_vote.annee);
            fgetc(fp_check);

            (void)fscanf(fp_check, "%d %d %d", &courant.Id_BV, &courant.NINU, &courant.Id_candid);
            fgetc(fp_check);

            if (courant.NINU == v->NINU) {
                fclose(fp_check);
                return 0; // a déjà voté
            }
        }
        fclose(fp_check);
    }

    if (v->Id_candid != 0 && !lire_candid(CANDID_FILE, v->Id_candid, &c))
        return 0;

    v->Id_vote = obtenir_dernier_id_vote(VOTES_FILE) + 1;
    v->Id_BV = e.Id_BV; // le BV du vote est celui assigné à l'électeur
    generer_date_courante(&v->Date_vote);

    return ecrire_vote(VOTES_FILE, v);
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              ecrire_vote                                             */
/* DESCRIPTION:           Écrit un enregistrement vote dans le fichier            */
/* PARAMETRES:            nom_fichier, pointeur sur le vote à écrire              */
/* VALEUR DE RETOUR:      1 succès, 0 erreur                                      */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      20/08/2026                                              */
/* DATE DE MODIFICATION:  17/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int ecrire_vote(char* nom_fichier, Vote* v) {
    FILE* fp = fopen(nom_fichier, "a");
    if (fp == NULL) {
        perror("Erreur lors de l'ouverture du fichier des votes");
        return 0;
    }

    fprintf(fp, "%d\n", v->Id_vote);
    fprintf(fp, "%d %d %d\n", v->Date_vote.jour, v->Date_vote.mois, v->Date_vote.annee);
    fprintf(fp, "%d %d %d\n", v->Id_BV, v->NINU, v->Id_candid);

    fclose(fp);
    return 1;
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              obtenir_dernier_id_vote                                 */
/* DESCRIPTION:           Retourne le plus grand Id_vote déjà enregistré          */
/* PARAMETRES:            nom_fichier                                             */
/* VALEUR DE RETOUR:      Dernier Id_vote (0 si fichier vide ou inexistant)       */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      20/08/2026                                              */
/* DATE DE MODIFICATION:  17/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int obtenir_dernier_id_vote(char* nom_fichier) {
    FILE* fp = fopen(nom_fichier, "r");
    if (fp == NULL) return 0;

    Vote v;
    int max_id = 0;

    while (fscanf(fp, "%d", &v.Id_vote) == 1) {
        (void)fscanf(fp, "%d %d %d", &v.Date_vote.jour, &v.Date_vote.mois, &v.Date_vote.annee);
        (void)fscanf(fp, "%d %d %d", &v.Id_BV, &v.NINU, &v.Id_candid);

        if (v.Id_vote > max_id) max_id = v.Id_vote;
    }

    fclose(fp);
    return max_id;
}