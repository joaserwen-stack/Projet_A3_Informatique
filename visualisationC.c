/*#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include "visualisationC.h"

void visualisationC(float puissance_f)
{
   // 1. On regarde si Python est déjà en train de lire le fichier
   FILE *test_verrou = fopen(".verrouData", "r");
   if (test_verrou != NULL)
   {
      fclose(test_verrou); 
      return; // Python bosse, on abandonne pour ce tour
   }

   // 2. On crée le verrou et on le referme IMMÉDIATEMENT (Spécial Windows)
   FILE *verrou = fopen(".verrouData", "w");
   if (verrou != NULL) {
       fclose(verrou); // La ligne magique anti-crash !
   }
   
   // 3. LES VARIABLES (C'est ce qui avait disparu !)
   float int_temp = 0.0, ext_temp = 0.0; 
   char statut[6]; 

   // 4. Lecture des températures actuelles
   FILE *f_lecture = fopen("data.txt", "r");
   if (f_lecture != NULL) {
       fscanf(f_lecture, "%s", statut);    
       fscanf(f_lecture, "%f", &int_temp); 
       fscanf(f_lecture, "%f", &ext_temp); 
       fclose(f_lecture);
   }

   // 5. Écriture de la nouvelle commande
   FILE *data = fopen("data.txt", "w");
   if (data != NULL) {
       if (puissance_f > 0.0) {
          fprintf(data, "true\n");
       } else {
          fprintf(data, "false\n");
       }
       fprintf(data, "%.2f\n", int_temp);
       fprintf(data, "%.2f\n", ext_temp);
       fclose(data);
   }

   // 6. On efface le verrou à la fin
   remove(".verrouData"); 
}


*/







#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include "visualisationC.h"

void visualisationC(float puissance_f)
{
   FILE *test_verrou = fopen(".verrouData", "r");// Vérification de l'existence du verrou pour éviter les conflits d'accès
   if (test_verrou != NULL)// Si le verrou existe, cela signifie qu'une opération est déjà en cours, on ferme le fichier et on quitte la fonction
   {// Fermeture du fichier de verrou pour éviter les conflits d'accès
      fclose(test_verrou); // une opération est déjà en cours
      return; // on quitte la fonction pour éviter d'écrire dans le fichier pendant qu'il est utilisé
   }// Création du verrou pour signaler que le fichier est en cours d'utilisation

   FILE *verrou = fopen(".verrouData", "w");// Création du verrou pour signaler que le fichier est en cours d'utilisation
   if (verrou != NULL){
      fclose(verrou); 
   }
   
   float int_temp = 0.0, ext_temp = 0.0; // Variables pour stocker les températures intérieure et extérieure
   char statut[6]; // Variable pour stocker le statut de la consigne (true/false)

   // on sait que le fichier ne contient que trois ligne que l'on souvegade
   FILE *f_lecture = fopen("data.txt", "r");

   fscanf(f_lecture, "%s", statut);    // On lit la ligne 1 pour avoir acces au variable dans l'ordre
   fscanf(f_lecture, "%f", &int_temp); // On sauvegarde Tint
   fscanf(f_lecture, "%f", &ext_temp); // On sauvegarde Text
   fclose(f_lecture);

   

   // Ouverture en mode ecriture ce qui ecrase l'ancienne version
   FILE *data = fopen("data.txt", "w");

   // ici on regarde la nouvelle valeur de etat grace a puissance
   if (puissance_f > 0.0)
   {
      fprintf(data, "true\n");
   }
   else
   {
      fprintf(data, "false\n");
   }

   // on rajoute les valeurs extraite plus haut
   fprintf(data, "%.2f\n", int_temp);
   fprintf(data, "%.2f\n", ext_temp);

   fclose(data);


   remove(".verrouData"); // suppression du verrou pour signaler que le fichier est disponible
}
