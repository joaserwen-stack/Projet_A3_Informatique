#ifndef RELEVE_H
#define RELEVE_H

#include "define.h"
#include "ftd2xx.h"
#include <stdint.h>

uint32_t acquisition_SOT(FT_HANDLE ftHandle);
float convertir_SOT_en_temp(uint16_t SOT);
temp_t releve(temp_t myTemp, FT_HANDLE ftHandle);

#endif 