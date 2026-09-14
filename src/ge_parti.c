/*------------------------------------------------------------------------*/
/* FICHIER:                         ge_parti.c                            */
/*AUTEUR:                           EMPR                                  */
/*DATE DE CREATION:                 03/08/2026                            */
/*DATE DE MODIFICATION:             15/09/2026                            */
/*DESCRIPTION:                      fonctions de manipulation des données */
/*                                  et fichiers relatifs aux partis       */
/*                                  politiques                            */
/*------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "election.h"

/*--------------------------------------------------------------------------------*/
/* FONCTION:              date_valide                                              */
/* DESCRIPTION:           Vérifie qu'une date de cration est une date calendrier  */
/*                        valide ET que le parti politique a au moins 1 an        */
/* PARAMETRES:            une date de creation (structure Date)                   */
/* VALEUR DE RETOUR:      1 si valide et âge >= 1 an, 0 sinon                     */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      17/08/2026                                              */
/* DATE DE MODIFICATION:  13/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int date_valide(Date d) {
    int jours_mois[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int bissextile = (d.annee % 4 == 0 && d.annee % 100 != 0) || (d.annee % 400 == 0);
    int max_jour;
    time_t t = time(NULL);
    struct tm* maintenant = localtime(&t);
    int annee_courante = maintenant->tm_year + 1900;
    int mois_courant = maintenant->tm_mon + 1;
    int jour_courant = maintenant->tm_mday;
    int age;

    if (d.mois < 1 || d.mois > 12) return 0;
    if (d.jour < 1) return 0;

    max_jour = jours_mois[d.mois - 1];
    if (d.mois == 2 && bissextile) max_jour = 29;

    if (d.jour > max_jour) return 0;

    age = annee_courante - d.annee;
    if (mois_courant < d.mois || (mois_courant == d.mois && jour_courant < d.jour))
        age--; // l'anniversaire de cette année n'est pas encore passé

    if (age < 1) return 0;

    return 1;
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              generer_date_courante                                   */
/* DESCRIPTION:           Remplit une Date avec la date du système (jour/mois/    */
/*                        année courants)                                         */
/* PARAMETRES:            pointeur sur la Date à remplir                          */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      26/08/2026                                              */
/* DATE DE MODIFICATION:  31/08/2026                                              */
/*--------------------------------------------------------------------------------*/
void generer_date_courante(Date* d) {
    time_t t = time(NULL);
    struct tm* maintenant = localtime(&t);

    d->jour = maintenant->tm_mday;
    d->mois = maintenant->tm_mon + 1;
    d->annee = maintenant->tm_year + 1900;
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              ins_PP                                                  */
/* DESCRIPTION:           Insère un nouveau parti politique dans p_pol.dat        */
/* PARAMETRES:            pointeur sur le parti politique à insérer               */
/* VALEUR DE RETOUR:      1 succès, 0 échec                                       */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      29/08/2026                                              */
/* DATE DE MODIFICATION:  15/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int ins_PP(PartiPolitique* pp) {
    if (!date_valide(pp->Date_creat))
        return 0;

    pp->Id_PP = obtenir_dernier_id_PP(PP_FILE) + 1;
    generer_date_courante(&pp->Date_ins);

    return ecrire_PP(PP_FILE, pp);
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              ecrire_PP                                               */
/* DESCRIPTION:           Écrit un enregistrement parti politique dans le fichier */
/* PARAMETRES:            nom_fichier, pointeur sur le parti politique à écrire   */
/* VALEUR DE RETOUR:      1 succès, 0 erreur                                      */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      20/08/2026                                              */
/* DATE DE MODIFICATION:  15/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int ecrire_PP(char* nom_fichier, PartiPolitique* pp) {
    FILE* fp = fopen(nom_fichier, "a");
    if (fp == NULL) {
        perror("Erreur lors de l'ouverture du fichier des partis politiques");
        return 0;
    }

    fprintf(fp, "%d\n", pp->Id_PP);
    fprintf(fp, "%s\n", pp->Nom);
    fprintf(fp, "%d %d %d\n", pp->Date_creat.jour, pp->Date_creat.mois, pp->Date_creat.annee);
    fprintf(fp, "%d %d %d\n", pp->Date_ins.jour, pp->Date_ins.mois, pp->Date_ins.annee);
    fprintf(fp, "%s\n", pp->Adr.no_rue);
    fprintf(fp, "%s\n", pp->Adr.rue);
    fprintf(fp, "%s\n", pp->Adr.ville);
    fprintf(fp, "%s\n", pp->Adr.departement);
    fprintf(fp, "%s\n", pp->Responsable);
    fprintf(fp, "%s\n", pp->Telephone);
    fprintf(fp, "%s\n", pp->Email);

    fclose(fp);
    return 1;
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              lire_PP                                                 */
/* DESCRIPTION:           Cherche un parti politique par Id_PP dans p_pol.dat     */
/* PARAMETRES:            nom_fichier, id_recherche, pointeur resultat            */
/* VALEUR DE RETOUR:      1 si trouvé, 0 sinon                                    */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      20/08/2026                                              */
/* DATE DE MODIFICATION:  15/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int lire_PP(char* nom_fichier, int id_recherche, PartiPolitique* resultat) {
    FILE* fp = fopen(nom_fichier, "r");
    if (fp == NULL) return 0;

    PartiPolitique pp;
    while (fscanf(fp, "%d", &pp.Id_PP) == 1) {
        fgetc(fp);

        fgets(pp.Nom, sizeof(pp.Nom), fp);
        pp.Nom[strlen(pp.Nom) - 1] = '\0';

        (void)fscanf(fp, "%d %d %d", &pp.Date_creat.jour, &pp.Date_creat.mois, &pp.Date_creat.annee);
        fgetc(fp);

        (void)fscanf(fp, "%d %d %d", &pp.Date_ins.jour, &pp.Date_ins.mois, &pp.Date_ins.annee);
        fgetc(fp);

        fgets(pp.Adr.no_rue, sizeof(pp.Adr.no_rue), fp);
        pp.Adr.no_rue[strlen(pp.Adr.no_rue) - 1] = '\0';

        fgets(pp.Adr.rue, sizeof(pp.Adr.rue), fp);
        pp.Adr.rue[strlen(pp.Adr.rue) - 1] = '\0';

        fgets(pp.Adr.ville, sizeof(pp.Adr.ville), fp);
        pp.Adr.ville[strlen(pp.Adr.ville) - 1] = '\0';

        fgets(pp.Adr.departement, sizeof(pp.Adr.departement), fp);
        pp.Adr.departement[strlen(pp.Adr.departement) - 1] = '\0';

        fgets(pp.Responsable, sizeof(pp.Responsable), fp);
        pp.Responsable[strlen(pp.Responsable) - 1] = '\0';

        fgets(pp.Telephone, sizeof(pp.Telephone), fp);
        pp.Telephone[strlen(pp.Telephone) - 1] = '\0';

        fgets(pp.Email, sizeof(pp.Email), fp);
        pp.Email[strlen(pp.Email) - 1] = '\0';

        if (pp.Id_PP == id_recherche) {
            *resultat = pp;
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              obtenir_dernier_id_PP                                   */
/* DESCRIPTION:           Retourne le plus grand Id_PP déjà enregistré            */
/* PARAMETRES:            nom_fichier                                             */
/* VALEUR DE RETOUR:      Dernier Id_PP (0 si fichier vide)                       */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      29/08/2026                                              */
/* DATE DE MODIFICATION:  29/08/2026                                              */
/*--------------------------------------------------------------------------------*/
int obtenir_dernier_id_PP(char* nom_fichier) {
    FILE* fp = fopen(nom_fichier, "r");
    if (fp == NULL) return 0;

    PartiPolitique pp;
    int max_id = 0;

    while (fscanf(fp, "%d", &pp.Id_PP) == 1) {
        fgetc(fp);

        fgets(pp.Nom, sizeof(pp.Nom), fp);
        pp.Nom[strlen(pp.Nom) - 1] = '\0';

        (void)fscanf(fp, "%d %d %d", &pp.Date_creat.jour, &pp.Date_creat.mois, &pp.Date_creat.annee);
        fgetc(fp);

        (void)fscanf(fp, "%d %d %d", &pp.Date_ins.jour, &pp.Date_ins.mois, &pp.Date_ins.annee);
        fgetc(fp);

        fgets(pp.Adr.no_rue, sizeof(pp.Adr.no_rue), fp);
        pp.Adr.no_rue[strlen(pp.Adr.no_rue) - 1] = '\0';

        fgets(pp.Adr.rue, sizeof(pp.Adr.rue), fp);
        pp.Adr.rue[strlen(pp.Adr.rue) - 1] = '\0';

        fgets(pp.Adr.ville, sizeof(pp.Adr.ville), fp);
        pp.Adr.ville[strlen(pp.Adr.ville) - 1] = '\0';

        fgets(pp.Adr.departement, sizeof(pp.Adr.departement), fp);
        pp.Adr.departement[strlen(pp.Adr.departement) - 1] = '\0';

        fgets(pp.Responsable, sizeof(pp.Responsable), fp);
        pp.Responsable[strlen(pp.Responsable) - 1] = '\0';

        fgets(pp.Telephone, sizeof(pp.Telephone), fp);
        pp.Telephone[strlen(pp.Telephone) - 1] = '\0';

        fgets(pp.Email, sizeof(pp.Email), fp);
        pp.Email[strlen(pp.Email) - 1] = '\0';

        if (pp.Id_PP > max_id) max_id = pp.Id_PP;
    }

    fclose(fp);
    return max_id;
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              modif_PP                                                */
/* DESCRIPTION:           Modifie un parti politique existant (Id_PP inchangé)    */
/* PARAMETRES:            pointeur sur le parti politique à modifier              */
/* VALEUR DE RETOUR:      1 succès ; 0 echecc                                       */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      20/08/2026                                              */
/* DATE DE MODIFICATION:  15/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int modif_PP(PartiPolitique* pp) {
    PartiPolitique existant;

    if (!lire_PP(PP_FILE, pp->Id_PP, &existant))
        return 0;

    if (!date_valide(pp->Date_creat))
        return 0;

    pp->Date_ins = existant.Date_ins; // Date_ins ne se modifie jamais

    FILE* fp_ancien = fopen(PP_FILE, "r");
    FILE* fp_temp = fopen("temp.dat", "w");
    if (fp_ancien == NULL || fp_temp == NULL) return 0;

    PartiPolitique courant;
    while (fscanf(fp_ancien, "%d", &courant.Id_PP) == 1) {
        fgetc(fp_ancien);

        fgets(courant.Nom, sizeof(courant.Nom), fp_ancien);
        courant.Nom[strlen(courant.Nom) - 1] = '\0';

        (void)fscanf(fp_ancien, "%d %d %d", &courant.Date_creat.jour, &courant.Date_creat.mois, &courant.Date_creat.annee);
        fgetc(fp_ancien);

        (void)fscanf(fp_ancien, "%d %d %d", &courant.Date_ins.jour, &courant.Date_ins.mois, &courant.Date_ins.annee);
        fgetc(fp_ancien);

        fgets(courant.Adr.no_rue, sizeof(courant.Adr.no_rue), fp_ancien);
        courant.Adr.no_rue[strlen(courant.Adr.no_rue) - 1] = '\0';

        fgets(courant.Adr.rue, sizeof(courant.Adr.rue), fp_ancien);
        courant.Adr.rue[strlen(courant.Adr.rue) - 1] = '\0';

        fgets(courant.Adr.ville, sizeof(courant.Adr.ville), fp_ancien);
        courant.Adr.ville[strlen(courant.Adr.ville) - 1] = '\0';

        fgets(courant.Adr.departement, sizeof(courant.Adr.departement), fp_ancien);
        courant.Adr.departement[strlen(courant.Adr.departement) - 1] = '\0';

        fgets(courant.Responsable, sizeof(courant.Responsable), fp_ancien);
        courant.Responsable[strlen(courant.Responsable) - 1] = '\0';

        fgets(courant.Telephone, sizeof(courant.Telephone), fp_ancien);
        courant.Telephone[strlen(courant.Telephone) - 1] = '\0';

        fgets(courant.Email, sizeof(courant.Email), fp_ancien);
        courant.Email[strlen(courant.Email) - 1] = '\0';

        if (courant.Id_PP == pp->Id_PP) {
            fprintf(fp_temp, "%d\n", pp->Id_PP);
            fprintf(fp_temp, "%s\n", pp->Nom);
            fprintf(fp_temp, "%d %d %d\n", pp->Date_creat.jour, pp->Date_creat.mois, pp->Date_creat.annee);
            fprintf(fp_temp, "%d %d %d\n", pp->Date_ins.jour, pp->Date_ins.mois, pp->Date_ins.annee);
            fprintf(fp_temp, "%s\n", pp->Adr.no_rue);
            fprintf(fp_temp, "%s\n", pp->Adr.rue);
            fprintf(fp_temp, "%s\n", pp->Adr.ville);
            fprintf(fp_temp, "%s\n", pp->Adr.departement);
            fprintf(fp_temp, "%s\n", pp->Responsable);
            fprintf(fp_temp, "%s\n", pp->Telephone);
            fprintf(fp_temp, "%s\n", pp->Email);
        }
        else {
            fprintf(fp_temp, "%d\n", courant.Id_PP);
            fprintf(fp_temp, "%s\n", courant.Nom);
            fprintf(fp_temp, "%d %d %d\n", courant.Date_creat.jour, courant.Date_creat.mois, courant.Date_creat.annee);
            fprintf(fp_temp, "%d %d %d\n", courant.Date_ins.jour, courant.Date_ins.mois, courant.Date_ins.annee);
            fprintf(fp_temp, "%s\n", courant.Adr.no_rue);
            fprintf(fp_temp, "%s\n", courant.Adr.rue);
            fprintf(fp_temp, "%s\n", courant.Adr.ville);
            fprintf(fp_temp, "%s\n", courant.Adr.departement);
            fprintf(fp_temp, "%s\n", courant.Responsable);
            fprintf(fp_temp, "%s\n", courant.Telephone);
            fprintf(fp_temp, "%s\n", courant.Email);
        }
    }

    fclose(fp_ancien);
    fclose(fp_temp);

    remove(PP_FILE);
    (void)rename("temp.dat", PP_FILE);

    return 1;
}