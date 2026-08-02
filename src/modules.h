#pragma once
#include "election.h"

/* ge_parti.c */
int ins_PP(const PartiPolitique *pp);
int modif_PP(const char *id_PP, const PartiPolitique *pp);

/* ge_BV.c */
int ajout_BV(const BureauVote *bv);
int modif_BV(const char *id_BV, const BureauVote *bv);

/* ge_elect.c */
int ins_elect(const Electeur *e);
int modif_elect(const char *NINU, const Electeur *e);

/* ge_candi.c */
int ins_cand(const Candidat *c);
int modif_cand(const char *id_candid, const Candidat *c);

/* ge_votes.c */
int enregistrer_vote(const Vote *v);

/* rapp.c */
void List_Electeurs(void);
void List_Candid(void);
void List_BV(void);
void Resultats(void);
