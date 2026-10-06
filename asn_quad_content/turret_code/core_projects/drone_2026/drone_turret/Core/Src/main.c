/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdlib.h"
#include "math.h"

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
CAN_HandleTypeDef hcan1;

I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim10;
TIM_HandleTypeDef htim13;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart6_rx;
DMA_HandleTypeDef hdma_usart6_tx;

/* Definitions for LibraryHandler */
osThreadId_t LibraryHandlerHandle;
const osThreadAttr_t LibraryHandler_attributes = {
  .name = "LibraryHandler",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for MainTask */
osThreadId_t MainTaskHandle;
const osThreadAttr_t MainTask_attributes = {
  .name = "MainTask",
  .stack_size = 2048 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_CAN1_Init(void);
static void MX_TIM13_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM10_Init(void);
static void MX_I2C2_Init(void);
void StartLibraryHandler(void *argument);
void StartMainTask(void *argument);

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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART6_UART_Init();
  MX_USART3_UART_Init();
  MX_CAN1_Init();
  MX_TIM13_Init();
  MX_SPI1_Init();
  MX_TIM10_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of LibraryHandler */
  LibraryHandlerHandle = osThreadNew(StartLibraryHandler, NULL, &LibraryHandler_attributes);

  /* creation of MainTask */
  MainTaskHandle = osThreadNew(StartMainTask, NULL, &MainTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 6;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 3;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_10TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_3TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM10 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 0;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 4999;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim10, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */
  HAL_TIM_MspPostInit(&htim10);

}

/**
  * @brief TIM13 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM13_Init(void)
{

  /* USER CODE BEGIN TIM13_Init 0 */

  /* USER CODE END TIM13_Init 0 */

  /* USER CODE BEGIN TIM13_Init 1 */

  /* USER CODE END TIM13_Init 1 */
  htim13.Instance = TIM13;
  htim13.Init.Prescaler = 14;
  htim13.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim13.Init.Period = 65535;
  htim13.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim13.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim13) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM13_Init 2 */

  /* USER CODE END TIM13_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 100000;
  huart3.Init.WordLength = UART_WORDLENGTH_9B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_EVEN;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
  /* DMA2_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
  /* DMA2_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
  /* DMA2_Stream7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

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
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : INT1_ACCEL_Pin_Pin INT1_GYRO_Pin_Pin */
  GPIO_InitStruct.Pin = INT1_ACCEL_Pin_Pin|INT1_GYRO_Pin_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
float yawGyros[3] = {0.0f};
float pitchGyros[5] = {0.0f};
float averageGyroYaw = 0.0f;
float averageGyroPitch = 0.0f;
const float GYRO_YAW_OFFSET = 0.524156f;
const float GYRO_PITCH_OFFSET = -0.001239f;

// inline function for clamping floats
static inline float clampFloat(float val, float minVal, float maxVal) {
    if (val < minVal) return minVal;
    if (val > maxVal) return maxVal;
    return val;
}

void Gyros_Update() {
    // continuously update most recent gyro values
    // BUGFIX: was sizeof(x[0]) which only shifts ONE element -> [2] stayed 0 forever,
    // so the 3-tap average was silently attenuating the gyro signal by 2/3.
    memmove(&yawGyros[1], &yawGyros[0], 2 * sizeof(yawGyros[0]));
    yawGyros[0] = MPU6050_get_gyro_z() + GYRO_YAW_OFFSET;
    memmove(&pitchGyros[1], &pitchGyros[0], 2 * sizeof(pitchGyros[0]));
    pitchGyros[0] = IMU_get_gyro(y) + GYRO_PITCH_OFFSET;

	// gyro LPF
	averageGyroYaw = (yawGyros[0] + yawGyros[1] + yawGyros[2]) / 3.0f;
	averageGyroPitch = (pitchGyros[0] + pitchGyros[1] + pitchGyros[2] + pitchGyros[3] + pitchGyros[4]) / 5.0f;
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartLibraryHandler */
/**
  * @brief  Function implementing the LibraryHandler thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartLibraryHandler */
void StartLibraryHandler(void *argument)
{
  /* USER CODE BEGIN 5 */
	UART_Init();
	UART_LinkLib(DBUS_DR16(), UART_IBUS());

	CAN_Init_Buses(BUS1);
	CAN_LinkLib(CAN_RMMotors());
	CAN_RM_Init_Buses(BUS1);

	BMI088_INIT();
	MPU6050_ACCEL_SENS(3);
	MPU6050_GYRO_SENS(3);
	MPU6050_INIT();

  /* Infinite loop */
  for(;;)
  {
    CAN_RM_Update();
    BMI088_Update(5);
    MPU6050_Update();
    Gyros_Update();
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartMainTask */
/**
* @brief Function implementing the MainTask thread.
* @param argument: Not used
* @retval None
*/


/* USER CODE END Header_StartMainTask */
void StartMainTask(void *argument)
{
  /* USER CODE BEGIN StartMainTask */
	// motor ids: left flywheel = 1; right flywheel = 2; pitch = 3; yaw = 4; indexer = 5
    const uint8_t FLYWHEEL_LEFT = 1;
    const uint8_t FLYWHEEL_RIGHT = 2;
    const uint8_t PITCH_MOTOR = 3;
    const uint8_t YAW_MOTOR = 4;
    const uint8_t INDEXER_MOTOR = 5;

    // flywheel vars
    const uint16_t FLYWHEEL_RPM = 6250;
    float flywheelPID[] = {20.0f, 0.0f, 0.0f};
    int16_t flywheelRamp = 0;

    // yaw & pitch vars
    const float PITCH_MAX = 2200.0f; // when turret is pointed lowest
    const float PITCH_MIN = 1600.0f; // when turret is pointed highest
    const float YAW_MAX = 4900.0f;
    const float YAW_MIN = 2250.0f;
    const float YAW_SENS = 0.15f;
    const float DT = 0.005;
    const float PITCH_GYRO_TO_COUNTS = -75.0f;
    const float INPUT_DEADBAND = 0.01f;
    const float OUTPUT_DEADBAND = 0.1f;
    const float PITCH_SENS = -0.15f;
    const float PITCH_KP = 0.75f;
    const float PITCH_KD = 15.0f;
    float pitchPID[] = {150.0f, 0.0f, 0.0f};
    float yawPID[] = {150.0f, 0.0f, 0.0f};
    float yawPD[] = {90.0f, 0.4f};
    float actualYawPos = 0.0f; // higher value when pitch is pointed low
    float actualPitchPos = 0.0f;
    float targetYawPos = 0.0f;
    float targetPitchPos = 0.0f;
    float targetYawRPM = 0.0f;
    float prevYawOutput = 0.0f;
    float pitchError = 0.0f;
    int8_t yawEndstopActive = 0;

    // indexer vars
    const int16_t INDEXER_RPM = 5000;
    const uint16_t INDEXER_SPINUP_TIME = 8;
    const uint16_t INDEXER_UNJAM_TIME = 30;
    const uint16_t INDEXER_JAM_TIME = 60;
    float indexerPID[] = {10.0f, 0.0, 0.0f};
    uint16_t unjamCounter = 0;
    uint16_t startupCounter = 0;
    uint16_t jamCounter = 0;

    // misc vars
    const uint16_t SWITCH_ON = 600;
    float zeroPID[] = {0.0f, 0.0f, 0.0f};

	/* Infinite loop */
	for (;;) {
	  actualPitchPos += averageGyroPitch * PITCH_GYRO_TO_COUNTS;
	  actualYawPos += averageGyroYaw * DT;
	  int16_t pitchEncoderPos = get6020Pos(BUS1, PITCH_MOTOR);

	  // killswitch
	  if (getIBUSCH5() >= SWITCH_ON) {
//______________________________________________________________________________________________________________________________________________________________________
		  // spin flywheels if switch is enabled
		  if (getIBUSCH6() > SWITCH_ON) {
		      if (flywheelRamp < FLYWHEEL_RPM) flywheelRamp += 200;
		      if (flywheelRamp > FLYWHEEL_RPM) flywheelRamp = FLYWHEEL_RPM;
		      setC620RPM(BUS1, FLYWHEEL_LEFT,  -flywheelRamp, flywheelPID);
		      setC620RPM(BUS1, FLYWHEEL_RIGHT,  flywheelRamp, flywheelPID);

		  } // stop flywheels otherwise
		  else {
		      if (flywheelRamp > 0) flywheelRamp -= 100;
		      if (flywheelRamp < 0) flywheelRamp = 0;
		      setC620RPM(BUS1, FLYWHEEL_LEFT,  -flywheelRamp, flywheelPID);
		      setC620RPM(BUS1, FLYWHEEL_RIGHT,  flywheelRamp, flywheelPID);
		  }
//______________________________________________________________________________________________________________________________________________________________________
		    // IMU stabilized pitch control
			targetPitchPos += PITCH_SENS * (float)getIBUSCH2();

			// soft endstops
			if (pitchEncoderPos != 0) { // guard: encoder reads 0 before the first CAN frame
			   targetPitchPos = clampFloat(targetPitchPos, (actualPitchPos + (PITCH_MIN - (float)pitchEncoderPos)), (actualPitchPos + (PITCH_MAX - (float)pitchEncoderPos)));
			}

			// get current error (counts)
			pitchError = targetPitchPos - actualPitchPos;
			// PD output
			float pitchOutput = (PITCH_KP * pitchError) + (PITCH_KD * averageGyroPitch);

			// output
			set6020RPM(BUS1, PITCH_MOTOR, (int16_t)pitchOutput, pitchPID);
//______________________________________________________________________________________________________________________________________________________________________
			// IMU stabilized yaw control
			// get controller input, scale by loop freq
			targetYawRPM = ((float)getIBUSCH1()) * DT;
			// input deadband
			if ((targetYawRPM >= -INPUT_DEADBAND) && (targetYawRPM <= INPUT_DEADBAND)) {
			targetYawRPM = 0;
			}

			// multiply by sensitivity to get target angle
			targetYawPos += YAW_SENS * targetYawRPM;

			// get current error
			float yawError = targetYawPos - actualYawPos;
			// compute output command using PD control
			float yawOutput = (yawPD[0] * yawError) - (yawPD[1] * averageGyroYaw);

			int16_t yawEncoderPos = (get6020Pos(BUS1, YAW_MOTOR));
			// end limit logic
			// release when commanded away from the wall
			yawEndstopActive = ((yawEndstopActive ==  1) && (targetYawRPM < 0)) ? 0 : yawEndstopActive;
			yawEndstopActive = ((yawEndstopActive == -1) && (targetYawRPM > 0)) ? 0 : yawEndstopActive;

			// or when the turret is back inside the range and isn't still commanding into that wall
			yawEndstopActive = ((yawEndstopActive ==  1) && (targetYawRPM <= 0) && (yawEncoderPos < YAW_MAX)) ? 0 : yawEndstopActive;
			yawEndstopActive = ((yawEndstopActive == -1) && (targetYawRPM >= 0) && (yawEncoderPos > YAW_MIN)) ? 0 : yawEndstopActive;

			// if at end limit: setpoint follows the turret
			if (yawEndstopActive != 0) {
			targetYawPos = actualYawPos;
			yawOutput = 0;
			}
			// clamp max move speed
			// output deadband
			if ((fabsf(yawOutput - prevYawOutput) <= OUTPUT_DEADBAND)) {
			yawOutput = prevYawOutput;
			}
			// output
			set6020RPM(BUS1, YAW_MOTOR, (int16_t)yawOutput, yawPID);
			prevYawOutput = yawOutput;
			if (yawEncoderPos >= YAW_MAX) yawEndstopActive = 1;
			else if (yawEncoderPos <= YAW_MIN) yawEndstopActive = -1;
//______________________________________________________________________________________________________________________________________________________________________
		  // testing code
//		  if (getIBUSCH7() > SWITCH_ON) {
//			  setC610RPM(BUS1, INDEXER_MOTOR, 23*(getIBUSCH8()+660), indexerPID);
//		  } else if (getIBUSCH7() > -600 && getIBUSCH7() < 600) {
//			  setC610RPM(BUS1, INDEXER_MOTOR, -23*(getIBUSCH8()+660), indexerPID);
//		  } else {
//			  setC610RPM(BUS1, INDEXER_MOTOR, 0, zeroPID);
//		  }
		  //   indexer code
		  if (getIBUSCH7() > SWITCH_ON) {
		      if (unjamCounter > 0) {
		          // jammed: reverse until timer runs out
		          setC610RPM(BUS1, INDEXER_MOTOR, -INDEXER_RPM, indexerPID);
		          unjamCounter--;
		          // when reverse ends, re-arm the spinup grace so the motor has
		          // time to come back up to speed before jam detection resumes
		          if (unjamCounter == 0) {
		              startupCounter = 0;
		              jamCounter = 0;
		          }
		      } else if (getIBUSCH8() > SWITCH_ON) {
		    	  // override: reverse indexer
		    	  setC610RPM(BUS1, INDEXER_MOTOR, -INDEXER_RPM, indexerPID);
		      }
		      else {
		          // normal run
		          setC610RPM(BUS1, INDEXER_MOTOR, INDEXER_RPM, indexerPID);
		          // only check for jams once spun up
		          if (startupCounter >= INDEXER_SPINUP_TIME) {
		              if (getC610RPM(BUS1, INDEXER_MOTOR) <= 1000) {
		                  // stalled this loop; only commit to a reverse once the
		                  // stall has persisted continuously for INDEXER_JAM_TIME loops
		                  jamCounter++;
		                  if (jamCounter >= INDEXER_JAM_TIME) {
		                      unjamCounter = INDEXER_UNJAM_TIME;
		                      jamCounter = 0;
		                  }
		              } else {
		                  // stall cleared before hitting the threshold
		                  jamCounter = 0;
		              }
		          } else {
		              startupCounter++;
		          }
		      }
		  } else {
		      // disarmed, stop and clear
		      setC610RPM(BUS1, INDEXER_MOTOR, 0, zeroPID);
		      unjamCounter = 0;
		      startupCounter = 0;
		      jamCounter = 0;
		  }
	  }
//______________________________________________________________________________________________________________________________________________________________________
	  // disable all motor currents if killswitch is flipped
	  else {
		setC620RPM(BUS1, FLYWHEEL_LEFT, 0, zeroPID);
		setC620RPM(BUS1, FLYWHEEL_RIGHT, 0, zeroPID);
		set6020Pos(BUS1, PITCH_MOTOR, 0, zeroPID);
		set6020Pos(BUS1, YAW_MOTOR, 0, zeroPID);
		setC610RPM(BUS1, INDEXER_MOTOR, 0, zeroPID);
		// keep targets pinned to reality while disarmed so re-arming doesn't jerk it
		targetPitchPos = actualPitchPos;
		targetYawPos = actualYawPos;
	  }
//		UART_Printf(UART1,"pitchActual=%f, pitchTarget=%f, yawActual=%f, yawTarget=%f\r\n", actualPitchPos, targetPitchPos, actualYawPos, targetYawPos);
//		UART_Printf(UART1, "pitchGyro=%f, pitchTarget=%f, pitchActual=%f, pitchEncoder=%d\r\n", averageGyroPitch, targetPitchPos, actualPitchPos, pitchEncoderPos);
//		UART_Printf(UART1, "%d %d\r\n", get6020Pos(BUS1, YAW_MOTOR), get6020Pos(BUS1, PITCH_MOTOR));
//		UART_Printf(UART1, "yaw=%f, pitch=%f\r\n", yawGyros[0], pitchGyros[0]);
		UART_Printf(UART1, "indexerTarget=%d, indexerRPM=%d\r\n", 23*(getIBUSCH8()+660), getC610RPM(BUS1, INDEXER_MOTOR));
//		UART_Printf(UART1, "flywheelRamp=%d, flywheel1RPM=%d, flywheel2RPM=%d\r\n", flywheelRamp, getC620RPM(BUS1, FLYWHEEL_LEFT), getC620RPM(BUS1, FLYWHEEL_RIGHT));
	  osDelay(5);
	}
  /* USER CODE END StartMainTask */
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
