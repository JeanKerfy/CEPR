/*------------------------------------------------------------------------*/
/* FICHIER:                         ge_candi.c                            */
/*AUTEUR:                           EMPR                                  */
/*DATE DE CREATION:                 17/08/2026                            */
/*DATE DE MODIFICATION:             13/09/2026                            */
/*DESCRIPTION:                      fonctions de manipulation des données */
/*                                   et fichiers relatifs aux candidats   */
/*------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "election.h"

/*--------------------------------------------------------------------------------*/
/* FONCTION:              age_valide                                              */
/* DESCRIPTION:           Vérifie qu'une date de naissance est une date calendrier*/
/*                        valide ET que la personne a au moins 30 ans             */
/* PARAMETRES:            une date de naissance (structure Date)                  */
/* VALEUR DE RETOUR:      1 si valide et âge >= 30 ans, 0 sinon                   */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      17/08/2026                                              */
/* DATE DE MODIFICATION:  13/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int age_valide(Date d) {
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

    if (age < 30) return 0;

    return 1;
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              ins_cand                                                */
/* DESCRIPTION:           Inscrit un nouveau candidat dans candid.dat             */
/* PARAMETRES:            pointeur sur le candidat à insérer                      */
/* VALEUR DE RETOUR:      Succès 1 ; Échec 0                                      */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      17/08/2026                                              */
/* DATE DE MODIFICATION:  13/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int ins_cand(Candidat* c) {
    PartiPolitique pp;

    if (!age_valide(c->Dat_Nais))
        return 0;

    if (c->type != 0 && !lire_PP(PP_FILE, c->type, &pp))
        return 0;

    c->Id_candid = obtenir_dernier_id_candid(CANDID_FILE) + 1;
    generer_date_courante(&c->Date_ins);

    return ecrire_candid(CANDID_FILE, c);
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              ecrire_candid                                           */
/* DESCRIPTION:           Écrit un enregistrement candidat dans le fichier,       */
/*                        un champ par ligne (permet les espaces dans le texte)   */
/* PARAMETRES:            nom_fichier, pointeur sur le candidat à écrire          */
/* VALEUR DE RETOUR:      1 succès, 0 erreur                                      */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      17/08/2026                                              */
/* DATE DE MODIFICATION:  13/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int ecrire_candid(char* nom_fichier, Candidat* c) {
    FILE* fp = fopen(nom_fichier, "a");
    if (fp == NULL) {
        perror("Erreur lors de l'ouverture du fichier des candidats");
        return 0;
    }

    fprintf(fp, "%d\n", c->Id_candid);
    fprintf(fp, "%s\n", c->Nom);
    fprintf(fp, "%s\n", c->Prenom);
    fprintf(fp, "%d %d %d\n", c->Dat_Nais.jour, c->Dat_Nais.mois, c->Dat_Nais.annee);
    fprintf(fp, "%s\n", c->Adr.no_rue);
    fprintf(fp, "%s\n", c->Adr.rue);
    fprintf(fp, "%s\n", c->Adr.ville);
    fprintf(fp, "%s\n", c->Adr.departement);
    fprintf(fp, "%s\n", c->Email);
    fprintf(fp, "%s\n", c->Telephone);
    fprintf(fp, "%d\n", c->type);
    fprintf(fp, "%d %d %d\n", c->Date_ins.jour, c->Date_ins.mois, c->Date_ins.annee);

    fclose(fp);
    return 1;
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              lire_candid                                             */
/* DESCRIPTION:           Cherche un candidat par Id_candid dans candid.dat       */
/* PARAMETRES:            nom_fichier, id_recherche, pointeur resultat            */
/* VALEUR DE RETOUR:      1 si trouvé, 0 sinon                                    */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      17/08/2026                                              */
/* DATE DE MODIFICATION:  13/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int lire_candid(char* nom_fichier, int id_recherche, Candidat* resultat) {
    FILE* fp = fopen(nom_fichier, "r");
    if (fp == NULL) return 0;

    Candidat c;
    while (fscanf(fp, "%d", &c.Id_candid) == 1) {
        fgetc(fp);

        fgets(c.Nom, sizeof(c.Nom), fp);
        c.Nom[strlen(c.Nom) - 1] = '\0';

        fgets(c.Prenom, sizeof(c.Prenom), fp);
        c.Prenom[strlen(c.Prenom) - 1] = '\0';

        (void)fscanf(fp, "%d %d %d", &c.Dat_Nais.jour, &c.Dat_Nais.mois, &c.Dat_Nais.annee);
        fgetc(fp);

        fgets(c.Adr.no_rue, sizeof(c.Adr.no_rue), fp);
        c.Adr.no_rue[strlen(c.Adr.no_rue) - 1] = '\0';

        fgets(c.Adr.rue, sizeof(c.Adr.rue), fp);
        c.Adr.rue[strlen(c.Adr.rue) - 1] = '\0';

        fgets(c.Adr.ville, sizeof(c.Adr.ville), fp);
        c.Adr.ville[strlen(c.Adr.ville) - 1] = '\0';

        fgets(c.Adr.departement, sizeof(c.Adr.departement), fp);
        c.Adr.departement[strlen(c.Adr.departement) - 1] = '\0';

        fgets(c.Email, sizeof(c.Email), fp);
        c.Email[strlen(c.Email) - 1] = '\0';

        fgets(c.Telephone, sizeof(c.Telephone), fp);
        c.Telephone[strlen(c.Telephone) - 1] = '\0';

        (void)fscanf(fp, "%d", &c.type);
        fgetc(fp);

        (void)fscanf(fp, "%d %d %d", &c.Date_ins.jour, &c.Date_ins.mois, &c.Date_ins.annee);
        fgetc(fp);

        if (c.Id_candid == id_recherche) {
            *resultat = c;
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              obtenir_dernier_id_candid                               */
/* DESCRIPTION:           Retourne le plus grand Id_candid déjà enregistré        */
/* PARAMETRES:            nom_fichier                                             */
/* VALEUR DE RETOUR:      Dernier Id_candid (0 si fichier vide ou inexistant)     */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      17/08/2026                                              */
/* DATE DE MODIFICATION:  13/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int obtenir_dernier_id_candid(char* nom_fichier) {
    FILE* fp = fopen(nom_fichier, "r");
    if (fp == NULL) return 0;

    Candidat c;
    int max_id = 0;

    while (fscanf(fp, "%d", &c.Id_candid) == 1) {
        fgetc(fp);

        fgets(c.Nom, sizeof(c.Nom), fp);
        c.Nom[strlen(c.Nom) - 1] = '\0';

        fgets(c.Prenom, sizeof(c.Prenom), fp);
        c.Prenom[strlen(c.Prenom) - 1] = '\0';

        (void)fscanf(fp, "%d %d %d", &c.Dat_Nais.jour, &c.Dat_Nais.mois, &c.Dat_Nais.annee);
        fgetc(fp);

        fgets(c.Adr.no_rue, sizeof(c.Adr.no_rue), fp);
        c.Adr.no_rue[strlen(c.Adr.no_rue) - 1] = '\0';

        fgets(c.Adr.rue, sizeof(c.Adr.rue), fp);
        c.Adr.rue[strlen(c.Adr.rue) - 1] = '\0';

        fgets(c.Adr.ville, sizeof(c.Adr.ville), fp);
        c.Adr.ville[strlen(c.Adr.ville) - 1] = '\0';

        fgets(c.Adr.departement, sizeof(c.Adr.departement), fp);
        c.Adr.departement[strlen(c.Adr.departement) - 1] = '\0';

        fgets(c.Email, sizeof(c.Email), fp);
        c.Email[strlen(c.Email) - 1] = '\0';

        fgets(c.Telephone, sizeof(c.Telephone), fp);
        c.Telephone[strlen(c.Telephone) - 1] = '\0';

        (void)fscanf(fp, "%d", &c.type);
        fgetc(fp);

        (void)fscanf(fp, "%d %d %d", &c.Date_ins.jour, &c.Date_ins.mois, &c.Date_ins.annee);
        fgetc(fp);

        if (c.Id_candid > max_id) max_id = c.Id_candid;
    }

    fclose(fp);
    return max_id;
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              modif_cand                                              */
/* DESCRIPTION:           Modifie un candidat existant (Id_candid inchangé)       */
/* PARAMETRES:            pointeur sur le candidat à modifier                     */
/* VALEUR DE RETOUR:      Succès 1 ; Échec 0                                      */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      17/08/2026                                              */
/* DATE DE MODIFICATION:  13/09/2026                                              */
/*--------------------------------------------------------------------------------*/
int modif_cand(Candidat* c) {
    Candidat existant;
    PartiPolitique pp;

    if (!lire_candid(CANDID_FILE, c->Id_candid, &existant))
        return 0;

    if (!age_valide(c->Dat_Nais))
        return 0;

    if (c->type != 0 && !lire_PP(PP_FILE, c->type, &pp))
        return 0;

    c->Date_ins = existant.Date_ins; // Date_ins ne se modifie jamais

    // Réécriture complète du fichier avec le candidat modifié
    FILE* fp_ancien = fopen(CANDID_FILE, "r");
    FILE* fp_temp = fopen("temp.dat", "w");
    if (fp_ancien == NULL || fp_temp == NULL) return 0;

    Candidat courant;
    while (fscanf(fp_ancien, "%d", &courant.Id_candid) == 1) {
        fgetc(fp_ancien);

        fgets(courant.Nom, sizeof(courant.Nom), fp_ancien);
        courant.Nom[strlen(courant.Nom) - 1] = '\0';

        fgets(courant.Prenom, sizeof(courant.Prenom), fp_ancien);
        courant.Prenom[strlen(courant.Prenom) - 1] = '\0';

        (void)fscanf(fp_ancien, "%d %d %d", &courant.Dat_Nais.jour, &courant.Dat_Nais.mois, &courant.Dat_Nais.annee);
        fgetc(fp_ancien);

        fgets(courant.Adr.no_rue, sizeof(courant.Adr.no_rue), fp_ancien);
        courant.Adr.no_rue[strlen(courant.Adr.no_rue) - 1] = '\0';

        fgets(courant.Adr.rue, sizeof(courant.Adr.rue), fp_ancien);
        courant.Adr.rue[strlen(courant.Adr.rue) - 1] = '\0';

        fgets(courant.Adr.ville, sizeof(courant.Adr.ville), fp_ancien);
        courant.Adr.ville[strlen(courant.Adr.ville) - 1] = '\0';

        fgets(courant.Adr.departement, sizeof(courant.Adr.departement), fp_ancien);
        courant.Adr.departement[strlen(courant.Adr.departement) - 1] = '\0';

        fgets(courant.Email, sizeof(courant.Email), fp_ancien);
        courant.Email[strlen(courant.Email) - 1] = '\0';

        fgets(courant.Telephone, sizeof(courant.Telephone), fp_ancien);
        courant.Telephone[strlen(courant.Telephone) - 1] = '\0';

        (void)fscanf(fp_ancien, "%d", &courant.type);
        fgetc(fp_ancien);

        (void)fscanf(fp_ancien, "%d %d %d", &courant.Date_ins.jour, &courant.Date_ins.mois, &courant.Date_ins.annee);
        fgetc(fp_ancien);

        if (courant.Id_candid == c->Id_candid) {
            fprintf(fp_temp, "%d\n", c->Id_candid);
            fprintf(fp_temp, "%s\n", c->Nom);
            fprintf(fp_temp, "%s\n", c->Prenom);
            fprintf(fp_temp, "%d %d %d\n", c->Dat_Nais.jour, c->Dat_Nais.mois, c->Dat_Nais.annee);
            fprintf(fp_temp, "%s\n", c->Adr.no_rue);
            fprintf(fp_temp, "%s\n", c->Adr.rue);
            fprintf(fp_temp, "%s\n", c->Adr.ville);
            fprintf(fp_temp, "%s\n", c->Adr.departement);
            fprintf(fp_temp, "%s\n", c->Email);
            fprintf(fp_temp, "%s\n", c->Telephone);
            fprintf(fp_temp, "%d\n", c->type);
            fprintf(fp_temp, "%d %d %d\n", c->Date_ins.jour, c->Date_ins.mois, c->Date_ins.annee);
        }
        else {
            fprintf(fp_temp, "%d\n", courant.Id_candid);
            fprintf(fp_temp, "%s\n", courant.Nom);
            fprintf(fp_temp, "%s\n", courant.Prenom);
            fprintf(fp_temp, "%d %d %d\n", courant.Dat_Nais.jour, courant.Dat_Nais.mois, courant.Dat_Nais.annee);
            fprintf(fp_temp, "%s\n", courant.Adr.no_rue);
            fprintf(fp_temp, "%s\n", courant.Adr.rue);
            fprintf(fp_temp, "%s\n", courant.Adr.ville);
            fprintf(fp_temp, "%s\n", courant.Adr.departement);
            fprintf(fp_temp, "%s\n", courant.Email);
            fprintf(fp_temp, "%s\n", courant.Telephone);
            fprintf(fp_temp, "%d\n", courant.type);
            fprintf(fp_temp, "%d %d %d\n", courant.Date_ins.jour, courant.Date_ins.mois, courant.Date_ins.annee);
        }
    }

    fclose(fp_ancien);
    fclose(fp_temp);

    remove(CANDID_FILE);
    (void)rename("temp.dat", CANDID_FILE);

    return 1;
}