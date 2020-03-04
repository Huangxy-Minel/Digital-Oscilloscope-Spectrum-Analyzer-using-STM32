#ifndef _KEY_H
#define _KEY_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//KEY��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/11/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
#define KEY0        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_0)  //KEY0����PH3
#define KEY1        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_1)  //KEY1����PH2
#define KEY2        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_8) //KEY2����PC13
#define KEY3        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9)  //WKUP����PA0



#define KEY0_PRES 1
#define KEY1_PRES	2
#define KEY2_PRES	3
#define KEY3_PRES 4


void KEY_Init(void);
u8 KEY_Scan(u8 mode);
#endif
