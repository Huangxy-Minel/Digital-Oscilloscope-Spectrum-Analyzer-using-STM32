#ifndef _LED_H
#define _LED_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/6/10
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//LED�˿ڶ���
#define LED0(n)		(n?HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET))
#define LED0_Toggle (HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0))
#define LED1(n)		(n?HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET))
#define LED1_Toggle (HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1))
#define LED2(n)		(n?HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_RESET))
#define LED2_Toggle (HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_2))
#define LED3(n)		(n?HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_RESET))
#define LED3_Toggle (HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_3))
void LED_Init(void);
#endif
