#ifndef MESA_FUNCOES_H_
#define MESA_FUNCOES_H_

#include "stm32f1xx_hal.h"

void TelaInicial();
void TelaModo();
void TelaCalib1();
void TelaCalib2();
void TelaRun();
void TelaAngulo1(int angulo1);

void Passo(int motor, int direcao);

#endif 
