#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>
#include "ftd2xx.h"

#include "releve.h"
#include "commande.h"
#include "consigne.h"
#include "regulation.h"
#include "visualisationC.h"
#include "visualisationT.h"

#define MAX 2000

int main()

{
    // 1. DÉCLARATION DES VARIABLES
    FT_HANDLE ftHandle;
    FT_STATUS ftStatus;

    float csgn; // Valeur par défaut
    float cmd = 0.0;
    int nT = 0;
    float T[MAX];
    temp_t temperature;
    temperature.exterieure = 10.0; // Température extérieure initiale
    temperature.interieure = 10.0; // Température intérieure initiale

    int choix; // Choix du mode de régulation
    printf("Choisissez le mode de régulation (1 pour tout ou rien, 2 pour PID)\n");
    scanf("%d", &choix);

    switch (choix)
    {
    case 1:
        choix = 1;
        break;
    case 2:
        choix = 2;
        break;
    default:
        main(); // Si l'utilisateur entre une valeur incorrecte, on lui redemande de choisir
        break;
    }

    // 3. INITIALISATION USB FTD2XX
    printf("Connexion à la carte USB...\n");
    ftStatus = FT_Open(0, &ftHandle);
    if (ftStatus != FT_OK)
    {
        printf("Erreur : Impossible d'ouvrir la carte FTDI (code %d)\n", (int)ftStatus);
        return EXIT_FAILURE;
    }

    // Configuration du port (Vérifie si ta carte est en 9600 ou 115200)
    FT_SetBaudRate(ftHandle, 115200);
    FT_SetDataCharacteristics(ftHandle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE);
    FT_SetFlowControl(ftHandle, FT_FLOW_NONE, 0, 0); // Indispensable
    FT_SetTimeouts(ftHandle, 1000, 1000);            // Timeouts 1s
    FT_SetLatencyTimer(ftHandle, 2);                 // Réactivité max (2ms)
    FT_Purge(ftHandle, FT_PURGE_RX | FT_PURGE_TX);   // Évite que le code freeze si on débranche

    printf("Régulation lancée. Appuyez sur Ctrl+C pour arrêter.\n");

    
    FT_Purge(ftHandle, FT_PURGE_RX | FT_PURGE_TX);


    while (1) // Boucle infinie, l'arrêt est géré par la consigne
    {
        temperature = releve(temperature, ftHandle);
        T[nT] = temperature.interieure;

        csgn = consigne(csgn);

        // Condition d'arrêt : consigne minimale = 5°C
        if (csgn <= 5.0f)
        {
            printf("Consigne minimale atteinte, arrêt du programme...\n");
            commande(0.0f, ftHandle); // Puissance à 0% avant de quitter
            FT_Purge(ftHandle, FT_PURGE_RX | FT_PURGE_TX);
            FT_Close(ftHandle);
            return 0;
        }

        cmd = regulation_PID(csgn, T, nT + 1);
        commande(cmd, ftHandle);

        visualisationC(cmd);
        visualisationT(temperature);

        printf("Consigne: %.2f | Mesure: %.2f | PID: %.2f\n",
               csgn, temperature.interieure, cmd);

        Sleep(40);

        if (nT < MAX - 1){
            nT++; // On protège le tableau T sans bloquer la boucle
        }
    }

    commande(0.0f, ftHandle);
    FT_Purge(ftHandle, FT_PURGE_RX | FT_PURGE_TX);
    FT_Close(ftHandle);
    return 0;
}
