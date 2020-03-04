#include "myspi.h"
#include "delay.h"

void SPI_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
  __HAL_RCC_GPIOF_CLK_ENABLE();			//开启GPIOF时钟
  //__HAL_RCC_GPIOH_CLK_ENABLE();			//开启GPIOF时钟
	
  GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5; //PF0,2,3,4,5--DRDY,D1,D2,D3,D4
	//GPIO_Initure.Pin=GPIO_PIN_2;
  GPIO_Initure.Mode=GPIO_MODE_INPUT;  //输入
  GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
  GPIO_Initure.Speed=GPIO_SPEED_FAST;     //高速
  HAL_GPIO_Init(GPIOF,&GPIO_Initure);
	
	GPIO_Initure.Pin=GPIO_PIN_1|GPIO_PIN_6; //PF6,1--SYNC,SCLK
	//GPIO_Initure.Pin=GPIO_PIN_1;
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
  GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
  GPIO_Initure.Speed=GPIO_SPEED_FAST;     //高速
  HAL_GPIO_Init(GPIOF,&GPIO_Initure);
	
	HAL_GPIO_WritePin(GPIOF,GPIO_PIN_6,GPIO_PIN_RESET);	//SYNC置0
	delay_us(1);
	HAL_GPIO_WritePin(GPIOF,GPIO_PIN_6,GPIO_PIN_SET);	//SYNC置0
	HAL_GPIO_WritePin(GPIOF,GPIO_PIN_1,GPIO_PIN_RESET);	//SCLK置0
	
}

u32 Get_Adc_ads1274(u8 adc_num)
{
	u8 i=0,j;
	u32 adc_buf = 0;
//	SYNC(0);
//	SCLK(0);
//	SYNC(1); //建立连接
	while(1)
	{
		if(DRDY == 0)
		{
			for(j=0;j<23;j++)
			{
				SCLK(1);
				//delay_us(50);
				adc_buf += D1;
				SCLK(0);
				//delay_us(50);
				adc_buf <<= 1;
			}
			adc_buf += D1;
			break;
		}
	}
	adc_buf = ~adc_buf + 1;
	adc_buf &= 0x00FFFFFF;
//	SYNC(0);
//	if(adc_buf & 0x800000)
//		adc_buf |= 0xFF000000;
	return adc_buf;
}

