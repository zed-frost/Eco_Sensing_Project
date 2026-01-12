#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "structures.h"
#include "logique.h"
#include "persistance.h"

int main() {
    srand(time(NULL));
    Capteur mon_capteur;


    // 1. Initialisation et chargement de l'état précédent
    initialiser_capteur(&mon_capteur);
    charger_systeme(&mon_capteur);

    printf("--- Lancement du Capteur Autonome ---\n");
    printf("Position: (%.1f, %.1f) | Batterie initiale: %.2f%%\n\n",
            mon_capteur.x, mon_capteur.y, mon_capteur.batterie);

    // 2. Boucle infinie tant qu'il reste de l'énergie
    int tour = 1;
    while (mon_capteur.batterie > 0) {
        printf("[Tour %d] ", tour++);

        produire_paquet(&mon_capteur);
        simuler_envoi(&mon_capteur);

        printf("Batterie restante: %.2f%% | Paquets en file: %d\n",
                mon_capteur.batterie, mon_capteur.buffer_usage);

        // Optionnel : un petit délai pour ne pas que la console défile trop vite
        // _sleep(500); // Sur Windows (ms) ou usleep(500000) sur Linux
    }

    printf("\n--- ARRET DU SYSTEME : Batterie vide ---\n");
    printf("Nombre total d'envois effectues avant la panne : %d\n", mon_capteur.nb_envois);

    return 0;
}
