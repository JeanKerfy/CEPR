/*------------------------------------------------------------------------*/
/* FICHIER:                         rapp.c                                */
/*AUTEUR:                           EMPR                                  */
/*DATE DE CREATION:                 24/08/2026                            */
/*DATE DE MODIFICATION:             17/09/2026                            */
/*DESCRIPTION:                      fonctions de génération des rapports  */
/*                                  de gestion et des résultats du scrutin*/
/*------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "election.h"

/*--------------------------------------------------------------------------------*/
/* FONCTION:              List_Electeurs                                          */
/* DESCRIPTION:           Liste les électeurs dans l'ordre du fichier elect.dat   */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      24/08/2026                                              */
/* DATE DE MODIFICATION:  17/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void List_Electeurs(void) {
    FILE* fp = fopen(ELECT_FILE, "r");
    if (fp == NULL) {
        printf("Aucun électeur enregistré.\n");
        return;
    }

    Electeur e;

    printf("\nListe des électeurs :\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("| Département | Id_BV | NINU       | Nom            | Prénom         | Adresse\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

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

        printf("| %-11s | BV-%04d | %10d | %-14s | %-14s | %s, %s, %s\n",
            e.Adr.departement, e.Id_BV, e.NINU, e.Nom, e.Prenom,
            e.Adr.no_rue, e.Adr.rue, e.Adr.ville);
    }

    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    fclose(fp);
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              List_Candid                                             */
/* DESCRIPTION:           Liste les candidats                                     */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      24/08/2026                                              */
/* DATE DE MODIFICATION:  17/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void List_Candid(void) {
    FILE* fp = fopen(CANDID_FILE, "r");
    if (fp == NULL) {
        printf("Aucun candidat enregistré.\n");
        return;
    }

    Candidat c;
    PartiPolitique pp;

    printf("\nListe des candidats à la présidence :\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("| Id_cand | Nom            | Prénom         | Type              |\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

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

        if (c.type == 0) {
            printf("| CD-%04d | %-14s | %-14s | %-17s |\n", c.Id_candid, c.Nom, c.Prenom, "Indépendant");
        }
        else if (lire_PP(PP_FILE, c.type, &pp)) {
            printf("| CD-%04d | %-14s | %-14s | %-17s |\n", c.Id_candid, c.Nom, c.Prenom, pp.Nom);
        }
        else {
            printf("| CD-%04d | %-14s | %-14s | %-17s |\n", c.Id_candid, c.Nom, c.Prenom, "Parti inconnu");
        }
    }

    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    fclose(fp);
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              List_BV                                                 */
/* DESCRIPTION:           Liste les bureaux de vote                               */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      24/08/2026                                              */
/* DATE DE MODIFICATION:  17/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void List_BV(void) {
    FILE* fp = fopen(BV_FILE, "r");
    if (fp == NULL) {
        printf("Aucun bureau de vote enregistré.\n");
        return;
    }

    BureauVote bv;
    Electeur president, vpresident, secretaire;
    int trouve_pres, trouve_vp, trouve_sec;

    printf("\nListe des bureaux de vote :\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("| Id_BV | Adresse                        | Président       | Vice-Président  | Secrétaire      |\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

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

        trouve_pres = lire_elect(ELECT_FILE, bv.Id_President, &president);
        trouve_vp = lire_elect(ELECT_FILE, bv.Id_VPresident, &vpresident);
        trouve_sec = lire_elect(ELECT_FILE, bv.Id_Secret, &secretaire);

        printf("| BV-%04d | %s, %s, %s, %s | %-8s %-8s | %-8s %-8s | %-8s %-8s |\n",
            bv.Id_BV,
            bv.Adresse_BV.no_rue, bv.Adresse_BV.rue, bv.Adresse_BV.ville, bv.Adresse_BV.departement,
            trouve_pres ? president.Nom : "Inconnu", trouve_pres ? president.Prenom : "",
            trouve_vp ? vpresident.Nom : "Inconnu", trouve_vp ? vpresident.Prenom : "",
            trouve_sec ? secretaire.Nom : "Inconnu", trouve_sec ? secretaire.Prenom : "");
    }

    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    fclose(fp);
}

/*--------------------------------------------------------------------------------*/
/* FONCTION:              Resultats                                               */
/* DESCRIPTION:           Calcule et affiche les résultats par département et par */
/*                        candidat (triés par pourcentage décroissant), ainsi     */
/*                        qu'un résumé global par candidat pour tout le pays      */
/* PARAMETRES:            Aucun                                                   */
/* VALEUR DE RETOUR:      Aucune                                                  */
/* AUTEUR:                EMPR                                                    */
/* DATE DE CREATION:      24/08/2026                                              */
/* DATE DE MODIFICATION:  17/09/2026                                              */
/*--------------------------------------------------------------------------------*/
void Resultats(void) {
    ResultatDeptCandidat aggregat[MAX_RESULTATS];
    ResultatDept aggregat_dept[MAX_RESULTATS];
    ResultatGlobal aggregat_global[MAX_RESULTATS];
    int n_aggregat = 0, n_dept = 0, n_global = 0;
    int i, j, k;
    int trouve_dept, trouve_agg, trouve_global;
    Vote v;
    BureauVote bv;
    Candidat c;

    FILE* fp = fopen(VOTES_FILE, "r");
    if (fp == NULL) {
        printf("Aucun vote enregistré.\n");
        return;
    }

    // Étape 1 : parcourir tous les votes et accumuler les totaux
    while (fscanf(fp, "%d", &v.Id_vote) == 1) {
        fgetc(fp);

        (void)fscanf(fp, "%d %d %d", &v.Date_vote.jour, &v.Date_vote.mois, &v.Date_vote.annee);
        fgetc(fp);

        (void)fscanf(fp, "%d %d %d", &v.Id_BV, &v.NINU, &v.Id_candid);
        fgetc(fp);

        if (!lire_BV(BV_FILE, v.Id_BV, &bv)) continue; // BV introuvable, on ignore ce vote

        // Chercher ou créer l'entrée du département dans aggregat_dept
        trouve_dept = 0;
        for (i = 0; i < n_dept; i++) {
            if (strcmp(aggregat_dept[i].departement, bv.Adresse_BV.departement) == 0) {
                trouve_dept = 1;
                break;
            }
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
        }
        else {
            aggregat_dept[i].total_non_blancs++;

            // Chercher ou créer l'entrée (département, candidat) dans aggregat
            trouve_agg = 0;
            for (j = 0; j < n_aggregat; j++) {
                if (strcmp(aggregat[j].departement, bv.Adresse_BV.departement) == 0 &&
                    aggregat[j].id_candid == v.Id_candid) {
                    trouve_agg = 1;
                    break;
                }
            }
            if (!trouve_agg) {
                strcpy(aggregat[n_aggregat].departement, bv.Adresse_BV.departement);
                aggregat[n_aggregat].id_candid = v.Id_candid;
                aggregat[n_aggregat].total_votes = 0;
                j = n_aggregat;
                n_aggregat++;
            }
            aggregat[j].total_votes++;

            // Chercher ou créer l'entrée globale par candidat dans aggregat_global
            trouve_global = 0;
            for (k = 0; k < n_global; k++) {
                if (aggregat_global[k].id_candid == v.Id_candid) {
                    trouve_global = 1;
                    break;
                }
            }
            if (!trouve_global) {
                aggregat_global[n_global].id_candid = v.Id_candid;
                aggregat_global[n_global].total_votes = 0;
                k = n_global;
                n_global++;
            }
            aggregat_global[k].total_votes++;
        }
    }
    fclose(fp);

    // Étape 2 : afficher les résultats département par département
    for (i = 0; i < n_dept; i++) {
        ResultatDeptCandidat temp[MAX_RESULTATS];
        float pct[MAX_RESULTATS];
        int n_temp = 0;
        int total_dept = aggregat_dept[i].total_non_blancs + aggregat_dept[i].total_blancs;
        int a, b;

        // Extraire les candidats de ce département
        for (j = 0; j < n_aggregat; j++) {
            if (strcmp(aggregat[j].departement, aggregat_dept[i].departement) == 0) {
                temp[n_temp] = aggregat[j];
                n_temp++;
            }
        }

        // Calculer les pourcentages (formule du PDF)
        for (j = 0; j < n_temp; j++) {
            pct[j] = (total_dept > 0) ? (100.0 * temp[j].total_votes / total_dept) : 0.0;
        }

        // Tri à bulles par pourcentage décroissant
        for (a = 0; a < n_temp - 1; a++) {
            for (b = 0; b < n_temp - 1 - a; b++) {
                if (pct[b] < pct[b + 1]) {
                    ResultatDeptCandidat tmp_r = temp[b];
                    float tmp_p = pct[b];

                    temp[b] = temp[b + 1];
                    temp[b + 1] = tmp_r;

                    pct[b] = pct[b + 1];
                    pct[b + 1] = tmp_p;
                }
            }
        }

        printf("\nDépartement : %s (votes non blancs : %d, votes blancs : %d)\n",
            aggregat_dept[i].departement, aggregat_dept[i].total_non_blancs, aggregat_dept[i].total_blancs);
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        printf("| Candidat (Nom Prénom)          | Votes obtenus | Pourcentage |\n");
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

        for (j = 0; j < n_temp; j++) {
            if (lire_candid(CANDID_FILE, temp[j].id_candid, &c))
                printf("| %-14s %-14s | %13d | %10.2f%% |\n", c.Nom, c.Prenom, temp[j].total_votes, pct[j]);
        }
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    }

    // Étape 3 : résumé global par candidat, trié par total de votes décroissant
    for (i = 0; i < n_global - 1; i++) {
        for (j = 0; j < n_global - 1 - i; j++) {
            if (aggregat_global[j].total_votes < aggregat_global[j + 1].total_votes) {
                ResultatGlobal tmp_g = aggregat_global[j];
                aggregat_global[j] = aggregat_global[j + 1];
                aggregat_global[j + 1] = tmp_g;
            }
        }
    }

    printf("\nRésumé global par candidat (tout le pays) :\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("| Candidat (Nom Prénom)          | Total votes    |\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

    for (i = 0; i < n_global; i++) {
        if (lire_candid(CANDID_FILE, aggregat_global[i].id_candid, &c))
            printf("| %-14s %-14s | %14d |\n", c.Nom, c.Prenom, aggregat_global[i].total_votes);
    }
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}