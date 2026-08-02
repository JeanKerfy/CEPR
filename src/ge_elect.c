#include <stdio.h>
#include "election.h"

/**
 * ins_elect - Enregistre un nouvel electeur dans le fichier elect.dat
 * @param e Pointeur vers la structure Electeur a enregistrer
 * @return 0 si succes, -1 en cas d'erreur (ex: NINU deja existant)
 */
int ins_elect(const Electeur *e)
{
    /* TODO: verifier l'unicite du NINU puis ecrire dans FICHIER_ELECT */
    return 0;
}

/**
 * modif_elect - Modifie les donnees d'un electeur existant.
 * @param NINU Identifiant de l'electeur a modifier
 * @param e Nouvelles valeurs (le champ NINU est ignore)
 * @return 0 si succes, -1 si l'electeur n'existe pas
 */
int modif_elect(const char *NINU, const Electeur *e)
{
    /* TODO: localiser NINU dans FICHIER_ELECT et mettre a jour les champs */
    return 0;
}
