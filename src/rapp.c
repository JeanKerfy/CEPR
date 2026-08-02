#include <stdio.h>
#include "election.h"

/**
 * List_Electeurs - Affiche la liste des electeurs par departement et par
 * bureau de vote (Departement, Id_BV, NINU, Nom, Prenom, Adresse).
 */
void List_Electeurs(void)
{
    /* TODO: parcourir FICHIER_ELECT, croiser avec FICHIER_BV pour le departement,
       et afficher sous forme de tableau trie */
}

/**
 * List_Candid - Affiche la liste des candidats a la presidence
 * (Nom, Prenom, NINU, Type).
 */
void List_Candid(void)
{
    /* TODO: parcourir FICHIER_CANDID et afficher sous forme de tableau */
}

/**
 * List_BV - Affiche la liste des bureaux de vote
 * (Id_BV, Adresse, Nom et prenom des membres du BV).
 */
void List_BV(void)
{
    /* TODO: parcourir FICHIER_BV et afficher sous forme de tableau */
}

/**
 * Resultats - Affiche les resultats du scrutin par departement et par
 * candidat (Departement, Nom, Prenom, total votes, pourcentage), avec le
 * total des votes blancs/non blancs par departement, trie par pourcentage
 * decroissant, plus un resume global par candidat pour tout le pays.
 */
void Resultats(void)
{
    /* TODO: agreger FICHIER_VOTES par departement et par candidat,
       calculer les pourcentages, trier et afficher */
}
