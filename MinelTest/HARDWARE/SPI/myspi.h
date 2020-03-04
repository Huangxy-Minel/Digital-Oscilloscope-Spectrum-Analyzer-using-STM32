#ifndef _MYSPI_H
#define _MYSPI_H
#include "sys.h"

//SPI相关IO定义
#define SYNC(n)		(n?HAL_GPIO_WritePin(GPIOF,GPIO_PIN_6,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOF,GPIO_PIN_6,GPIO_PIN_RESET))
#define SYNC_Toggle (HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_6)) //IO电平翻转
#define SCLK(n)		(n?HAL_GPIO_WritePin(GPIOF,GPIO_PIN_1,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOF,GPIO_PIN_1,GPIO_PIN_RESET))
#define SCLK_Toggle (HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_1)) //IO电平翻转

#define D1        HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_2)  
#define D2        HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_3)  
#define D3        HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_4) 
#define D4        HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_5)
#define DRDY      HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_0)
void SPI_Init(void);
u32 Get_Adc_ads1274(u8 adc_num);
#endif
