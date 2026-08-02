#pragma once

/*
 * election.h
 * Structures de donnees, types et constantes globales du projet CEPR
 * (Conseil Electoral Provisoire - gestion des elections presidentielles)
 */

/* ----------------- Constantes globales ----------------- */
#define TAILLE_ID       32
#define TAILLE_NOM      64
#define TAILLE_DATE     16   /* format JJ/MM/AAAA */
#define TAILLE_TEL      20   /* format specifique impose */
#define TAILLE_EMAIL    64
#define TAILLE_RUE      64
#define TAILLE_DEPT     32

/* ----------------- Sous-structure Adresse -----------------
 * Reutilisee par PartiPolitique, Electeur, Candidat, BureauVote
 */
typedef struct
{
    int  no_rue;
    char rue[TAILLE_RUE];
    char departement[TAILLE_DEPT];
} Adresse;

/* ----------------- Module Partis Politiques ----------------- */
typedef struct
{
    char    id_PP[TAILLE_ID];
    char    nom[TAILLE_NOM];
    char    date_creat[TAILLE_DATE];
    char    date_ins[TAILLE_DATE];
    Adresse adresse;
    char    responsable[TAILLE_NOM];
    char    telephone[TAILLE_TEL];
    char    email[TAILLE_EMAIL];
} PartiPolitique;

/* ----------------- Module Bureaux de Votes ----------------- */
typedef struct
{
    char    id_BV[TAILLE_ID];
    Adresse adresse_BV;
    char    id_president[TAILLE_ID];
    char    id_vpresident[TAILLE_ID];
    char    id_secret[TAILLE_ID];
} BureauVote;

/* ----------------- Module Electeurs ----------------- */
typedef struct
{
    char    NINU[TAILLE_ID];
    char    nom[TAILLE_NOM];
    char    prenom[TAILLE_NOM];
    Adresse adresse;
    char    id_BV[TAILLE_ID];
} Electeur;

/* ----------------- Module Candidats -----------------
 * type = 0 si independant, sinon id_PP du parti politique
 */
typedef struct
{
    char    id_candid[TAILLE_ID];
    char    nom[TAILLE_NOM];
    char    prenom[TAILLE_NOM];
    char    dat_nais[TAILLE_DATE];
    Adresse adresse;
    char    email[TAILLE_EMAIL];
    char    telephone[TAILLE_TEL];
    char    type[TAILLE_ID];
    char    date_ins[TAILLE_DATE];
} Candidat;

/* ----------------- Module Votes ----------------- */
typedef struct
{
    char id_vote[TAILLE_ID];
    char date_vote[TAILLE_DATE];
    char id_BV[TAILLE_ID];
    char NINU[TAILLE_ID];
    char id_candid[TAILLE_ID]; /* vide/"0" = vote blanc */
} Vote;

/* ----------------- Noms des fichiers de donnees ----------------- */
#define FICHIER_PARTIS   "p_pol.dat"
#define FICHIER_BV       "B_votes.dat"
#define FICHIER_ELECT    "elect.dat"
#define FICHIER_CANDID   "candid.dat"
#define FICHIER_VOTES    "votes.dat"
