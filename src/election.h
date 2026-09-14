/*--------------------------------------------------------------------------*/
/* FICHIER:                         election.h                              */
/*AUTEUR:                           EMPR                                    */
/*DATE DE CREATION:                 29/07/2026                              */
/*DATE DE MODIFICATION:             10/09/2026                              */
/*DESCRIPTION:                      fichier contenant les nouveaux types de */
/*                                  données, les prototypes, les structures */
/*                                  de données et les constantes            */
/*--------------------------------------------------------------------------*/

#ifndef ELECTION_H
#define ELECTION_H
#include <time.h>
#include <math.h>

// Fichiers de données
#define PP_FILE "p_pol.dat"
#define BV_FILE "B_votes.dat"
#define ELECT_FILE "elect.dat"
#define CANDID_FILE "candid.dat"
#define VOTES_FILE "votes.dat"
#define MAX_CARACTERE 1000
#define MAX_RESULTATS 500
#define BACKGROUND "flag.png"

//structure pour la date
typedef struct {
    int jour;
    int mois;
    int annee;
} Date;

//structure pour l'adresse
typedef struct {
    char no_rue[5];
    char rue[50];
    char ville[30];
    char departement[15];
} Adresse;

//structure Parti Politique
typedef struct {
    int Id_PP;
    char Nom[50];
    Date Date_creat;
    Date Date_ins;
    Adresse Adr;
    char Responsable[64];
    char Telephone[20];
    char Email[50];
} PartiPolitique;

//structure Bureau de Vote
typedef struct {
    int Id_BV;
    Adresse Adresse_BV;
    int Id_President;//
    int Id_VPresident;//
    int Id_Secret;//
} BureauVote;

//structure Electeur
typedef struct {
    int NINU;//
    char Nom[50];
    char Prenom[50];
    Adresse Adr;
    int Id_BV;
} Electeur;

//structure Candidat
typedef struct {
    int Id_candid;
    char Nom[50];
    char Prenom[50];
    Date Dat_Nais;
    Adresse Adr;
    char Email[50];
    char Telephone[20];
    int type; // si 0 indépendant, sinon Id_PP du parti
    Date Date_ins;
} Candidat;

//structure Vote
typedef struct {
    int Id_vote;//
    Date Date_vote;
    int Id_BV;
    int NINU;
    int Id_candid; // 0 si vote blanc
} Vote;

typedef struct {
    char departement[50];
    int id_candid;
    int total_votes;
} ResultatDeptCandidat;

typedef struct {
    char departement[50];
    int total_blancs;
    int total_non_blancs;
} ResultatDept;

typedef struct {
    int id_candid;
    int total_votes;
} ResultatGlobal;

// --- Prototypes Module Partis Politiques ---
int obtenir_dernier_id_PP(char* nom_fichier);
int ecrire_PP(char* nom_fichier, PartiPolitique* pp);
int lire_PP(char* nom_fichier, int id_recherche, PartiPolitique* resultat);
int ins_PP(PartiPolitique* pp);
int modif_PP(PartiPolitique* pp);

// --- Prototypes Module Bureaux de Votes ---
int obtenir_dernier_id_BV(char* nom_fichier);
int ecrire_BV(char* nom_fichier, BureauVote* bv);
int lire_BV(char* nom_fichier, int id_recherche, BureauVote* resultat);
int ajout_BV(BureauVote* bv);
int modif_BV(BureauVote* bv);

// --- Prototypes Module Electeurs ---
int ecrire_elect(char* nom_fichier, Electeur* e);
int lire_elect(char* nom_fichier, int ninu_recherche, Electeur* resultat);
int ins_elect(Electeur* e);
int modif_elect(Electeur* e);

// --- Prototypes Module Candidats ---
int obtenir_dernier_id_candid(char* nom_fichier);
int ecrire_candid(char* nom_fichier, Candidat* c);
int lire_candid(char* nom_fichier, int id_recherche, Candidat* resultat);
int ins_cand(Candidat* c);
int modif_cand(Candidat* c);

// --- Prototypes Module Votes ---
int obtenir_dernier_id_vote(char* nom_fichier);
int ecrire_vote(char* nom_fichier, Vote* v);
int ins_vote(Vote* v);
void generer_date_courante(Date* d);

// --- Prototypes Module Rapports et Résultats ---
void List_Electeurs(void);
void List_Candid(void);
void List_BV(void);
void Resultats(void);

/*Prototypes des fonctions pour les menus */
void afficher_menu_principal(void);
void menu_parti(void);
void menu_BV(void);
void menu_elect(void);
void menu_candid(void);
void menu_votes(void);
void menu_rapports(void);

//prototype de la fonction date_valide
int date_valide(Date d);

//prototype de la fonction age_valide
int age_valide(Date d);

#endif //ELECTION_H