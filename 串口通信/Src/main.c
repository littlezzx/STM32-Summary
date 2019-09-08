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
I2C_HandleTypeDef hi2c2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

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
uint8_t shuzu[]={
	0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,
	0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e
};
void DisplayOnebit(uint8_t digtal,uint8_t bit){
		uint8_t i;
		for(i=0;i<8;i++){
			HAL_GPIO_WritePin(segs[i].port,segs[i].pin,(GPIO_PinState)(shuzu[digtal]&(0x01<<i)));
		}
		for(i=0;i<4;i++){
			HAL_GPIO_WritePin(bits[i].port,bits[i].pin,(GPIO_PinState)(i!=bit));
		}
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
//0到9  要在数码管显示 十六进制值 
// 扫描按键使用到的gpio口 弄到结构数组里面去
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C2_Init(void);
/* USER CODE BEGIN PFP */
uint8_t connctt[15]="hello world";
uint8_t connctt2[25]="The Key is ";	
uint8_t *receive;  
uint8_t receive_str[4]="0";  // 串口发送接收到的值 
int number=0;
int receive_kye;  //扫描键盘获取到的值
int get_key ; //串口发送的值 转换为int 
int getkey_length=0; // 获取到的值的长度
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
	if (key !=0)
	{
		number = 1;
		uint8_t key_str[5];  //按下的键值 转换成字符型
		//connctt2 ="The Key is ";
		strcpy(connctt2,"The Key is ");
		DisplayDigtal(key);
		Int2Str(key_str,key);  //吧receive_key 按下的键值 转换成字符型 key_str
		strncat(connctt2,key_str,10);  //把key_str 添加到connctt2后面 
		HAL_Delay(500);
		
		HAL_UART_Transmit_IT(&huart1,connctt2,25);
		//HAL_UART_Transmit_IT(&huart1,ReadBuffer,10);
	}else
	number = 0;
		
	}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
	
	
void Int2Str(uint8_t* str, int intnum)   //将无符号整形整数intnum 转换成字符 str
{
    int i, Div = 1000000000, j = 0, Status = 0;
   //32????????10???,??Div=10 0000 0000
    for (i = 0; i < 10; i++)
    {
        str[j++] = (intnum / Div) + '0';//???? ????? 
 
        intnum = intnum % Div;//?????
        Div /= 10;//???10-i????
        if ((str[j-1] == '0') & (Status == 0))//??????'0'
        {
            j = 0;
        }
        else
        {
            Status++;
        }
    }
}
int str2int1( char* str)  // 字符串转整形
{
    int temp = 0;
    const char* p = str;
    if(str == NULL) return 0;
    if(*str == '-' || *str == '+')
    {
        str ++;
    }
    while( *str != 0)
    {
        if( *str < '0' || *str > '9')
        {
            break;
        }
        temp = temp*10 +(*str -'0');
        str ++;
    }
    if(*p == '-')
    {
        temp = -temp;
    }
    return temp;
}
#define ADDR_AT24C02_Write 0xA0
#define ADDR_AT24C02_Read 0xA1

#define BufferSize 0x100


uint16_t i;
uint8_t  AT24c02_flag;//

uint8_t params[4]="0";

uint8_t test_i2c[3];
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
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
	HAL_UART_Receive_IT(&huart1, receive_str,4);

	HAL_I2C_Mem_Read(&hi2c2, ADDR_AT24C02_Read,0,I2C_MEMADD_SIZE_8BIT,receive_str,4,10000);
	HAL_UART_Transmit(&huart1,receive_str,4,50);
	int t =0;
//	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	
		t++;
		Displaykeying();
		
		//get_key = str2int1(receive_str);
		get_key = (receive_str[0]-48)*1000+(receive_str[1]-48)*100+(receive_str[2]-48)*10+(receive_str[3]-48);
		if (t%1000==0)
		{
		HAL_UART_Transmit(&huart1,params,4,50);
			HAL_UART_Transmit(&huart1,receive_str,4,50);
			t=0;
		}
		DisplayDigtal(get_key);

		
	
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_12|GPIO_PIN_13 
                          |GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA4 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_7;
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

  /*Configure GPIO pins : PC6 PC7 PC8 PC9 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 PA11 PA12 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
	//getkey_length = 4-huart1.RxXferCount;
	uint8_t received[4];
		for(int i=0;i<4;i++)
		received[i]=receive_str[i];
	HAL_I2C_Mem_Write(&hi2c2, ADDR_AT24C02_Write, 0, I2C_MEMADD_SIZE_8BIT,received,4, 10000);
	HAL_UART_Transmit(&huart1,received,4,50);
  HAL_UART_Receive_IT(&huart1, receive_str,4);

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
