#ifndef _TASK_H
#define _TASK_H
#include "sys.h"
#include "delay.h"
#include "lcd.h"
#include "led.h"
#include "exti.h"
#include "adc.h"
#include "dac.h"
#include "myspi.h"

void InitTask(void);
void SelectTask(void);

#endif
