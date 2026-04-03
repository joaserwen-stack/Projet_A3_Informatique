    #include <stdio.h>
	#include <stdlib.h>
	#include "define.h" 
    
   float regulationTest(int regul,float consigne,float* tabT, int nT);
    float regulation_tout_ou_rien(float consigne, float* tabT, int nT);
    float regulation_PID(float consigne, float* tabT, int nT);