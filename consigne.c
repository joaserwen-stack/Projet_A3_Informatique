#include <stdio.h>
#include <stdlib.h>
#include "consigne.h"

float consigne(float thermostatPrec_f)
{
    FILE *test_verrou = fopen(".verrouConsigne", "r");
    
    if (test_verrou != NULL)
    {
        fclose(test_verrou); // une opération est déjà en cours 
        return thermostatPrec_f;// On retourne la valeur d'entree
    }
 
    float thermostat_f = thermostatPrec_f;
    FILE *consigne_file = fopen("consigne.txt", "r");

    if (consigne_file != NULL)
    {
      fscanf(consigne_file, "%f", &thermostat_f);//on lit la valeur dans le fichier
      fclose(consigne_file);
    }

    return thermostat_f;
}