#include <stdio.h>
#include <stdlib.h>
#include "persistance.h"

void sauvegarder_systeme(Capteur *c)
{
    FILE *f = fopen("save.bin", "wb");
    if (f == NULL) return;

    fwrite(c, sizeof(Capteur), 1, f);

    Paquet *temp = c->buffer_tete;
    while (temp != NULL)
    {

        fwrite(temp, sizeof(Paquet) - sizeof(Paquet*), 1, f);
        temp = temp->suivant;
    }
    fclose(f);
}


void charger_systeme(Capteur *c)
{
    FILE *f = fopen("save.bin", "rb");
    if (f == NULL) return;

    // Étape A : On lit les infos du capteur
    fread(c, sizeof(Capteur), 1, f);

    // On prépare la reconstruction de la liste
    int quantite = c->buffer_usage;
    c->buffer_tete = NULL;
    c->buffer_usage = 0;

    // Étape B : Boucle pour recréer chaque paquet
    for (int i = 0; i < quantite; i++)
    {
        Paquet *nouveau = malloc(sizeof(Paquet));
        if (nouveau == NULL) break;

        // On lit les données depuis le fichier binaire
        fread(nouveau, sizeof(Paquet) - sizeof(Paquet*), 1, f);
        nouveau->suivant = NULL;

        // On raccroche le nouveau wagon à la fin de notre liste en mémoire
        if (c->buffer_tete == NULL)
        {
            c->buffer_tete = nouveau;
        }
        else
        {
            Paquet *temp = c->buffer_tete;
            while (temp->suivant != NULL) temp = temp->suivant;
            temp->suivant = nouveau;
        }
        c->buffer_usage++;
    }
    fclose(f);
}


void enregistrer_log(const char *msg, int id)
{
    FILE *f = fopen("log.txt", "a"); // "a" pour Append (ajouter à la fin)
    if (f == NULL) return;

    fprintf(f, "Evenement: %s | ID: %d\n", msg, id);
    fclose(f);
}


