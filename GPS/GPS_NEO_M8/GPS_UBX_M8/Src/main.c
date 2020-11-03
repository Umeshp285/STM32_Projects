/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include"string.h"
#include"stdio.h"
#include"stdlib.h"
#include"stdarg.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */


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
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t cold_start[] = {0xB5, 0x62, 0x06, 0x04 , 0x04, 0x00,  0xFF, 0xB9, 0x02, 0x00, 0xC8, 0x8F};
uint8_t warm_start[] = {0xB5, 0x62, 0x06, 0x04, 0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x11, 0x6C};
uint8_t Hot_start[] =  {0xB5, 0x62, 0x06, 0x04, 0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x10, 0x68};

uint8_t ubxRate1Hz[]= {0xB5, 0x62,0x06,0x08,0x06,0x00,0xE8,0x03,0x01,0x00,0x01,0x00, 0x01,0x39};
uint8_t ubxRate5Hz[]= {0xB5, 0x62,0x06,0x08,0x06,0x00,200,0x00,0x01,0x00,0x01,0x00,0xDE,0x6A};
uint8_t ubxRate10Hz[]={0xB5, 0x62,0x06,0x08,0x06,0x00,100,0x00,0x01,0x00,0x01,0x00,0x7A,0x12};


uint8_t clrGLL[] = {0xB5 ,0x62 ,0x06 ,0x01 ,0x08 ,0x00 ,0xF0 ,0x01 ,0x00, 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x2A};
uint8_t clrGSV[] = {0xB5 ,0x62 ,0x06 ,0x01 ,0x03 ,0x00 ,0xF0 ,0x03 ,0x00 ,0xFD ,0x15};
uint8_t clrGSA[] = {0xB5 ,0x62 ,0x06 ,0x01 ,0x03 ,0x00 ,0xF0 ,0x02 ,0x00 ,0xFC ,0x13};
uint8_t clrRMC[] = {0xB5 ,0x62 ,0x06 ,0x01 ,0x03 ,0x00 ,0xF0 ,0x04 ,0x00 ,0xFE ,0x17};
uint8_t clrVTG[] = {0xB5 ,0x62 ,0x06 ,0x01 ,0x03 ,0x00 ,0xF0 ,0x05 ,0x00 ,0xFF ,0x19};
uint8_t clrDTM[] = {0xB5 ,0x62 ,0x06 ,0x01 ,0x03 ,0x00 ,0xF0 ,0x0A ,0x00 ,0x04 ,0x23};
uint8_t clrGBS[] = {0xB5 ,0x62 ,0x06 ,0x01 ,0x03 ,0x00 ,0xF0 ,0x09 ,0x00 ,0x03 ,0x21};
uint8_t clrGNS[] = {0xB5 ,0x62 ,0x06 ,0x01 ,0x03 ,0x00 ,0xF0 ,0x0D ,0x00 ,0x07 ,0x29};
uint8_t clrGRS[] = {0xB5 ,0x62 ,0x06 ,0x01 ,0x03 ,0x00 ,0xF0 ,0x06 ,0x00 ,0x00 ,0x1B};
uint8_t clrGST[] = {0xB5 ,0x62 ,0x06 ,0x01 ,0x03 ,0x00 ,0xF0 ,0x07 ,0x00 ,0x01 ,0x1D};
uint8_t clrTHS[] = {0xB5 ,0x62 ,0x06 ,0x01 ,0x03 ,0x00 ,0xF0 ,0x0E ,0x00 ,0x08 ,0x2B};
uint8_t clrVLW[] = {0xB5 ,0x62 ,0x06 ,0x01 ,0x03 ,0x00 ,0xF0 ,0x0F ,0x00 ,0x09 ,0x2D};
uint8_t clrZDA[] = {0xB5 ,0x62 ,0x06 ,0x01 ,0x03 ,0x00 ,0xF0 ,0x08 ,0x00 ,0x02 ,0x1F};


uint8_t ch;
uint8_t str[76];

int32_t degree;
long minutes;
char degreebuff[10];
float timef;
uint32_t time;
uint8_t hour , minute , seconds ;
uint8_t day , year , month ;

long degree, degree1;
float minutes1, minutes2;
int LongDir= 0, LatDir = 0;
float latitude , longitude ;
int32_t latitude_fixed , longitude_fixed ;
float latitude_Degrees , longitude_Degrees ;
float speed , angle ;


void print(char* format , ...)
{
	char str[80];

	va_list args;

	va_start(args , format);

	vsprintf(str , format , args);
	HAL_UART_Transmit(&huart2 , (uint8_t*)str , strlen(str) , HAL_MAX_DELAY);

	va_end(args);
}


void ProcessGPSLine(uint8_t* GPSBuffer)
{

   print("GNGGA Detected!\r\n");

   uint8_t *p = GPSBuffer;

   print(" %s\r\n",p);

   //Time parsing
   	p = strchr(p , ',')+1;
   	timef = atof(p);
   	time = timef;
   	hour = time/10000;
   	minute = (time % 10000)/100 ;
   	seconds = (time % 100);
   	print("hour : min : sec %d: %d: %d\r\n" , hour,minute,seconds);

   	p = strchr(p , ',')+1;

//   	printf("%s\r\n lat str",p);
   		    if (',' != *p)
   		    {
   		      strncpy(degreebuff, p, 2);
   		      p += 2;
   		      degreebuff[2] = '\0';
   		      degree = atol(degreebuff);
//   			print("degree =%d\r\n ",degree);
   		      char data[9];
   		      strncpy(data, p , 8);
//   			print("data = %s\r\n",data);
   		      p=p+8;
//   		      printf("p = %s\r\n",p);
   		      data[8] = '\0';
//   			print("data %s\r\n",data);
   		      minutes1 = atof(data);
//   		      print("minutes = %f\r\n",minutes1);
   		      latitude = (degree + ((float)(minutes1)/60));
   			print("latitude = %f\r\n",latitude);
   		    }

//   		  /*    // Latitude Direction South or North
   		    p = strchr(p , ',')+1;
//   		    printf("parsed str = %s\r\n" , p);
   		 	if(',' != *p)
   		 	{
   		 		LatDir = p[0];

   		 		if(p[0]=='N')
   		 			print("North..\r\n" , p[0]);
   		 		else if(p[0]=='S')
   		 			print("South..\r\n",p[0]);
   		  	}

   		 p = strchr(p , ',')+1;
   		if(',' != *p)
   		{

//   			print("p = %s\r\n",p);
   			strncpy(degreebuff , p ,3);
   															// taking the longitude degree and seconds
   			p += 3;
   			degreebuff[3] = '\0';
//   			print("degreebuff %s\r\n",degreebuff);
   			degree1 = atol(degreebuff);
   			char data1[9];											// Parse the only degree
   			strncpy(data1, p, 8);
   			p = p+6;
   			data1[8] = '\0';
//   			print("data1 %s\r\n",data1);
   			minutes2 = atof(data1);
//   			printf("minutes2 %f\r\n",minutes2);
   			longitude = (degree1 + ((float)(minutes2)/60));							// final longitude coordinates
   			print("longitude %f\r\n",longitude);
   		}
   		p = strchr(p , ',')+1;
//   		printf("parsed string %s\r\n",p);
             if(',' != *p)
             {
            	 LatDir = p[0];

            	 if(p[0]=='E')
            		 print("East..\r\n" , p[0]);
            	 else if(p[0]=='W')
            		 print("West..\r\n",p[0]);
             }
         p = strchr(p , ',')+1;
         p = strchr(p , ',')+1;
         p = strchr(p , ',')+1;
         p = strchr(p , ',')+1;

//   		print("p = %s\r\n",p);
   		if(',' != *p)
        {
         char alti[6];
   		strncpy(alti , p , 5);
   		 printf("altt  %s\r\n",alti);
   			   p = p+6;
   			  alti[5] = '\0';
//   			print("alti %s\r\n",alti);
   			float altitude =atof(alti);
   			print("altitude = %.1f\r\n",altitude);

   			}

}

void GPS_SendCommands()
{
	HAL_UART_Transmit(&huart1,(uint8_t*)Hot_start, sizeof(Hot_start),1000);
	HAL_UART_Transmit(&huart1 ,(uint8_t*)ubxRate1Hz, sizeof(ubxRate1Hz),1000);

	HAL_UART_Transmit(&huart1,(uint8_t*)clrGLL , sizeof(clrGLL),1000);
	HAL_UART_Transmit(&huart1,(uint8_t*)clrGSA , sizeof(clrGSA),1000);
	HAL_UART_Transmit(&huart1,(uint8_t*) clrGSV , sizeof(clrGSV),1000);
	HAL_UART_Transmit(&huart1,(uint8_t*)clrRMC , sizeof(clrRMC),1000);
	HAL_UART_Transmit(&huart1,(uint8_t*)clrVTG , sizeof(clrVTG),1000);
	HAL_UART_Transmit(&huart1,(uint8_t*)clrZDA , sizeof(clrZDA),1000);
	HAL_UART_Transmit(&huart1,(uint8_t*) clrDTM , sizeof(clrDTM),1000);
	HAL_UART_Transmit(&huart1,(uint8_t*)clrGBS, sizeof(clrGBS),1000);
	HAL_UART_Transmit(&huart1,(uint8_t*) clrGNS, sizeof(clrGNS),1000);
	HAL_UART_Transmit(&huart1,(uint8_t*)clrGRS, sizeof(clrGRS),1000);
	HAL_UART_Transmit(&huart1,(uint8_t*) clrGST, sizeof(clrGST),1000);
	HAL_UART_Transmit(&huart1,(uint8_t*)clrTHS, sizeof(clrTHS),1000);
	HAL_UART_Transmit(&huart1,(uint8_t*)clrVLW, sizeof(clrVLW),1000);

}
/* USER CODE END PD */
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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */

   GPS_SendCommands();


  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	  HAL_Delay(15000);
	  	HAL_UART_Receive_IT(&huart1 , (uint8_t*)str , 76 );

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_HSI;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
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
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
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
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	ProcessGPSLine(str);
}
/* USER CODE END 4 */
/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
