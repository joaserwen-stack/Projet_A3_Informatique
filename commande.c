#include "commande.h"
#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include "ftd2xx.h"

unsigned char calculer_octet_commande(float puissance_f)
{
    if (puissance_f < 0.0f)  puissance_f = 0.0f;
    if (puissance_f > 33.0f) puissance_f = 33.0f;

    // Conversion 0-33 → 0-127
    uint8_t puis = (uint8_t)((puissance_f * 127.0f) / 33.0f);

    // Seuil sur puis : < 63 = froid (bit7=1), >= 63 = chauffage (bit7=0)
    uint8_t mode = (puis < 63) ? 1 : 0;

    // Construction de l'octet
    unsigned char octet = (mode << 7) | (puis & 0x7F);

    printf("DEBUG => puissance=%.2f puis=%d mode=%s octet=0x%02X\n",
           puissance_f, puis, mode ? "FROID" : "CHAUFFAGE", octet);

    return octet;
}

void commande(float puissance_f, FT_HANDLE ftHandle)
{
    DWORD ecrits;
    uint8_t cmd = calculer_octet_commande(puissance_f);

    char ordre = 'c';

    // on annonce qu'on va envoyer une commande
    FT_Write(ftHandle, &ordre, 1, &ecrits);

    // puis on envoie l'octet puissance
    FT_Write(ftHandle, &cmd, 1, &ecrits);
}