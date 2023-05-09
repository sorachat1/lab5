/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#include "stdio.h"
#include "string.h"
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
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t RxBuffer[20];
uint8_t TxBuffer[400];
uint8_t text[80];
uint8_t test = 0;
uint8_t state = 0;
uint8_t meau[20];
uint16_t a;
uint8_t step;
uint8_t check;
uint8_t t;
uint8_t Buttontick;
uint8_t p;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void UARTPollingMethod();
void Dummytask();
void UARTInterruptConfig();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
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
	MX_USART2_UART_Init();
	/* USER CODE BEGIN 2 */
	//  uint8_t text[] = "HELLO FIBO";
	//  HAL_UART_Transmit(&huart2, text, 11, 10);
	UARTInterruptConfig();
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		//Dummytask();
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		switch (state) {
		case 1: {
			if (step == 1) {
				Dummytask();
			}
			if (RxBuffer[0] == 97) // a speed up 1 Hz
					{
				a = a + 500;
				sprintf((char*) TxBuffer, "speed : %d\r\n", a / 500);
				HAL_UART_Transmit_IT(&huart2, TxBuffer,
						strlen((char*) TxBuffer));
				RxBuffer[0] = 0;
			} else if (RxBuffer[0] == 115) // s speed down 1 Hz
					{
				a = a - 500;
				sprintf((char*) TxBuffer, "speed : %d\r\n", a / 500);
				HAL_UART_Transmit_IT(&huart2, TxBuffer,
						strlen((char*) TxBuffer));
				RxBuffer[0] = 0;
			} else if (RxBuffer[0] == 100) // d on off
					{
				if (step == 0) {
					step = 1;
				} else if (step == 1) {
					step = 0;
				}

				HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
				//HAL_GPIO_WritePin(LED_D2_GPIO_Port, LED_D2_Pin, GPIO_PIN_SET);
				//sprintf((char*) TxBuffer, "speed : %d\r\n", test);
				//HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*) TxBuffer));
				RxBuffer[0] = 0;
			} else if (RxBuffer[0] == 120) {
				state = 0;
				sprintf((char*)TxBuffer, "LED Control\r\na: Speed Up+1Hz\r\ns: Speed Down -1Hz\r\nd: On/Off\r\nx: Back\r\n\0");
							HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*) TxBuffer));
				RxBuffer[0] = 0;
			}
			break;

		}
		case 2: {
			//readbutton();
			Buttontick = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
			if (check == 0 & Buttontick > 0) {
						t = 1;
					} else {

						t = 0;
					}
					check = Buttontick;
					if(Buttontick == 1 && t == 1)
					{
						sprintf((char*) TxBuffer, "Button : Press \r\n\0 ");
						HAL_UART_Transmit_IT(&huart2, TxBuffer,	strlen((char*) TxBuffer));
					}
					else if(Buttontick == 0 && t == 1)
					{

				        sprintf((char*) TxBuffer, "Button : UnPress \r\n\0 ");
				      HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*) TxBuffer));
					}


			}
		if(RxBuffer[0] == 120){
			state = 0 ;
			sprintf((char*)TxBuffer, "LED Control\r\na: Speed Up +1Hz\r\n   s: Speed Down -1Hz\r\n   d: On/Off\r\n   x: Back\r\n----------------------------------\r\n\0");
						HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*) TxBuffer));
			RxBuffer[0] = 0;
		}
		break;
		}



	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 16;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 9600;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : B1_Pin */
	GPIO_InitStruct.Pin = B1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : LD2_Pin */
	GPIO_InitStruct.Pin = LD2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void Dummytask() {
	static uint32_t timestamp = 0;
	if (HAL_GetTick() >= timestamp) {
		timestamp = HAL_GetTick() + a;
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	}

}

void UARTInterruptConfig() {
	HAL_UART_Receive_IT(&huart2, RxBuffer, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart == &huart2) {
		RxBuffer[1] = '\0';
		sprintf((char*) TxBuffer, "status : %s\r\n", RxBuffer);
		HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*) TxBuffer));
		HAL_UART_Receive_IT(&huart2, RxBuffer, 1);
		if (RxBuffer[0] == 48) // 0 LED CONtrol
				{
			sprintf((char*)TxBuffer, "LED Control\r\n a : speed+1Hz\r\n s: speed-1Hz\r\n");
			HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*) TxBuffer));
			state = 1;
		} else if (RxBuffer[0] == 49) // 1 button status
				{
			state = 2;
		}

	}
}


/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
