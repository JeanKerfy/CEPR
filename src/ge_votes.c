#include <stdio.h>
#include "election.h"

/**
 * enregistrer_vote - Enregistre le vote d'un electeur dans le fichier votes.dat.
 * La date du vote est saisie automatiquement a partir de la date systeme.
 * Si id_candid est vide ou "0", le vote est considere comme blanc.
 * @param v Pointeur vers la structure Vote a enregistrer
 * @return 0 si succes, -1 en cas d'erreur (ex: electeur a deja vote)
 */
int enregistrer_vote(const Vote *v)
{
    /* TODO: verifier que l'electeur n'a pas deja vote, dater automatiquement,
       puis ecrire dans FICHIER_VOTES */
    return 0;
}
