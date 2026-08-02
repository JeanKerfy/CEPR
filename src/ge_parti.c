#include <stdio.h>
#include "election.h"

/**
 * ins_PP - Enregistre un nouveau parti politique dans le fichier p_pol.dat
 * @param pp Pointeur vers la structure PartiPolitique a enregistrer
 * @return 0 si succes, -1 en cas d'erreur (ex: Id_PP deja existant)
 */
int ins_PP(const PartiPolitique *pp)
{
    /* TODO: verifier l'unicite de id_PP puis ecrire dans FICHIER_PARTIS */
    return 0;
}

/**
 * modif_PP - Modifie les donnees d'un parti politique existant.
 * L'Id_PP ne peut jamais etre modifie.
 * @param id_PP Identifiant du parti politique a modifier
 * @param pp Nouvelles valeurs (le champ id_PP est ignore)
 * @return 0 si succes, -1 si le parti n'existe pas
 */
int modif_PP(const char *id_PP, const PartiPolitique *pp)
{
    /* TODO: localiser id_PP dans FICHIER_PARTIS et mettre a jour les champs (sauf id_PP) */
    return 0;
}
