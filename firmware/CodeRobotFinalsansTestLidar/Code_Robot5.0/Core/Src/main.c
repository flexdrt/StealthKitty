/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 * Author: StealthKitty
 * Copyright (c) 2024 STMicroelectronics.
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
#include "cmsis_os.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "vl53l1_platform.h"
#include "VL53L1X_api.h"
#include "string.h"
#include "moteur.h"
#include "ADXL343_SPI.h"
//#include "drv_LIDAR.h"

/**
 * @brief Redirects printf to UART2 for debugging purposes.
 * @param file: Unused parameter for file descriptor.
 * @param ptr: Pointer to the buffer containing the data to send.
 * @param len: Length of the data to send.
 * @return The number of characters sent.
 */
int _write(int file, char *ptr, int len) {
	HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len, HAL_MAX_DELAY);
	return len;
}

/**
 * Global variables
 */
extern SPI_HandleTypeDef hspi1;
ADXL343 accelerometer;





/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* Configuration FreeRTOS */
#define STACK_SIZE 256
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

TaskHandle_t h_task_bord = NULL; /**< Task handle for managing a specific task. */


int frame_start = 0; /**< Start index of the LIDAR frame. */
int frame_end = 0; /**< End index of the LIDAR frame. */

int dist_min = 0; /**< Minimum detected distance. */
int idx_min = 0; /**< Index of the closest detected object. */

SemaphoreHandle_t SemHalfCallBack; /**< Semaphore for half DMA completion. */
SemaphoreHandle_t SemClpCallBack; /**< Semaphore for complete DMA transfer. */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/**
 * @brief Configures the system clock.
 */
void SystemClock_Config(void);
/**
 * @brief Initializes FreeRTOS objects.
 */
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/**
 * @brief Transmits data using UART synchronously.
 * @param p_data: Pointer to the data buffer.
 * @param size: Size of the data to transmit.
 * @return 0 on success.
 */
int uart_transmit(uint8_t *p_data, uint16_t size){
	HAL_UART_Transmit(&huart4, p_data, size, HAL_MAX_DELAY);
	return 0;
}

/**
 * @brief Transmits data using UART with interrupt mode.
 * @param p_data: Pointer to the data buffer.
 * @param size: Size of the data to transmit.
 * @return 0 on success.
 */
int uart_it_transmit(uint8_t *p_data, uint16_t size){
	HAL_UART_Transmit_IT(&huart4, p_data, size);
	return 0;
}

/**
 * @brief Transmits data using UART with DMA.
 * @param p_data: Pointer to the data buffer.
 * @param size: Size of the data to transmit.
 * @return 0 on success.
 */
int uart_dma_transmit(uint8_t *p_data, uint16_t size){
	HAL_UART_Transmit_DMA(&huart4, p_data, size);
	return 0;
}

/**
 * @brief Receives data using UART in polling mode.
 * @param p_data: Pointer to the buffer to store received data.
 * @param size: Size of the data to receive.
 * @return 0 on success.
 */
int uart_poll_receive(uint8_t *p_data, uint16_t size){
	HAL_UART_Receive(&huart4, p_data, size, HAL_MAX_DELAY);
	return 0;
}

/**
 * @brief Receives data using UART with interrupt mode.
 * @param p_data: Pointer to the buffer to store received data.
 * @param size: Size of the data to receive.
 * @return 0 on success.
 */
int uart_it_receive(uint8_t *p_data, uint16_t size){
	HAL_UART_Receive_IT(&huart4, p_data, size);
	return 0;
}

/**
 * @brief Receives data using UART with DMA.
 * @param p_data: Pointer to the buffer to store received data.
 * @param size: Size of the data to receive.
 * @return 0 on success.
 */
int uart_dma_receive(uint8_t *p_data, uint16_t size){
	HAL_UART_Receive_DMA(&huart4, p_data, size);
	return 0;
}

//void Task_MotorControl(void *argument);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*-----------------------------------------Test---------------------------------------*/
/*void task_Bord(void * unused)
{
	printf("task_Bord démarrée.\n");
	for(;;) {
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		VL53L1X_ClearInterrupt(TOF_ADDR);
		printf(" Attention, seuil franchi !\n");
		VL53L1X_ClearInterrupt(TOF_ADDR);
	}
}*/

/*
void task_SensorMeasurement(void *unused) {
	printf("task_SensorMeasurement démarrée.\n");
	uint16_t distance ;
	uint8_t rangeStatus = 0;

	while (1) {
		VL53L1X_CheckForDataReady(TOF_ADDR, &rangeStatus);
		if (rangeStatus == 0) {
			VL53L1X_GetDistance(TOF_ADDR, &distance);
			printf("Distance mesurée : %d mm\n", distance);
		}
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}
 */

/*void Task_MotorControl(void *argument) {

	printf("Tâche MotorControl démarrée.\n");
	// Choisissez un alpha adapté à la PWM
	uint16_t alpha = 1200;  // 30% de 8500
	uint16_t distance = 0;  // Distance mesurée par le capteur
	const uint32_t delta_time_ms = 50;  // Intervalle de temps pour le calcul
	const uint16_t encoder_resolution = 1024;  // Nombre d'impulsions par tour

	for (;;) {
		// Lire la distance avec VL53L1X
		if (VL53L1X_GetDistance(TOF_ADDR, &distance) == 0) {
			printf("Distance mesurée : %d mm\n", distance);

			// Vérifier si la distance dépasse le seuil critique
			if (distance > 250) {
				// Arrêter les moteurs
				stop_r();
				stop_l();
				printf("Danger détecté : moteur arrêté. Pivot arrière en cours...\n");

				// Effectuer un pivot en arrière
				reverse_r(alpha + 3050);       // Moteur droit recule
				reverse_l(alpha );   // Moteur gauche recule plus lentement pour pivoter
				vTaskDelay(pdMS_TO_TICKS(600)); // Pivot pendant 1,5 seconde

				// Arrêter les moteurs après le pivot
				stop_r();
				stop_l();
				vTaskDelay(pdMS_TO_TICKS(200));
				forward_r(alpha+ 2450);
				vTaskDelay(pdMS_TO_TICKS(320));
				printf("Pivot terminé, moteurs arrêtés.\n");
			} else {
				forward_r(alpha + 1650);
				forward_l(alpha+ 150);

				// Calcul et affichage des vitesses
				float speed_left = calculate_motor_speed(MOTOR_LEFT, delta_time_ms, encoder_resolution);
				float speed_right = calculate_motor_speed(MOTOR_RIGHT, delta_time_ms, encoder_resolution);

				printf("Vitesse moteur gauche : %.2f tours/s\n", speed_left);
				printf("Vitesse moteur droit : %.2f tours/s\n", speed_right);
			}
		} else {
			printf("Erreur : lecture de la distance impossible.\n");
		}
		printf("Position moteur gauche : %d\n", __HAL_TIM_GET_COUNTER(&htim3));
		printf("Position moteur droit : %d\n", __HAL_TIM_GET_COUNTER(&htim4));


		vTaskDelay(pdMS_TO_TICKS(delta_time_ms)); // Vérification toutes les 100 ms
	}
}*/
/*----------------------------------------Fin-------------------------------------------*/


/**
 * @brief Task to manage the ADXL343 accelerometer.
 * @param argument: Pointer to the task arguments (if any).
 */
void vTaskADX(void *argument)
{
	// Initialisation de l'accéléromètre
	ADXL343_initialization(&hspi1, &accelerometer);
	ADXL343_EnableTapInterrupts(&accelerometer);
	ADXL343_set_tap_parameters(&accelerometer, 20, 1, 80, 200);
	ADXL343_SetDataFormat(&hspi1, 0x03); // Configurer en ±16g

	// Variables pour suivre l'état des LEDs et le mode moteur
	uint8_t led_state = 0;    // 0 = rouge, 1 = bleu
	uint8_t motor_mode = 0;  // 0 = souris, 1 = prédateur

	printf("Tâche ADXL343 démarrée. Attente des tapotements...\n");

	while (1)
	{
		// Attente de détection de tap
		vTaskDelay(pdMS_TO_TICKS(500));

		// Si un tap est détecté
		        if (ADXL343_check_tap(&accelerometer)) {
		            // Alterner les LEDs
		            if (led_state == 0) {
		                HAL_GPIO_WritePin(GPIOB, Status_Red_Pin, GPIO_PIN_SET);    // LED rouge ON
		                HAL_GPIO_WritePin(GPIOB, Status_Blue_Pin, GPIO_PIN_RESET); // LED bleue OFF
		                led_state = 1;
		            } else {
		                HAL_GPIO_WritePin(GPIOB, Status_Red_Pin, GPIO_PIN_RESET); // LED rouge OFF
		                HAL_GPIO_WritePin(GPIOB, Status_Blue_Pin, GPIO_PIN_SET);  // LED bleue ON
		                led_state = 0;
		            }

			// Alterner le mode moteur
			if (motor_mode == 0)
			{
				vTaskDelay(pdMS_TO_TICKS(1000));
				printf("Mode Prédateur activé.\n");
				motorcontrol_predateur(); // Activer le mode prédateur
				motor_mode = 1;
			}
			else
			{
				vTaskDelay(pdMS_TO_TICKS(1000));
				printf("Mode Souris activé.\n");
				motorcontrol_souris(); // Activer le mode souris
				motor_mode = 0;
			}
		}
	}
}

/**
 * @brief Task for the VL53L1X ToF sensor.
 * @param argument: Pointer to the task arguments (if any).
 */

void vTaskToF(void *argument) {
	uint16_t distance;
	uint8_t range_status;

	while (1) {
		// Vérifier la disponibilité des données
		VL53L1X_CheckForDataReady(TOF_ADDR, &range_status);
		if (range_status == 0) {
			VL53L1X_GetDistance(TOF_ADDR, &distance);
			printf("Distance mesurée : %d mm\n", distance);

			if (distance > 250) {
				pivot(); // Effectuer un pivot
			}
		}
		vTaskDelay(pdMS_TO_TICKS(500)); // Vérification rapide
	}
}






	/* USER CODE END 0 */

	/**
	 * @brief  The application entry point.
	 * @retval int
	 */
	int main(void)
	{

		/* USER CODE BEGIN 1 */
		//BaseType_t ret;
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
		MX_DMA_Init();
		MX_I2C1_Init();
		MX_SPI1_Init();
		MX_USART2_UART_Init();
		MX_TIM1_Init();
		MX_TIM3_Init();
		MX_TIM4_Init();
		MX_UART4_Init();

		/* USER CODE BEGIN 2 */

		init_motors();

		//I2C_Scan();

		VL53L1__Init();
		printf("Initialisation du système STM32 terminée.\n");

		VL53L1X_StartRanging(TOF_ADDR); // Démarrer le capteur
		VL53L1X_SetDistanceThreshold(TOF_ADDR, 10, 250, 1, 0); // Définir le seuil de distance
		VL53L1X_SetInterruptPolarity(TOF_ADDR, 1); // Configurer la polarité de l'interruption
		VL53L1X_ClearInterrupt(TOF_ADDR); // Effacer les interruptions en attente


		// Créer les tâches FreeRTOS
		xTaskCreate(vTaskADX, "TaskADX", 256, NULL, 2, NULL);
		xTaskCreate(vTaskToF, "TaskToF", 256, NULL, 5, NULL);
		//xTaskCreate(TaskLIDAR, "TaskLIDAR", 256, NULL, 5, NULL);


		//xTaskCreate(task_Bord, "Bord", 128, NULL, 8, &h_task_bord);
		//xTaskCreate(task_SensorMeasurement, "SensorMeasurement", 256, NULL, 4, NULL);








		/* USER CODE END 2 */

		/* Call init function for freertos objects (in cmsis_os2.c) */
		MX_FREERTOS_Init();

		/* Start scheduler */
		osKernelStart();

		/* We should never get here as control is now taken by the scheduler */

		/* Infinite loop */
		/* USER CODE BEGIN WHILE */





		while (1)
		{

			//uint16_t distance;
			// Lecture de la distance avec vérification d'erreur
			/*if (VL53L1X_GetDistance(TOF_ADDR, &distance) == 0) {
		   printf("Distance mesurée : %d mm\n", distance);
		   } else {
		   printf("Erreur lors de la mesure.\n");
		   }*/

			//VL53L1__test();

			HAL_Delay(500);



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

		/** Configure the main internal regulator output voltage
		 */
		HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

		/** Initializes the RCC Oscillators according to the specified parameters
		 * in the RCC_OscInitTypeDef structure.
		 */
		RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
		RCC_OscInitStruct.HSEState = RCC_HSE_ON;
		RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
		RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
		RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
		RCC_OscInitStruct.PLL.PLLN = 85;
		RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
		RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
		RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
		if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
		{
			Error_Handler();
		}

		/** Initializes the CPU, AHB and APB buses clocks
		 */
		RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
				|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
		RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
		RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

		if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
		{
			Error_Handler();
		}
	}

	/* USER CODE BEGIN 4 */
	void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
	{
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		xSemaphoreGiveFromISR(SemHalfCallBack, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}

	void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	{
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		xSemaphoreGiveFromISR(SemClpCallBack, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}

	void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
	{
		if (GPIO_Pin == INT_TOF2_Pin) {
			printf("Interruption détectée sur le capteur TOF.\n");

			// Effacer l'interruption immédiatement
			VL53L1X_ClearInterrupt(TOF_ADDR);
		}
	}

	/*void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	VL53L1X_ClearInterrupt(TOF_ADDR);
	printf("Tu tombes \n") ;
	VL53L1X_ClearInterrupt(TOF_ADDR);

}*/
	/* USER CODE END 4 */

	/**
	 * @brief  Period elapsed callback in non blocking mode
	 * @note   This function is called  when TIM6 interrupt took place, inside
	 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
	 * a global variable "uwTick" used as application time base.
	 * @param  htim : TIM handle
	 * @retval None
	 */
	void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	{
		/* USER CODE BEGIN Callback 0 */

		/* USER CODE END Callback 0 */
		if (htim->Instance == TIM6) {
			HAL_IncTick();
		}
		/* USER CODE BEGIN Callback 1 */

		/* USER CODE END Callback 1 */
	}

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
