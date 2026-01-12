#include "structures.h"
#include <stdlib.h>

void initialiser_capteur(Capteur *c)
{
    c->batterie = 100.0;
    c->x = 10.0;
    c->y = 10.0;
    c->buffer_tete = NULL;
    c->buffer_usage = 0;
    c->dernier_id = 0;
    c->nb_envois = 0;
}
