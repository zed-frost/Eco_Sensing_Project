#ifndef STRUCTURES_H_INCLUDED
#define STRUCTURES_H_INCLUDED

typedef struct Paquet {
    int id;
    float valeur;
    long timestamp;
    struct Paquet *suivant;
} Paquet;

typedef struct {
    float batterie;
    float x, y;
    Paquet *buffer_tete;
    int buffer_usage;
    int dernier_id;
    int nb_envois;
} Capteur;

void initialiser_capteur(Capteur *c);

#endif // STRUCTURES_H_INCLUDED
