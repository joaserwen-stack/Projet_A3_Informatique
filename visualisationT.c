/*#include <stdio.h>
#include "define.h"
#include "visualisationT.h"

void visualisationT(temp_t myTemp)
{
    // 1. On regarde si Python est déjà en train de lire le fichier
    FILE *test_verrou = fopen(".verrouData", "r");
    if (test_verrou != NULL)
    {
        fclose(test_verrou); 
        return; //  
    }

    // 2. On crée le verrou et on le referme 
    FILE *verrou = fopen(".verrouData", "w");
    if (verrou != NULL) {
        fclose(verrou);
    }
    
    // 3. LA VARIABLE
    char temoin[10] = "false"; 

    // 4. Lecture du statut actuel du chauffage
    FILE *f_lecture = fopen("data.txt", "r");
    if (f_lecture != NULL) {
        fscanf(f_lecture, "%s", temoin); 
        fclose(f_lecture);
    }

    // 5. Écriture des nouvelles températures
    FILE *data = fopen("data.txt", "w");
    if (data != NULL) {
        fprintf(data, "%s\n", temoin);
        fprintf(data, "%.2f\n", myTemp.interieure);
        fprintf(data, "%.2f\n", myTemp.exterieure);
        fclose(data);
    }

    // 6. On efface le verrou à la fin
    remove(".verrouData");
}

*/




#include <stdio.h>
#include "define.h"
#include "visualisationT.h"

void visualisationT(temp_t myTemp)
{
    FILE *test_verrou = fopen(".verrouData", "r");

    if (test_verrou != NULL)
    {
        fclose(test_verrou); // une opération est déjà en cours
        return;
    }

    FILE *verrou = fopen(".verrouData", "w");
    if (verrou != NULL){
        fclose(verrou);
    }
    
    char temoin[10] = "false"; // Initialisation du témoin à "false"
    FILE *f_lecture = fopen("data.txt", "r");

    fscanf(f_lecture, "%s", temoin); // On récupère la ligne 1
    fclose(f_lecture);

    // apres avoir fait la lecture il faut se rassurer que le fichier a bien ete fermé
    if (test_verrou != NULL)
    {
        fclose(test_verrou); // une opération est déjà en cours
        return;
    }

    FILE *data = fopen("data.txt", "w");

    fprintf(data, "%s\n", temoin);
    fprintf(data, "%.2f\n", myTemp.interieure);
    fprintf(data, "%.2f\n", myTemp.exterieure);

    fclose(data);

    remove(".verrouData");
}
