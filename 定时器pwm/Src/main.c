/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
int Tick_ms = 0; // 计时用的 
int led_hight = 0; // 放到callback中 循环显示led灯
int time_meter = 0; // 时间计数
int time_minute =0;
int time_second =0;
int judge =0 ;// 初始化一个值来判断是否让数码管显示    或变换?
int function_key = 16; // 为实现多个功能设置功能选项键能够切换多个功能
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
struct GPIO_PACK{
		GPIO_TypeDef * port;
		uint16_t pin;
	};
	struct GPIO_PACK segs[8]= {
	{GPIOA,GPIO_PIN_15},
	{GPIOA,GPIO_PIN_11},
	{GPIOC,GPIO_PIN_9},
	{GPIOC,GPIO_PIN_7},
	{GPIOC,GPIO_PIN_8},
	{GPIOA,GPIO_PIN_12},
	{GPIOA,GPIO_PIN_8},
	{GPIOC,GPIO_PIN_6},
	}	;
	struct GPIO_PACK bits[4]={
	{GPIOD,GPIO_PIN_2},
	{GPIOB,GPIO_PIN_4},
	{GPIOB,GPIO_PIN_6},
	{GPIOB,GPIO_PIN_7},

	};
	struct GPIO_PACK led_array[8]=
{
	{GPIOA,GPIO_PIN_4},
	{GPIOA,GPIO_PIN_3},
	{GPIOA,GPIO_PIN_2},
	{GPIOA,GPIO_PIN_1},	
	{GPIOA,GPIO_PIN_0},
	{GPIOC,GPIO_PIN_3},
	{GPIOC,GPIO_PIN_2},
	{GPIOC,GPIO_PIN_1},
};
uint8_t shuzu[]={
	0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,
	0xf8,0x80,0x90
};
uint8_t shuzu2[] = {
	0x40,0x79,0x24,0x30,0x19,0x12,0x2,
	0x78,0x0,0x10
};
void delay1ms (unsigned int a){ // 1ms????(12MHz 10??51??????)
	unsigned int i;
	while( a-- != 0){
		for(i = 0; i < 600; i++); 
	}   				   
}
void DisplayOnebit(uint8_t digtal,uint8_t bit){
		uint8_t i;
		for(i=0;i<8;i++){
			HAL_GPIO_WritePin(segs[i].port,segs[i].pin,(GPIO_PinState)(shuzu[digtal]&(0x01<<i)));
		}
		for(i=0;i<4;i++){
			HAL_GPIO_WritePin(bits[i].port,bits[i].pin,(GPIO_PinState)(i!=bit));
		}
		delay1ms(5);
}
void DisplayOnebit2(uint8_t digtal,uint8_t bit){
		uint8_t i;
		for(i=0;i<8;i++){
			HAL_GPIO_WritePin(segs[i].port,segs[i].pin,(GPIO_PinState)(shuzu2[digtal]&(0x01<<i)));
		}
		for(i=0;i<4;i++){
			HAL_GPIO_WritePin(bits[i].port,bits[i].pin,(GPIO_PinState)(i!=bit));
		}
		//HAL_Delay(2);
		delay1ms(5);
}

void DisplayDigtal(uint16_t digtal){
	int a=0;
	
	DisplayOnebit(digtal%10,0);
	HAL_Delay(5);
	if (digtal>=10){
	DisplayOnebit(digtal/10%10,1);
	HAL_Delay(5);
	if (digtal>=100){
	DisplayOnebit(digtal/100%10,2);
	HAL_Delay(5);
	if (digtal>=1000){
	DisplayOnebit(digtal/1000%10,3);
	HAL_Delay(5);
		
				}
			}
		}
}
struct GPIO_PACK lie[4]={   
   {GPIOB,GPIO_PIN_0},
   {GPIOB,GPIO_PIN_1},
   {GPIOB,GPIO_PIN_13},
   {GPIOB,GPIO_PIN_12}
};
struct GPIO_PACK hang[4]={
   {GPIOA,GPIO_PIN_6},
   {GPIOA,GPIO_PIN_7},
   {GPIOC,GPIO_PIN_4},
   {GPIOC,GPIO_PIN_5}
};
void Displaykeying()
	{
		uint8_t i,j,s,key=0;uint8_t k=1;
		for(i=0;i<4;i++)
	{	 HAL_GPIO_WritePin(lie[i].port,lie[i].pin,GPIO_PIN_RESET);	
		for(s=0;s<4;s++){
		if(i!=s)
			HAL_GPIO_WritePin(lie[s].port,lie[s].pin,GPIO_PIN_SET);}
		for(j=0;j<4;j++)
	{	k=HAL_GPIO_ReadPin(hang[j].port,hang[j].pin);
		if(k==0)
			HAL_Delay(5);
			if(k==0)
			switch(j)
			{
				case 0:key=j+1+i;break;
				case 1:key=j+4+i;break;
				case 2:key=j+7+i;break;
				case 3:key=j+10+i;break;
			}			  	
		}		
	}	
	if (key == 15)
		function_key =15;  // 数码管显示的是 计时功能
	else if (key == 16)
			function_key = 16; // 数码管显示的是 led灯亮
	else if(key == 14)
		function_key = 14; // pwm 
}

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
	// HAL_SYSTICK_IRQHandler();      
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
//	uint16_t cp=100,cs=300;
//	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,20);
//   __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,20);
//    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
//    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
	// HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_1);
	uint16_t dutyCycle = 0;
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		Displaykeying();
		
		if (function_key == 14)
		{	
			
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,0);
			
			while (dutyCycle < 1000)
				{
					dutyCycle = dutyCycle+20;
					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, dutyCycle);
					HAL_Delay(10);
				}
				while (dutyCycle)
				{
					dutyCycle =dutyCycle-20;
					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, dutyCycle);

					HAL_Delay(10);
				}
				HAL_Delay(50);
			}
		
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

  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 72-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM2;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_12|GPIO_PIN_13 
                          |GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC1 PC2 PC3 PC6 
                           PC7 PC8 PC9 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6 
                          |GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3 
                           PA4 PA8 PA11 PA12 
                           PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_8|GPIO_PIN_11|GPIO_PIN_12 
                          |GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PC4 PC5 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB12 PB13 
                           PB4 PB6 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_12|GPIO_PIN_13 
                          |GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_SYSTICK_Callback(void)
{
	
	Tick_ms++;
	if(Tick_ms%1000==0) 
	{
		if (function_key!=14 ) {
			if(led_hight!=4)
				HAL_GPIO_WritePin(led_array[4].port,led_array[4].pin,1);
		//HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_4);
		if (led_hight==0)
			HAL_GPIO_TogglePin(led_array[7].port,led_array[7].pin);
		else
				HAL_GPIO_TogglePin(led_array[led_hight-1].port,led_array[led_hight-1].pin);
		HAL_GPIO_TogglePin(led_array[led_hight].port,led_array[led_hight].pin);// 反转函数	
		  led_hight ++; 
	}else  //如果执行功能14 需要把所有灯都先关掉
	{	
		for(int j=0;j<8;j++)
			{
			HAL_GPIO_WritePin(led_array[j].port,led_array[j].pin,1);
			}
		led_hight = 0;
			
		}			
		
		Tick_ms=0;
		
		time_second++;
		
		if (led_hight == 8)
			led_hight=0;
		if (time_second == 60)
		{
			time_minute ++;
			time_second = 0;
		}
		if (time_minute ==60& time_second ==60)
		{
			time_minute =0;
			time_second = 0;
		}
	}
	//DisplayDigtal(led_hight);
	if (function_key == 16)
		DisplayOnebit(led_hight,0);
	if(function_key == 15)
	{
		if (time_second<10)
		{
			DisplayOnebit(time_second,0);
			if (time_minute<10)
				DisplayOnebit2(time_minute,1);
			else {
					DisplayOnebit2(time_minute%10,1);
				 DisplayOnebit(time_minute/10,2);
			}
		}
		else{
				DisplayOnebit(time_second%10,0);
				DisplayOnebit(time_second/10,1);
				if (time_minute<10)
				DisplayOnebit2(time_minute,2);
			else {
					DisplayOnebit2(time_minute%10,2);
				 DisplayOnebit(time_minute/10,3);
			}
		}

		
	}
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
