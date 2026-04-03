#include "regulation.h"

#define deltat 10.0 // Intervalle de temps entre les mesures en secondes
#define KP 0.4
#define KI 0.01
#define KD 0.1




float regulation_tout_ou_rien(float consigne, float *tabT, int nT) // Cette fonction implémente une régulation tout ou rien
{
    float com = 0.0; // Commande à retourner

    for (int i = 0; i < nT; i++) {
        if (tabT[i] < consigne) {
            // Chauffage activé à 50% si en dessous de la consigne
            com = 50.0; 
        } else {
            // Chauffage arrêté si la consigne est atteinte ou dépassée
            com = 0.0;
        }
    }

    return com; 
}

float regulation_PID(float consigne, float* tabT, int nT) {

    if (nT < 1) {
        return 0.0;
    }

    // Proportionnel
    float erreur_actuelle = consigne - tabT[nT - 1];
    float P = KP * erreur_actuelle; 

    // Intégral
    float integral = 0.0;
    if (nT > 1) { 
        int i = 0;
        do {
            float err1 = consigne - tabT[i];
            float err2 = consigne - tabT[i + 1];
            integral += ((err1 + err2) * deltat) / 2.0;
            i++;
        } while (i < nT - 1);
    }
    float I = KI * integral;

    //  Dérivé
    float derivative = 0.0;
    if (nT > 1) { 
        float erreur_precedente = consigne - tabT[nT - 2];
        derivative = (erreur_actuelle - erreur_precedente) / deltat;
    }
    float D = KD * derivative;

    //Calcul final
    float cmd = P + I + D;
    
    //Saturation
    if (cmd > 100.0){
        
     cmd = 100.0;
    }
    if (cmd < 0.0) 
    {
        cmd = 0.0;
    }

    return cmd;
}

float regulationTest(int regul, float consigne, float *tabT, int nT)
{
	float cmd;

	switch (regul)
	{
		case 1:
			cmd = regulation_tout_ou_rien( consigne, tabT,  nT);
			break;

		case 2:
			cmd = regulation_PID(consigne, tabT, nT);
			break;

	}
	return cmd;
	
}