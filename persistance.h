#ifndef PERSISTANCE_H_INCLUDED
#define PERSISTANCE_H_INCLUDED
#include "structures.h"

void enregistrer_log(const char *msg, int id);
void sauvegarder_systeme(Capteur *c);
void charger_systeme(Capteur *c);


#endif // PERSISTANCE_H_INCLUDED
