/*------------------------------------------------------------------------*/
/* FICHIER:                         ge_BV.c                               */
/* AUTEUR:                          EMPR                                  */
/* DATE DE CREATION:                16/08/2026                            */
/* DATE DE MODIFICATION:            14/09/2026                            */
/* DESCRIPTION:                     fonctions de manipulation des données */
/*                                    et fichiers relatifs aux bureaux de */
/*                                    votes                               */
/*------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "election.h"

/*--------------------------------------------------------------------------------*/
/* FONCTION:              ajout_BV                                                */
/* DESCRIPTION:           Ajoute un nouveau bureau de vote dans B_votes.dat       */
/* PARAMETRES:            pointeur sur le bureau de vote à insérer                */
/* VALEUR DE RETOUR:      Succès 1 ; Échec 0                                      */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      16/08/2026                                              */
/* DATE DE MODIFICATION:  14/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int ajout_BV(BureauVote* bv) {
    Electeur e;

    if (bv->Id_President != 0 && !lire_elect(ELECT_FILE, bv->Id_President, &e)) return 0;
    if (bv->Id_VPresident != 0 && !lire_elect(ELECT_FILE, bv->Id_VPresident, &e)) return 0;
    if (bv->Id_Secret != 0 && !lire_elect(ELECT_FILE, bv->Id_Secret, &e)) return 0;

    bv->Id_BV = obtenir_dernier_id_BV(BV_FILE) + 1;

    return ecrire_BV(BV_FILE, bv);
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              ecrire_BV                                               */
/* DESCRIPTION:           Écrit un enregistrement bureau de vote dans le fichier, */
/*                        un champ par ligne (permet les espaces dans le texte)   */
/* PARAMETRES:            nom_fichier, pointeur sur le bureau de vote à écrire    */
/* VALEUR DE RETOUR:      1 succès, 0 erreur                                      */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      16/08/2026                                              */
/* DATE DE MODIFICATION:  14/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int ecrire_BV(char* nom_fichier, BureauVote* bv) {
    FILE* fp = fopen(nom_fichier, "a");
    if (fp == NULL) {
        perror("Erreur lors de l'ouverture du fichier des bureaux de vote");
        return 0;
    }

    fprintf(fp, "%d\n", bv->Id_BV);
    fprintf(fp, "%s\n", bv->Adresse_BV.no_rue);
    fprintf(fp, "%s\n", bv->Adresse_BV.rue);
    fprintf(fp, "%s\n", bv->Adresse_BV.ville);
    fprintf(fp, "%s\n", bv->Adresse_BV.departement);
    fprintf(fp, "%d %d %d\n", bv->Id_President, bv->Id_VPresident, bv->Id_Secret);

    fclose(fp);
    return 1;
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              lire_BV                                                 */
/* DESCRIPTION:           Cherche un bureau de vote par Id_BV dans B_votes.dat    */
/* PARAMETRES:            nom_fichier, id_recherche, pointeur resultat            */
/* VALEUR DE RETOUR:      1 si trouvé, 0 sinon                                    */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      16/08/2026                                              */
/* DATE DE MODIFICATION:  14/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int lire_BV(char* nom_fichier, int id_recherche, BureauVote* resultat) {
    FILE* fp = fopen(nom_fichier, "r");
    if (fp == NULL) return 0;

    BureauVote bv;
    while (fscanf(fp, "%d", &bv.Id_BV) == 1) {
        fgetc(fp);

        fgets(bv.Adresse_BV.no_rue, sizeof(bv.Adresse_BV.no_rue), fp);
        bv.Adresse_BV.no_rue[strlen(bv.Adresse_BV.no_rue) - 1] = '\0';

        fgets(bv.Adresse_BV.rue, sizeof(bv.Adresse_BV.rue), fp);
        bv.Adresse_BV.rue[strlen(bv.Adresse_BV.rue) - 1] = '\0';

        fgets(bv.Adresse_BV.ville, sizeof(bv.Adresse_BV.ville), fp);
        bv.Adresse_BV.ville[strlen(bv.Adresse_BV.ville) - 1] = '\0';

        fgets(bv.Adresse_BV.departement, sizeof(bv.Adresse_BV.departement), fp);
        bv.Adresse_BV.departement[strlen(bv.Adresse_BV.departement) - 1] = '\0';

        (void)fscanf(fp, "%d %d %d", &bv.Id_President, &bv.Id_VPresident, &bv.Id_Secret);
        fgetc(fp);

        if (bv.Id_BV == id_recherche) {
            *resultat = bv;
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              obtenir_dernier_id_BV                                   */
/* DESCRIPTION:           Retourne le plus grand Id_BV déjà enregistré            */
/* PARAMETRES:            nom_fichier                                             */
/* VALEUR DE RETOUR:      Dernier Id_BV (0 si fichier vide ou inexistant)         */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      16/08/2026                                              */
/* DATE DE MODIFICATION:  14/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int obtenir_dernier_id_BV(char* nom_fichier) {
    FILE* fp = fopen(nom_fichier, "r");
    if (fp == NULL) return 0;

    BureauVote bv;
    int max_id = 0;

    while (fscanf(fp, "%d", &bv.Id_BV) == 1) {
        fgetc(fp);

        fgets(bv.Adresse_BV.no_rue, sizeof(bv.Adresse_BV.no_rue), fp);
        bv.Adresse_BV.no_rue[strlen(bv.Adresse_BV.no_rue) - 1] = '\0';

        fgets(bv.Adresse_BV.rue, sizeof(bv.Adresse_BV.rue), fp);
        bv.Adresse_BV.rue[strlen(bv.Adresse_BV.rue) - 1] = '\0';

        fgets(bv.Adresse_BV.ville, sizeof(bv.Adresse_BV.ville), fp);
        bv.Adresse_BV.ville[strlen(bv.Adresse_BV.ville) - 1] = '\0';

        fgets(bv.Adresse_BV.departement, sizeof(bv.Adresse_BV.departement), fp);
        bv.Adresse_BV.departement[strlen(bv.Adresse_BV.departement) - 1] = '\0';

        (void)fscanf(fp, "%d %d %d", &bv.Id_President, &bv.Id_VPresident, &bv.Id_Secret);
        fgetc(fp);

        if (bv.Id_BV > max_id) max_id = bv.Id_BV;
    }

    fclose(fp);
    return max_id;
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              modif_BV                                                */
/* DESCRIPTION:           Modifie un bureau de vote existant (Id_BV inchangé)     */
/* PARAMETRES:            pointeur sur le bureau de vote à modifier               */
/* VALEUR DE RETOUR:      Succès 1 ; Échec 0                                      */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      16/08/2026                                              */
/* DATE DE MODIFICATION:  14/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int modif_BV(BureauVote* bv) {
    BureauVote existant;
    Electeur e;

    if (!lire_BV(BV_FILE, bv->Id_BV, &existant))
        return 0;

    if (bv->Id_President != 0 && !lire_elect(ELECT_FILE, bv->Id_President, &e)) return 0;
    if (bv->Id_VPresident != 0 && !lire_elect(ELECT_FILE, bv->Id_VPresident, &e)) return 0;
    if (bv->Id_Secret != 0 && !lire_elect(ELECT_FILE, bv->Id_Secret, &e)) return 0;

    // Réécriture complète du fichier avec le bureau de vote modifié
    FILE* fp_ancien = fopen(BV_FILE, "r");
    FILE* fp_temp = fopen("temp.dat", "w");
    if (fp_ancien == NULL || fp_temp == NULL) return 0;

    BureauVote courant;
    while (fscanf(fp_ancien, "%d", &courant.Id_BV) == 1) {
        fgetc(fp_ancien);

        fgets(courant.Adresse_BV.no_rue, sizeof(courant.Adresse_BV.no_rue), fp_ancien);
        courant.Adresse_BV.no_rue[strlen(courant.Adresse_BV.no_rue) - 1] = '\0';

        fgets(courant.Adresse_BV.rue, sizeof(courant.Adresse_BV.rue), fp_ancien);
        courant.Adresse_BV.rue[strlen(courant.Adresse_BV.rue) - 1] = '\0';

        fgets(courant.Adresse_BV.ville, sizeof(courant.Adresse_BV.ville), fp_ancien);
        courant.Adresse_BV.ville[strlen(courant.Adresse_BV.ville) - 1] = '\0';

        fgets(courant.Adresse_BV.departement, sizeof(courant.Adresse_BV.departement), fp_ancien);
        courant.Adresse_BV.departement[strlen(courant.Adresse_BV.departement) - 1] = '\0';

        (void)fscanf(fp_ancien, "%d %d %d", &courant.Id_President, &courant.Id_VPresident, &courant.Id_Secret);
        fgetc(fp_ancien);

        if (courant.Id_BV == bv->Id_BV) {
            fprintf(fp_temp, "%d\n", bv->Id_BV);
            fprintf(fp_temp, "%s\n", bv->Adresse_BV.no_rue);
            fprintf(fp_temp, "%s\n", bv->Adresse_BV.rue);
            fprintf(fp_temp, "%s\n", bv->Adresse_BV.ville);
            fprintf(fp_temp, "%s\n", bv->Adresse_BV.departement);
            fprintf(fp_temp, "%d %d %d\n", bv->Id_President, bv->Id_VPresident, bv->Id_Secret);
        }
        else {
            fprintf(fp_temp, "%d\n", courant.Id_BV);
            fprintf(fp_temp, "%s\n", courant.Adresse_BV.no_rue);
            fprintf(fp_temp, "%s\n", courant.Adresse_BV.rue);
            fprintf(fp_temp, "%s\n", courant.Adresse_BV.ville);
            fprintf(fp_temp, "%s\n", courant.Adresse_BV.departement);
            fprintf(fp_temp, "%d %d %d\n", courant.Id_President, courant.Id_VPresident, courant.Id_Secret);
        }
    }

    fclose(fp_ancien);
    fclose(fp_temp);

    remove(BV_FILE);
    (void)rename("temp.dat", BV_FILE);

    return 1;
}