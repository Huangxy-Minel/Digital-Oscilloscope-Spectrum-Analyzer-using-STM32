#include "task.h"
#include "ffttask.h"
int main(void)
{
	u8 key;
	InitTask();
	key = 0;
  while(1) 
	{
		key = KEY_Scan(1);
		switch(key)
		{
			case 0:break;
			case 1:FFTTask();break;
		}
		LED0_Toggle;
	}

}
