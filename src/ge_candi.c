#include <stdio.h>
#include "election.h"

/**
 * ins_cand - Inscrit un nouveau candidat dans le fichier candid.dat
 * @param c Pointeur vers la structure Candidat a enregistrer
 * @return 0 si succes, -1 en cas d'erreur (ex: Id_candid deja existant)
 */
int ins_cand(const Candidat *c)
{
    /* TODO: verifier l'unicite de id_candid puis ecrire dans FICHIER_CANDID */
    return 0;
}

/**
 * modif_cand - Modifie les donnees d'un candidat existant.
 * @param id_candid Identifiant du candidat a modifier
 * @param c Nouvelles valeurs (le champ id_candid est ignore)
 * @return 0 si succes, -1 si le candidat n'existe pas
 */
int modif_cand(const char *id_candid, const Candidat *c)
{
    /* TODO: localiser id_candid dans FICHIER_CANDID et mettre a jour les champs */
    return 0;
}
