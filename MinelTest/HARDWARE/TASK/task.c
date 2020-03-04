#include "task.h"
#include "picture.h"
void InitTask(void)
{
	u8 key = 0;
	Cache_Enable();                 //打开L1-Cache
	HAL_Init();				        //初始化HAL库
	Stm32_Clock_Init(432,25,2,9);   //设置时钟,216Mhz 
	delay_init(216);                //延时初始化
	LCD_Init();                     //初始化LCD
	EXTI_Init();
	MY_ADC_Init();
	DAC1_Init();
	SPI_Init();
  POINT_COLOR=BLACK; 
	LCD_ShowString(10,40,260,32,32,"UESTC"); 	
	LCD_ShowString(10,80,240,24,24,"TEAM:CL-08");
	LCD_ShowString(10,110,240,16,16,"STM32F7"); 				
  LCD_DrawPicture(300,80,496,197,(u16*)(gImage_picture+8));
	LED_Init();
	LCD_ShowString(50,360,600,32,24,"Whether to enable the self-test program?");
	LCD_ShowString(50,390,240,32,24,"PRESS KEY_0 YES");	
	LCD_ShowString(50,420,240,32,24,"PRESS KEY_1 NO");
	while(1)
	{
		key = KEY_Scan(1);
		if(key == 1)
		{
			LCD_Clear(BLACK);
			break;
		}
		if(key == 2)
		{
			LCD_Clear(WHITE);
			SelectTask();
			break;
		}
	}
}
void SelectTask(void)
{
	LCD_ShowString(10,40,260,32,32,"UESTC");
	LCD_ShowString(10,80,240,24,24,"TEAM:CL-08");
	LCD_ShowString(10,110,240,16,16,"STM32F7");
	LCD_DrawPicture(300,80,496,197,(u16*)(gImage_picture+8));
	LCD_ShowString(50,330,600,32,24,"Please select mode");
	LCD_ShowString(50,360,600,32,24,"PRESS KEY_0 Oscilloscope");	
	LCD_ShowString(50,390,600,32,24,"PRESS KEY_1 DAC_Mode");
	LCD_ShowString(50,420,600,32,24,"PRESS KEY_2 ADC_Mode");
}
