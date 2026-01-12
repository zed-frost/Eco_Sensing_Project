#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "logique.h"
#include "persistance.h"

void produire_paquet(Capteur *c) {
    // ÉTAPE A : Gérer le buffer plein (FIFO)
    if (c->buffer_usage >= 5) {
        Paquet *a_supprimer = c->buffer_tete;
        c->buffer_tete = c->buffer_tete->suivant;

        // Correction : ajout de \n et du ;
        printf("\n[!!!] Memoire saturee, suppression du paquet %d\n", a_supprimer->id);

        enregistrer_log("MEMOIRE_SATUREE_SUPPRESSION", a_supprimer->id);

        free(a_supprimer);
        c->buffer_usage--;
    }

    // ÉTAPE B : Création du nouveau paquet
    Paquet *nouveau = malloc(sizeof(Paquet));
    if (nouveau == NULL) return;

    c->dernier_id++;
    nouveau->id = c->dernier_id;
    nouveau->valeur = (float)(rand() % 100);
    nouveau->timestamp = (long)time(NULL);
    nouveau->suivant = NULL;

    // ÉTAPE C : Ajout à la fin de la liste
    if (c->buffer_tete == NULL) {
        c->buffer_tete = nouveau;
    } else {
        Paquet *temp = c->buffer_tete;
        while (temp->suivant != NULL) {
            temp = temp->suivant;
        }
        temp->suivant = nouveau;
    }
    c->buffer_usage++;

    // On logue la création réussie
    enregistrer_log("PAQUET_CREE_DANS_BUFFER", nouveau->id);

    // ÉTAPE D : TRÈS IMPORTANT - On met à jour le fichier binaire
    sauvegarder_systeme(c);
}



void simuler_envoi(Capteur *c) {
    // Si on n'a pas encore 5 paquets, on ne fait rien (Phase d'accumulation)
    if (c->buffer_usage < 5) return;

    // Calcul du coût énergétique
    float d = sqrt(pow(c->x, 2) + pow(c->y, 2));
    float cout = 0.05 + 0.01 * pow(d, 2);

    // --- PHASE 1 : Premier envoi (on vide tout ce qui a été accumulé) ---
    if (c->nb_envois == 0) {
        Paquet *temp = c->buffer_tete;
        while (temp != NULL) {
            if (c->batterie >= cout) {
                c->batterie -= cout;
                c->nb_envois++;
                enregistrer_log("ENVOI_INITIAL", temp->id);
            } else {
                // ÉNERGIE INSUFFISANTE : On force l'arrêt du système
                printf("\n[!!!] PANNE : Batterie trop faible pour l'envoi initial (ID: %d)\n", temp->id);
                c->batterie = 0;
                sauvegarder_systeme(c);
                return; // On sort immédiatement
            }
            temp = temp->suivant;
        }
    }
    // --- PHASE 2 : Régime de croisière (envoi du dernier paquet produit) ---
    else {
        Paquet *temp = c->buffer_tete;
        // On va jusqu'au dernier paquet de la liste (le plus récent)
        while (temp->suivant != NULL) {
            temp = temp->suivant;
        }

        if (c->batterie >= cout) {
            c->batterie -= cout;
            c->nb_envois++;
            enregistrer_log("ENVOI_CROISIERE", temp->id);
        } else {
            // ÉNERGIE INSUFFISANTE : On force l'arrêt du système
            printf("\n[!!!] PANNE : Batterie trop faible pour l'envoi de croisiere (ID: %d)\n", temp->id);
            c->batterie = 0;
        }
    }

    // On sauvegarde l'état (batterie diminuée et nb_envois augmenté)
    sauvegarder_systeme(c);
}
