/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F429I_DISCOVERY_Examples
  * @{
  */

/** @addtogroup ADC_DMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
  #define MESSAGE0   "ADC =%5d"
	#define MESSAGE    "ADC =%4d"
	#define MESSAGE1   "ADC conversion w/DMA"
  #define MESSAGE1_1 "continuouslyTransfer" 
  #define MESSAGE2   " ADC PG13 Conv   "
  #define MESSAGE2_1 "    2.4Msps      "
  #define MESSAGE5   " ADC3 = %d.%1d V "
	#define MESSAGE6   " ADC2 = %d.%1d V "
  #define LINENUM            0x15
  #define FONTSIZE         Font12x12
	#define ADC2_DR_ADDRESS     ((uint32_t)0x4001124C)
  #define ADC3_DR_ADDRESS     ((uint32_t)0x4001224C)
	#define ADC_CCR_ADDRESS     ((uint32_t)0x40012308)
	
	#define DMA_STREAM               DMA2_Stream0
	#define DMA_IT_TCIF              DMA_IT_TCIF0
	#define DMA_STREAM_IRQ           DMA2_Stream0_IRQn

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t n;
uint32_t t;
uint16_t D1=0,D2=0;
char Text[20];
__IO uint16_t aADCDualConvertedValue[2],Aux=0;
__IO uint16_t Muest=0;
/* You can monitor the converted value by adding the variable "uhADC3ConvertedValue" 
   to the debugger watch window */
__IO uint16_t uhADC3ConvertedValue = 0,uhADC2ConvertedValue = 0;
__IO uint32_t uwADC3ConvertedVoltage = 0,uwADC2ConvertedVoltage = 0;
uint16_t Res=0,Res1=0,Res0=0;

/* Private function prototypes -----------------------------------------------*/
static void DMA_Config(void);
static void GPIO_Config(void);
static void ADC1_CH13_Config(void);
static void ADC2_CH5_Config(void);
void R_ADC(void);

#ifdef USE_LCD
static void Display_Init(void);
void Init_GPIO(void);
#endif /* USE_LCD */

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
	/* Enable peripheral clocks *************************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	
	Init_GPIO();
	
	GPIO_Config();
	
	#ifdef USE_LCD 
  /* LCD Display init  */
  Display_Init();
  #endif /* USE_LCD */
	
	/* DMA2 Stream0 channel0 configuration **************************************/
  DMA_Config();

  /* ADC Common Init */
  ADC_CommonInitStructure.ADC_Mode = ADC_DualMode_RegSimult;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC1 regular channels 10, 11 configuration */
  ADC1_CH13_Config();
  /* ADC2 regular channels 11, 12 configuration */
  ADC2_CH5_Config();

  /* Enable DMA request after last transfer (Multi-ADC mode) */
  ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);
	
	ADC_DMACmd(ADC1,ENABLE);
	ADC_DMACmd(ADC2,ENABLE);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  /* Enable ADC2 */
  ADC_Cmd(ADC2, ENABLE);
	
	ADC_ContinuousModeCmd(ADC1,ENABLE);
	ADC_ContinuousModeCmd(ADC2,ENABLE);

  /* Start ADCx Software Conversion */
  ADC_SoftwareStartConv(ADC1);
	ADC_SoftwareStartConv(ADC2);
	
	LCD_SetFont(&Font16x24);
	LCD_SetTextColor(LCD_COLOR_RED);
	//LCD_DisplayStringLine(LCD_LINE_1,"AnaLogic");
	GPIO_SetBits(GPIOE, GPIO_Pin_6);
	GPIO_SetBits(GPIOE, GPIO_Pin_4);

  while (1)
  {
    /* convert the ADC value (from 0 to 0xFFF) to a voltage value (from 0V to 3.0V)*/
    //ADC1 Channel 13 -> PC3
	  //ADC2 Channel 5  -> PA5
    /* Display ADCs converted values on LCD */
#ifdef USE_LCD 
    for(n=0;n<1920;n++)
		{
		  LCD_SetTextColor(LCD_COLOR_BLUE);
			//Aux=aADCDualConvertedValue[1]-100;
			//if((Aux)<0){Aux=1;}
			LCD_DrawFullCircle(n/8, 1+(Res), 1);
			snprintf(Text, sizeof(Text), "%d", Res);
			LCD_SetTextColor(LCD_COLOR_BLACK);
	 	  LCD_DisplayStringLine(LCD_LINE_1,(uint8_t*)Text);
			//for(t=0;t<100000;t++){;}
			R_ADC();
		}
		LCD_Clear(LCD_COLOR_GREY);
		//Display(); (240x320)
		//Resolución	320 x 480 Pixi LCD Cel
		/*LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_DrawFullRect(40, 0, 150, 320);
		
		D2=10+aADCDualConvertedValue[1];
		
		LCD_SetTextColor(LCD_COLOR_BLUE);
		LCD_DrawFullRect(140, 0, 50, D2);*/
#endif /* USE_LCD */
  }
}

void R_ADC()
{
	//IRLed_On	**********************
	GPIO_ResetBits(GPIOE, GPIO_Pin_4);
	for(t=0;t<50000;t++){;}
	Res1=aADCDualConvertedValue[1];
	if(Res1>1000){Res1=0;}
		
	//IRLed_Off	**********************
	GPIO_SetBits(GPIOE, GPIO_Pin_4);
	for(t=0;t<50000;t++){;}
	Res0=aADCDualConvertedValue[1];
	if(Res0>1000){Res0=0;}
		
	//		1			0
	Res=Res1-Res0;
	if(Res>1000){Res=1;}
	if(Res<1){Res=0;}
}

/**
  * @brief  ADC1 regular channels 10 and 11 configuration
  * @param  None
  * @retval None
  */
static void ADC1_CH13_Config(void)
{
  ADC_InitTypeDef ADC_InitStructure;

  ADC_InitStructure.ADC_Resolution = ADC_Resolution_10b;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;//2
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel 3 configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_3Cycles);
  //ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_3Cycles);
}

/**
  * @brief  ADC2 regular channels 11, 12 configuration
  * @param  None
  * @retval None
  */
static void ADC2_CH5_Config(void)
{
  ADC_InitTypeDef ADC_InitStructure;

  ADC_InitStructure.ADC_Resolution = ADC_Resolution_10b;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;//2
  ADC_Init(ADC2, &ADC_InitStructure);

  /* ADC2 regular channel 5 configuration */ 
  ADC_RegularChannelConfig(ADC2, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);
  //ADC_RegularChannelConfig(ADC2, ADC_Channel_12, 2, ADC_SampleTime_3Cycles);
}

/**
  * @brief  DMA Configuration
  * @param  None
  * @retval None
  */
static void DMA_Config(void)
{
  DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef      NVIC_InitStructure;

  DMA_InitStructure.DMA_Channel = DMA_Channel_0; 
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&aADCDualConvertedValue;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC_CCR_ADDRESS;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 2;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);

  /* DMA2_Stream0 enable */
  DMA_Cmd(DMA2_Stream0, ENABLE);
	
	//Enable DMA Stream Transfer Complete interrupt 
	DMA_ITConfig(DMA_STREAM, DMA_IT_TC, ENABLE);
	
	// Enable the DMA Stream IRQ Channel
	  NVIC_InitStructure.NVIC_IRQChannel = DMA_STREAM_IRQ;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief Configure ADC Channels 10, 11, 12 pins as analog inputs
  * @param  None
  * @retval None
  */
static void GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* 
     ADC1 Channel 13 -> PC3
	   ADC2 Channel 5  -> PA5
  */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

volatile uint8_t i;

void DMA2_Stream0_IRQHandler(void)
{
	GPIO_ToggleBits(GPIOG, GPIO_Pin_13);
	//GPIO_SetBits(GPIOG, GPIO_Pin_14);
	Muest++;
	
	// Test on DMA Stream Transfer Complete interrupt
	if (DMA_GetITStatus(DMA_STREAM, DMA_IT_TCIF))
	{
		// Clear DMA Stream Transfer Complete interrupt pending bit
		DMA_ClearITPendingBit(DMA_STREAM, DMA_IT_TCIF);
		i = 1;//hay diferencia kon esto y sin esto
	}
}


void Init_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;				//PORTG
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14; // we want to configure all LED GPIO pins
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; 		// we want the pins to be an output
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz; 	// this sets the GPIO modules clock speed
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; 	// this sets the pin type to push / pull (as opposed to open drain)
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; 	// this sets the pullup / pulldown resistors to be inactive
	GPIO_Init(GPIOG, &GPIO_InitStruct); 			// this finally passes all the values to the GPIO_Init function which takes care of setting the corresponding bits.
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_4; // we want to configure all LED GPIO pins
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; 		// we want the pins to be an output
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz; 	// this sets the GPIO modules clock speed
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; 	// this sets the pin type to push / pull (as opposed to open drain)
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; 	// this sets the pullup / pulldown resistors to be inactive
	GPIO_Init(GPIOE, &GPIO_InitStruct); 
}

#ifdef USE_LCD
/**
  * @brief  Display ADCs converted values on LCD
  * @param  None
  * @retval None
  */


/**
  * @brief  Display Init (LCD)
  * @param  None
  * @retval None
  */
static void Display_Init(void)
{
  /* Initialize the LCD */
  LCD_Init();
  LCD_LayerInit();
  /* Enable the LTDC */
  LTDC_Cmd(ENABLE);
  
  /* Set LCD Background Layer  */
  LCD_SetLayer(LCD_BACKGROUND_LAYER);
  
  /* Clear the Background Layer */ 
  LCD_Clear(LCD_COLOR_WHITE);
  
  /* Configure the transparency for background */
  LCD_SetTransparency(0);
  
  /* Set LCD Foreground Layer  */
  LCD_SetLayer(LCD_FOREGROUND_LAYER);

  /* Configure the transparency for foreground */
  LCD_SetTransparency(200);
  
  /* Clear the Foreground Layer */ 
  LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set the LCD Back Color and Text Color*/
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  
    /* Set the LCD Text size */
  LCD_SetFont(&FONTSIZE);
  
  /* Set the LCD Back Color and Text Color*/
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  
  
  /* Set the LCD Back Color and Text Color*/
  LCD_SetBackColor(LCD_COLOR_WHITE);
  LCD_SetTextColor(LCD_COLOR_BLUE); 
}
#endif /* USE_LCD */

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
