/*------------------------------------------------------------------------*/
/* FICHIER:                         ge_elect.c                            */
/*AUTEUR:                           EMPR                                  */
/*DATE DE CREATION:                 17/08/2026                            */
/*DATE DE MODIFICATION:             18/09/2026                            */
/*DESCRIPTION:                      fonctions de manipulation des données */
/*                                  et du fichier relatifs aux électeurs  */
/*------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "election.h"

/*------------------------------------------------------------------------------------------*/
/* FONCTION:                        ins_elect                                               */
/* DESCRIPTION:                     Enregistre un nouvel électeur dans elect.dat            */
/* PARAMETRES:                      pointeur sur l'électeur à insérer                       */
/* VALEUR DE RETOUR:                Succès 1 ; Échec 0                                      */
/* AUTEUR:                          EMPR                                                    */
/* DATE DE CREATION:                17/08/2026                                              */
/*DATE DE MODIFICATION:             18/09/2026                                              */
/*------------------------------------------------------------------------------------------*/
int ins_elect(Electeur* e) {
    Electeur temp;
    BureauVote bv;

    if (lire_elect(ELECT_FILE, e->NINU, &temp))
        return 0; // NINU déjà utilisé

    if (!lire_BV(BV_FILE, e->Id_BV, &bv))
        return 0; // bureau de vote introuvable

    return ecrire_elect(ELECT_FILE, e);
}

/*------------------------------------------------------------------------------------------*/
/* FONCTION:                        ecrire_elect                                            */
/* DESCRIPTION:                     Écrit un enregistrement électeur dans le fichier,       */
/* PARAMETRES:                      nom_fichier, pointeur sur l'électeur à écrire           */
/* VALEUR DE RETOUR:                1 succès, 0 erreur                                      */
/* AUTEUR:                          EMPR                                                    */
/* DATE DE CREATION:                21/08/2026                                              */
/* DATE DE MODIFICATION:            13/09/2026                                              */
/*------------------------------------------------------------------------------------------*/
int ecrire_elect(char* nom_fichier, Electeur* e) {
    FILE* fp = fopen(nom_fichier, "a");
    if (fp == NULL) {
        perror("Erreur lors de l'ouverture du fichier des électeurs");
        return 0;
    }

    fprintf(fp, "%d\n", e->NINU);
    fprintf(fp, "%s\n", e->Nom);
    fprintf(fp, "%s\n", e->Prenom);
    fprintf(fp, "%s\n", e->Adr.no_rue);
    fprintf(fp, "%s\n", e->Adr.rue);
    fprintf(fp, "%s\n", e->Adr.ville);
    fprintf(fp, "%s\n", e->Adr.departement);
    fprintf(fp, "%d\n", e->Id_BV);

    fclose(fp);
    return 1;
}

/*------------------------------------------------------------------------------------------*/
/* FONCTION:                        lire_elect                                              */
/* DESCRIPTION:                     Cherche un électeur par NINU dans elect.dat             */
/* PARAMETRES:                      nom_fichier, ninu_recherche, pointeur resultat          */
/* VALEUR DE RETOUR:                1 si trouvé, 0 sinon                                    */
/* AUTEUR:                          EMPR                                                    */
/* DATE DE CREATION:                17/08/2026                                              */
/* DATE DE MODIFICATION:            13/09/2026                                              */
/*------------------------------------------------------------------------------------------*/
int lire_elect(char* nom_fichier, int ninu_recherche, Electeur* resultat) {
    FILE* fp = fopen(nom_fichier, "r");
    if (fp == NULL) return 0;

    Electeur e;
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

        if (e.NINU == ninu_recherche) {
            *resultat = e;
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

/*------------------------------------------------------------------------------------------*/
/* FONCTION:                        modif_elect                                             */
/* DESCRIPTION:                     Modifie un électeur existant (NINU inchangé)            */
/* PARAMETRES:                      pointeur sur l'electeur à modifier                      */
/* VALEUR DE RETOUR:                Succè 1 ; Échec 0                                       */
/* AUTEUR:                          EMPR                                                    */
/* DATE DE CREATION:                17/08/2026                                              */
/* DATE DE MODIFICATION:            13/09/2026                                              */
/*------------------------------------------------------------------------------------------*/
int modif_elect(Electeur* e) {
    Electeur existant;
    BureauVote bv;

    if (!lire_elect(ELECT_FILE, e->NINU, &existant))
        return 0;

    if (!lire_BV(BV_FILE, e->Id_BV, &bv))
        return 0;

    // Réécriture complète du fichier avec l'électeur modifié
    FILE* fp_ancien = fopen(ELECT_FILE, "r");
    FILE* fp_temp = fopen("temp.dat", "w");
    if (fp_ancien == NULL || fp_temp == NULL) return 0;

    Electeur courant;
    while (fscanf(fp_ancien, "%d", &courant.NINU) == 1) {
        fgetc(fp_ancien);

        fgets(courant.Nom, sizeof(courant.Nom), fp_ancien);
        courant.Nom[strlen(courant.Nom) - 1] = '\0';

        fgets(courant.Prenom, sizeof(courant.Prenom), fp_ancien);
        courant.Prenom[strlen(courant.Prenom) - 1] = '\0';

        fgets(courant.Adr.no_rue, sizeof(courant.Adr.no_rue), fp_ancien);
        courant.Adr.no_rue[strlen(courant.Adr.no_rue) - 1] = '\0';

        fgets(courant.Adr.rue, sizeof(courant.Adr.rue), fp_ancien);
        courant.Adr.rue[strlen(courant.Adr.rue) - 1] = '\0';

        fgets(courant.Adr.ville, sizeof(courant.Adr.ville), fp_ancien);
        courant.Adr.ville[strlen(courant.Adr.ville) - 1] = '\0';

        fgets(courant.Adr.departement, sizeof(courant.Adr.departement), fp_ancien);
        courant.Adr.departement[strlen(courant.Adr.departement) - 1] = '\0';

        (void)fscanf(fp_ancien, "%d", &courant.Id_BV);
        fgetc(fp_ancien);

        if (courant.NINU == e->NINU) {
            fprintf(fp_temp, "%d\n", e->NINU);
            fprintf(fp_temp, "%s\n", e->Nom);
            fprintf(fp_temp, "%s\n", e->Prenom);
            fprintf(fp_temp, "%s\n", e->Adr.no_rue);
            fprintf(fp_temp, "%s\n", e->Adr.rue);
            fprintf(fp_temp, "%s\n", e->Adr.ville);
            fprintf(fp_temp, "%s\n", e->Adr.departement);
            fprintf(fp_temp, "%d\n", e->Id_BV);
        }
        else {
            fprintf(fp_temp, "%d\n", courant.NINU);
            fprintf(fp_temp, "%s\n", courant.Nom);
            fprintf(fp_temp, "%s\n", courant.Prenom);
            fprintf(fp_temp, "%s\n", courant.Adr.no_rue);
            fprintf(fp_temp, "%s\n", courant.Adr.rue);
            fprintf(fp_temp, "%s\n", courant.Adr.ville);
            fprintf(fp_temp, "%s\n", courant.Adr.departement);
            fprintf(fp_temp, "%d\n", courant.Id_BV);
        }
    }

    fclose(fp_ancien);
    fclose(fp_temp);

    remove(ELECT_FILE);
    (void)rename("temp.dat", ELECT_FILE);

    return 1;
}