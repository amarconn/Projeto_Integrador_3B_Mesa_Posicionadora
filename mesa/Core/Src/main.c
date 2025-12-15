/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "liquidcrystal_i2c.h"
#include "mesa_funcoes.h"
#include "stdlib.h"
#include "stdint.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  HD44780_Init(2);

  uint8_t degree_char[8] = {
		  0b00110,
		  0b01001,
		  0b01001,
		  0b00110,
		  0b00000,
		  0b00000,
		  0b00000,
		  0b00000
  };
  uint8_t degree = 200;
  HD44780_CreateSpecialChar(degree, degree_char);

  int LeftButton, RightButton, OKButton;
  int checkTelaRun, checkAngulo1, checkAngulo2, checkTelaInicial = 0, checkTelaCalib, checkCalibMotor1, checkCalibMotor2;
  int angulo1 = 0, angulo2 = 0, eixos, x = 0, angulo1real = 0, angulo2real = 0,passos1real = 0;

  char snum1[4], snum2[4];
  itoa(0, snum1, 10);
  itoa(0, snum2, 10);

  TelaInicial();

  HAL_GPIO_WritePin(Internal_LED_GPIO_Port, Internal_LED_Pin, 0);

  while (checkTelaInicial == 0)
  {
	  LeftButton = HAL_GPIO_ReadPin(LeftButton_GPIO_Port, LeftButton_Pin);
	  RightButton = HAL_GPIO_ReadPin(RightButton_GPIO_Port, RightButton_Pin);
	  OKButton = HAL_GPIO_ReadPin(OKButton_GPIO_Port, OKButton_Pin);

	  if ((RightButton == 0) | (LeftButton == 0) | (OKButton == 0))
	  {
		  checkTelaInicial = 1;
	  }
  }

  TelaModo();
  HAL_Delay(1000);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		TelaModo();

		LeftButton = HAL_GPIO_ReadPin(LeftButton_GPIO_Port, LeftButton_Pin);
		RightButton = HAL_GPIO_ReadPin(RightButton_GPIO_Port, RightButton_Pin);
		OKButton = HAL_GPIO_ReadPin(OKButton_GPIO_Port, OKButton_Pin);

		if (LeftButton == 0)
		{
			checkTelaCalib = 0;
			TelaCalib1();

			while (checkTelaCalib == 0)
			{
				LeftButton = HAL_GPIO_ReadPin(LeftButton_GPIO_Port, LeftButton_Pin);
				RightButton = HAL_GPIO_ReadPin(RightButton_GPIO_Port, RightButton_Pin);
				OKButton = HAL_GPIO_ReadPin(OKButton_GPIO_Port, OKButton_Pin);

				if (LeftButton == 0) // motor 1
				{
					TelaCalib2();
					checkCalibMotor1 = 0;

					while(checkCalibMotor1 == 0)
					{
						LeftButton = HAL_GPIO_ReadPin(LeftButton_GPIO_Port, LeftButton_Pin);
						RightButton = HAL_GPIO_ReadPin(RightButton_GPIO_Port, RightButton_Pin);
						OKButton = HAL_GPIO_ReadPin(OKButton_GPIO_Port, OKButton_Pin);

						if (LeftButton == 0)
						{
							HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, 0);

							HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, 1);
							HAL_Delay(2);
							HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, 0);
							HAL_Delay(2);
						}
						if (RightButton == 0)
						{
							HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, 1);

							HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, 1);
							HAL_Delay(2);
							HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, 0);
							HAL_Delay(2);
						}
						if (OKButton == 0)
						{
							angulo1real = 0;
							checkCalibMotor1 = 1;
							checkTelaCalib = 1;
							HD44780_Clear();
						}
					}
				}

				if (RightButton == 0) // motor 2
				{
					TelaCalib2();
					checkCalibMotor2 = 0;

					while(checkCalibMotor2 == 0)
					{
						LeftButton = HAL_GPIO_ReadPin(LeftButton_GPIO_Port, LeftButton_Pin);
						RightButton = HAL_GPIO_ReadPin(RightButton_GPIO_Port, RightButton_Pin);
						OKButton = HAL_GPIO_ReadPin(OKButton_GPIO_Port, OKButton_Pin);

						if (LeftButton == 0)
						{
							HAL_GPIO_WritePin(DIR2_GPIO_Port, DIR2_Pin, 0);

							HAL_GPIO_WritePin(STEP2_GPIO_Port, STEP2_Pin, 1);
							HAL_Delay(2);
							HAL_GPIO_WritePin(STEP2_GPIO_Port, STEP2_Pin, 0);
							HAL_Delay(2);
						}
						if (RightButton == 0)
						{
							HAL_GPIO_WritePin(DIR2_GPIO_Port, DIR2_Pin, 1);
							HAL_GPIO_WritePin(STEP2_GPIO_Port, STEP2_Pin, 1);
							HAL_Delay(2);
							HAL_GPIO_WritePin(STEP2_GPIO_Port, STEP2_Pin, 0);
							HAL_Delay(2);
						}
						if (OKButton == 0)
						{
							angulo2real = 0;
							checkCalibMotor2 = 1;
							checkTelaCalib = 1;
							HD44780_Clear();
						}
					}
				}
			}
		}

		if (RightButton == 0)
		{
			checkTelaRun = 0;
			eixos = 0;
			TelaRun();

			while (checkTelaRun == 0)
			{
				checkAngulo1 = 0;
				checkAngulo2 = 0;

				LeftButton = HAL_GPIO_ReadPin(LeftButton_GPIO_Port, LeftButton_Pin);
				RightButton = HAL_GPIO_ReadPin(RightButton_GPIO_Port, RightButton_Pin);
				OKButton = HAL_GPIO_ReadPin(OKButton_GPIO_Port, OKButton_Pin);

				if (RightButton == 0) eixos = 2;
				if (LeftButton == 0) eixos = 1;

				if (eixos != 0)
				{
					HD44780_Clear();
					HAL_Delay(300);

					x = 0;
					while (checkAngulo1 == 0)
					{
						HD44780_SetCursor(1, 0);
						HD44780_PrintStr("Angulo motor 1");

						HD44780_SetCursor(0, 1);
						HD44780_PrintStr(" <-");
						HD44780_SetCursor(9-x, 1);
						HD44780_PrintStr(snum1);
						HD44780_PrintSpecialChar(0xDF);
						HD44780_PrintStr("  -> ");

						LeftButton = HAL_GPIO_ReadPin(LeftButton_GPIO_Port, LeftButton_Pin);
						RightButton = HAL_GPIO_ReadPin(RightButton_GPIO_Port, RightButton_Pin);
						OKButton = HAL_GPIO_ReadPin(OKButton_GPIO_Port, OKButton_Pin);

						if ((LeftButton == 0) && (angulo1 > -180))
							{
								HAL_Delay(150);
								angulo1--;
							}
						if ((RightButton == 0) && (angulo1 < 180))
							{
								HAL_Delay(150);
								angulo1++;
							}
						if(OKButton == 0)
							{
								HAL_Delay(150);
								HD44780_Clear();
								checkAngulo1 = 1;
							}

						x = 0;
						if ((angulo1>9)|(angulo1<0)) x = 1;
						if ((angulo1<-9)|(angulo1>99)) x = 2;
						if (angulo1<-99) x = 3;

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
						itoa(angulo1, snum1, 10);
					}

					HAL_Delay(300);

					if (eixos == 2)
					{
						x = 0;
						while (checkAngulo2 == 0)
						{
							HD44780_SetCursor(1, 0);
							HD44780_PrintStr("Angulo motor 2");

							HD44780_SetCursor(0, 1);
							HD44780_PrintStr(" <-");
							HD44780_SetCursor(9-x, 1);
							HD44780_PrintStr(snum2);
							HD44780_PrintSpecialChar(0xDF);
							HD44780_PrintStr("  -> ");

							LeftButton = HAL_GPIO_ReadPin(LeftButton_GPIO_Port, LeftButton_Pin);
							RightButton = HAL_GPIO_ReadPin(RightButton_GPIO_Port, RightButton_Pin);
							OKButton = HAL_GPIO_ReadPin(OKButton_GPIO_Port, OKButton_Pin);

							if ((LeftButton == 0) && (angulo2 > -180))
								{
									HAL_Delay(150);
									angulo2--;
								}
							if ((RightButton == 0) && (angulo2 < 180))
								{
									HAL_Delay(150);
									angulo2++;
								}
							if(OKButton == 0)
								{
									HAL_Delay(150);
									HD44780_Clear();
									checkAngulo2 = 1;
								}

							x = 0;
							if ((angulo2>9)|(angulo2<0)) x = 1;
							if ((angulo2<-9)|(angulo2>99)) x = 2;
							if (angulo2<-99) x = 3;

							if ((angulo2>=-99) && (angulo2<-9))
							{
								HD44780_SetCursor(6, 1);
								HD44780_PrintStr(" ");
							}
							if ((angulo2>=-9) && (angulo2<0))
							{
								HD44780_SetCursor(7, 1);
								HD44780_PrintStr(" ");
							}
							if (angulo2==0)
							{
								HD44780_SetCursor(8, 1);
								HD44780_PrintStr(" ");
							}

							if ((angulo2<100) && (angulo2>=10))
							{
								HD44780_SetCursor(7, 1);
								HD44780_PrintStr(" ");
							}
							if ((angulo2<10) && (angulo2>0))
							{
								HD44780_SetCursor(8, 1);
								HD44780_PrintStr(" ");
							}
							itoa(angulo2, snum2, 10);
						}
					}

					//passos1 = abs(3200 * angulo1 / 360);
					passos1real = abs(3200 * (angulo1real - angulo1)/ 360);
					angulo1real = angulo1 - angulo1real;

					if (angulo1<angulo1real) HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, 1);
					if (angulo1>angulo1real) HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, 0);

					HD44780_Clear();
					HD44780_SetCursor(0, 0);
					HD44780_PrintStr("   Executando   ");

					for (int i = 0; i <= passos1real; i++)
					{
						HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, 1);
						HAL_Delay(2);
						HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, 0);
						HAL_Delay(2);
					}

					/*if (eixos == 2)
					{
						passos2 = 3200 * angulo2 / 360;

						if (angulo2>0) HAL_GPIO_WritePin(DIR2_GPIO_Port, DIR2_Pin, 1);
						if (angulo2<0) HAL_GPIO_WritePin(DIR2_GPIO_Port, DIR2_Pin, 0);

						HD44780_Clear();
						HD44780_SetCursor(0, 0);
						HD44780_PrintStr("   Executando   ");

						for (int i = 0; i <= passos2; i++)
						{
							HAL_GPIO_WritePin(STEP2_GPIO_Port, STEP2_Pin, 1);
							HAL_Delay(2);
							HAL_GPIO_WritePin(STEP2_GPIO_Port, STEP2_Pin, 0);
							HAL_Delay(2);

						}
					}*/

					checkTelaRun = 1;

				}
			}
		}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Internal_LED_GPIO_Port, Internal_LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, STEP_Pin|DIR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, STEP2_Pin|DIR2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Internal_LED_Pin */
  GPIO_InitStruct.Pin = Internal_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Internal_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : STEP_Pin DIR_Pin */
  GPIO_InitStruct.Pin = STEP_Pin|DIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : STEP2_Pin DIR2_Pin */
  GPIO_InitStruct.Pin = STEP2_Pin|DIR2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LeftButton_Pin OKButton_Pin RightButton_Pin */
  GPIO_InitStruct.Pin = LeftButton_Pin|OKButton_Pin|RightButton_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
