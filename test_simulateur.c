#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include "simulateur.h"

#include "consigne.h"
#include "regulation.h"
#include "visualisationC.h"
#include "visualisationT.h"

#define MAX 2000

// Voir le fichier trace.txt pour comprendre la baisse de température



int main()
{
	int choix; // Choix du mode de régulation
	printf("Choisissez le mode de régulation (1 pour tout ou rien, 2 pour PID)\n");
	scanf("%d", &choix);

	switch (choix)
	{
	case 1:
		choix = 1;
		break;
	case 2 :
		choix = 2;
		break;
	default:
		main(); // Si l'utilisateur entre une valeur incorrecte, on lui redemande de choisir
		break;
	}

	temp_t temperature;			   // initialisation de la température
	temperature.exterieure = 10.0; // initialisation de la température extérieure à 14.0°C
	temperature.interieure = 16.0; // initialisation de la température intérieure à 15.0°C

	float csgn = 15; // Température voulue
	int nT = 0;		// Nombre de mesures effectuées
	float *T = malloc(MAX * sizeof(float));	// Tableau pour stocker les températures mesurées

	float cmd = 0; // initialisation de la commande du chauffage à 0%

	struct simParam_s *monSimulateur = simConstruct(temperature); // creation du simulateur, puissance intialisé

	while (1)
	{

		//  Lecture de la consigne
		csgn = consigne(csgn);

		// Condition d'arrêt : consigne minimale = 5°C
        if (csgn <= 5.0f)
        {
            printf("Consigne minimale atteinte, arrêt du programme...\n");
            csgn = 0.0; // Puissance à 0% avant de quitter
            simDestruct(monSimulateur); // destruction de simulateur
            return 0;
        }

		T[nT] = temperature.interieure;

		int nb_mesures = nT + 1;

		cmd = regulationTest(choix, csgn, T, nb_mesures);

		//  Visualisation
		visualisationC(cmd);
		visualisationT(temperature);

		printf("Consigne: %.2f | Actuelle: %.2f | Commande: %.2f%% (n=%d)\n", csgn, temperature.interieure, cmd, nb_mesures);

		temperature = simCalc(cmd, monSimulateur);

		usleep(40000);
		//T++; // On avance dans le tableau pour stocker la prochaine température
		nT++;
	}

	printf("Il ne reste plus qu'a allumer le chauffage !\n");

	simDestruct(monSimulateur); // destruction de simulateur
	return 0;
}

