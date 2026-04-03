#ifndef COMMANDE_H
#define COMMANDE_H

#include "ftd2xx.h"

void commande(float puissance_f, FT_HANDLE ftHandle);
unsigned char calculer_octet_commande(float puissance_f);

#endif