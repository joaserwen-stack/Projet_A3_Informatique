#include "releve.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <unistd.h>
#include <stdint.h>

// cette fonction permet d'acquérir les 24 bits du SOT
uint32_t acquisition_SOT(FT_HANDLE ftHandle)
{
    DWORD octets_lus, bytesAvailable, ecrits;
    uint8_t buffer[64];
    uint8_t q[6] = {0};
    uint8_t recu = 0;

    char cmd = 't';

    // Demande de mesure
    FT_Write(ftHandle, &cmd, 1, &ecrits);

    // Attendre que la carte envoie quelque chose
    do {
        FT_GetQueueStatus(ftHandle, &bytesAvailable);
        Sleep(1);
    } while (bytesAvailable == 0);

    // Lire tout le buffer
    if (bytesAvailable > sizeof(buffer))
        bytesAvailable = sizeof(buffer);

    FT_Read(ftHandle, buffer, bytesAvailable, &octets_lus);

    // Parcours du buffer
    for (int i = 0; i < octets_lus; i++)
    {
        uint8_t marqueur = (buffer[i] >> 4) & 0x0F;
        uint8_t data     = buffer[i] & 0x0F;

        switch (marqueur)
        {
            case 0x0: q[0] = data; recu++; break;
            case 0x1: q[1] = data; recu++; break;
            case 0x2: q[2] = data; recu++; break;

            case 0x8: q[3] = data; recu++; break;
            case 0x9: q[4] = data; recu++; break;
            case 0xA: q[5] = data; recu++; break;
        }

        if (recu == 6)
            break;
    }

    return ((uint32_t)q[0] << 20) +
           ((uint32_t)q[1] << 16) +
           ((uint32_t)q[2] << 12) +
           ((uint32_t)q[3] << 8)  +
           ((uint32_t)q[4] << 4)  +
            (uint32_t)q[5];
}

float convertir_SOT_en_temp(uint16_t SOT)
{
    // Formule donnee dans le cahier de charge
    return -39.64f + 0.04f * (float)SOT;
}

temp_t releve(temp_t myTemp, FT_HANDLE ftHandle)
{

    uint32_t SOT24 = acquisition_SOT(ftHandle); // variable pour stocker les 24 bits du SOT

    uint16_t SOT_ext = (SOT24 >> 12) & 0xFFF; // extraction des 12 premiers bits pour la température extérieure
    uint16_t SOT_int = SOT24 & 0xFFF;         // extraction des 12 derniers bits pour la température intérieure

    myTemp.exterieure = convertir_SOT_en_temp(SOT_ext); // conversion du SOT extérieur en température extérieure
    myTemp.interieure = convertir_SOT_en_temp(SOT_int); // conversion du SOT intérieur en température intérieure

    return myTemp;
}