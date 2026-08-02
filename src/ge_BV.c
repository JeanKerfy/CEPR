#include <stdio.h>
#include "election.h"

/**
 * ajout_BV - Ajoute un nouveau bureau de vote dans le fichier B_votes.dat
 * @param bv Pointeur vers la structure BureauVote a enregistrer
 * @return 0 si succes, -1 en cas d'erreur (ex: Id_BV deja existant)
 */
int ajout_BV(const BureauVote *bv)
{
    /* TODO: verifier l'unicite de id_BV puis ecrire dans FICHIER_BV */
    return 0;
}

/**
 * modif_BV - Modifie les donnees d'un bureau de vote existant.
 * @param id_BV Identifiant du bureau de vote a modifier
 * @param bv Nouvelles valeurs (le champ id_BV est ignore)
 * @return 0 si succes, -1 si le bureau n'existe pas
 */
int modif_BV(const char *id_BV, const BureauVote *bv)
{
    /* TODO: localiser id_BV dans FICHIER_BV et mettre a jour les champs */
    return 0;
}
