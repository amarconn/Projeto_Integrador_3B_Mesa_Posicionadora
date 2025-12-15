#include "mesa_funcoes.h"
#include "liquidcrystal_i2c.h"
#include "main.h"
#include "stdlib.h"

void TelaInicial()
{
	HD44780_Clear();
	HD44780_SetCursor(6, 0);
	HD44780_PrintStr("Mesa");
	HD44780_SetCursor(2, 1);
	HD44780_PrintStr("Posicionadora");

	HAL_Delay(2500);

	HD44780_Clear();
	HD44780_SetCursor(0, 0);
	HD44780_PrintStr("Aperte um botao");
	HD44780_SetCursor(2, 1);
	HD44780_PrintStr("para iniciar");
}

void TelaModo()
{
	HD44780_SetCursor(0, 0);
	HD44780_PrintStr("Escolha um modo:");
	HD44780_SetCursor(1, 1);
	HD44780_PrintStr("<-Calib  Run->");
}

void TelaCalib1()
{
	for(int x=0; x<15; x=x+1)
		{
			HD44780_ScrollDisplayRight();
		    HAL_Delay(50);
		}

	HD44780_Clear();
	HD44780_SetCursor(1, 0);
	HD44780_PrintStr("                             Motor:");
	HD44780_SetCursor(3, 1);
	HD44780_PrintStr("                         <- 1  2 ->");

	for(int x=0; x<15; x=x+1)
		{
			HD44780_ScrollDisplayRight();
			HAL_Delay(50);
		}
}

void TelaCalib2()
{
	HD44780_Clear();
	HD44780_SetCursor(1, 0);
	HD44780_PrintStr("Posicione no 0");
	HD44780_SetCursor(3, 1);
	HD44780_PrintStr("<-  OK  ->");

}

void TelaAngulo1(int angulo1)
{
	char snum1[4];
	itoa(angulo1, snum1, 10);

	int x = 0;
	if ((angulo1>9)|(angulo1<0)) x = 1;
	if ((angulo1<-9)|(angulo1>99)) x = 2;
	if (angulo1<-99) x = 3;

	HD44780_SetCursor(1, 0);
	HD44780_PrintStr("Angulo motor 1");

	HD44780_SetCursor(0, 1);
	HD44780_PrintStr(" <-");
	HD44780_SetCursor(9-x, 1);
	HD44780_PrintStr(snum1);
	HD44780_PrintSpecialChar(0xDF);
	HD44780_PrintStr("  -> ");



	if ((angulo1>=-99) && (angulo1<-9))
	{
		HD44780_SetCursor(6, 1);
		HD44780_PrintStr(" ");
	}
	if ((angulo1>=-9) && (angulo1<0))
	{
		HD44780_SetCursor(7, 1);
		HD44780_PrintStr(" ");
	}
	if (angulo1==0)
	{
		HD44780_SetCursor(8, 1);
		HD44780_PrintStr(" ");
	}

	if ((angulo1<100) && (angulo1>=10))
	{
		HD44780_SetCursor(7, 1);
		HD44780_PrintStr(" ");
	}
	if ((angulo1<10) && (angulo1>0))
	{
		HD44780_SetCursor(8, 1);
		HD44780_PrintStr(" ");
	}
}

void TelaRun()
{
	for(int x=0; x<15; x=x+1)
		{
			HD44780_ScrollDisplayLeft();
		    HAL_Delay(50);
		}

	HD44780_Clear();
	HD44780_SetCursor(0, 0);
	HD44780_PrintStr("               Numero de eixos?");
	HD44780_SetCursor(0, 1);
	HD44780_PrintStr("                <- 1      2 ->");

	for(int x=0; x<15; x=x+1)
		{
			HD44780_ScrollDisplayLeft();
			HAL_Delay(50);
		}
}

void Passo(int motor, int direcao)
{

	if (motor == 1)
	{
		if (direcao == 0) HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, 0);
		if (direcao == 1) HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, 1);

		HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, 1);
		HAL_Delay(2);
		HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, 0);
		HAL_Delay(2);
	}

	if (motor == 2)
	{
		if (direcao == 0) HAL_GPIO_WritePin(DIR2_GPIO_Port, DIR2_Pin, 0);
		if (direcao == 1) HAL_GPIO_WritePin(DIR2_GPIO_Port, DIR2_Pin, 1);

		HAL_GPIO_WritePin(STEP2_GPIO_Port, STEP2_Pin, 1);
		HAL_Delay(2);
		HAL_GPIO_WritePin(STEP2_GPIO_Port, STEP2_Pin, 0);
		HAL_Delay(2);
	}
}
