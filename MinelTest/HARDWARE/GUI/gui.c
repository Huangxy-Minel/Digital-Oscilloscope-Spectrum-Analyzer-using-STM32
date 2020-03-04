#include "gui.h"
#include "lcd.h"
#include "touch.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "adc.h"
#include "led.h"

extern unsigned char gImage_XuZhe[78408];
extern unsigned char gImage_Zobin[20008];
extern unsigned char gImage_logo[18058];
extern u8 state;//全局变量：状态列表。
extern u32 adc_ch;
extern u8 adc_state;
u16 adc_data[1024];
void Load_Drow_Dialog(void)//主界面函数
{
	LCD_Clear(WHITE);//清屏   
 	POINT_COLOR=BLACK;//设置字体为蓝色 
	LCD_ShowxNum(600,10,state,2,16,0);
	LCD_ShowString(20,35,500,32,32,"ZoBin OS Alpha1.0");
	LCD_DrawPicture(460,10,555,93,(u16*)(gImage_logo+8));
	LCD_Fill(0,85,470,95,BLUE);
	LCD_Fill(0,100,230,110,BLUE);
	LCD_Fill(0,400,250,410,BLUE);
	LCD_Fill(0,420,470,430,BLUE);
	LCD_DrawRectangle(490,120,715,385);
	LCD_DrawPicture(500,140,600,123,(u16*)(gImage_XuZhe+8));
	LCD_DrawPicture(600,200,700,98,(u16*)(gImage_Zobin+8));
	LCD_ShowString(520,330,240,32,32,"2019 NUEDC");

	
	POINT_COLOR=RED;
	LCD_ShowString(260,100,500,16,16,"Defended on Mar 27,2019");
	LCD_ShowString(260,396,500,16,16,"UESTC XuZhe,HuangZobin");
	POINT_COLOR=BLACK;
	LCD_DrawRectangle(15,140,350,185);
	LCD_ShowString(20,150,500,32,32,"ADC Channels Check");
	LCD_DrawRectangle(15,200,350,245);
	LCD_ShowString(20,210,500,32,32,"DAC Channels Check");
	LCD_DrawRectangle(15,260,350,305);
	LCD_ShowString(20,270,500,32,32,"FFT For Spectrum");
	LCD_DrawRectangle(15,320,350,365);
	LCD_ShowString(20,330,500,32,32,"Reset The System");
	
}

//LCD_DrawRectangle(15,320,350,365);
//电阻触摸屏测试函数
void rtp_test(void)
{
	u8 key;
	u8 i=0;	  
	Load_Drow_Dialog();//加载界面
	delay_ms(200);
	while(1)
	{
		if(state == 0)
		{
//				key=KEY_Scan(0);
				tp_dev.scan(0); 		 
				if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
				{	
					if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
					{	
						if(tp_dev.x[0]>15&&tp_dev.y[0]>140&&tp_dev.x[0]<350&&tp_dev.y[0]<185)
						{
							state = 1;//进入AD检测模式
							tp_dev.x[0]=0;
							tp_dev.y[0]=0;
							break;
						}
						else if(tp_dev.x[0]>15&&tp_dev.y[0]>200&&tp_dev.x[0]<350&&tp_dev.y[0]<245)
						{	
							state = 2;//进入DA检测模式
							tp_dev.x[0]=0;
							tp_dev.y[0]=0;
							break;
						}
						else if(tp_dev.x[0]>15&&tp_dev.y[0]>260&&tp_dev.x[0]<350&&tp_dev.y[0]<305)
						{	
							state = 3;//进入FFT频谱分析模式
							tp_dev.x[0]=0;
							tp_dev.y[0]=0;
							break;
						}
						else if(tp_dev.x[0]>15&&tp_dev.y[0]>320&&tp_dev.x[0]<350&&tp_dev.y[0]<365)
						{	
							Load_Drow_Dialog();//进入重置模式
							state = 0;
							break;
						}
							//else TP_Draw_Big_Point(tp_dev.x[0],tp_dev.y[0],RED);		//画图	  
					}						
			}
		}
//		else delay_ms(10);	//没有按键按下的时候 	   
//		
//		if(key==KEY0_PRES)	//KEY0按下,则执行校准程序
//		{
//			LCD_Clear(WHITE);	//清屏
//		    TP_Adjust();  		//屏幕校准 
//			TP_Save_Adjdata();	 
//			Load_Drow_Dialog();
//		}
//		i++;
//		if(i%20==0)LED0_Toggle;
	}
}

void adc_mode(void)
	{
		u16 i = 0;
		LCD_Clear(WHITE);
		LCD_ShowString(15,20,500,32,32,"ADC Channel Catch Waveform");
		LCD_ShowxNum(600,420,state,2,16,0);
		LCD_DrawRectangle(0,50,799,305);
		POINT_COLOR=RED;
		LCD_ShowString(15,310,2000,16,16,"Tips: Use KEY0 To Return , Use KEY1 To Change Channel ， Use KEY2 To Start Convert");
		POINT_COLOR=BLACK;
		LCD_ShowString(15,350,1000,16,16,"Current ADC Channel:");
		LCD_ShowxNum(175,350,adc_ch,2,16,0);
		LCD_ShowString(15,370,500,16,16,"ADC Channel   is");
		LCD_ShowxNum(100,370,adc_ch,2,16,0);
		if(adc_state == 0)
		{
			LCD_ShowString(148,370,500,16,16,"Off");
		}else if(adc_state == 1)
		{
			LCD_ShowString(148,370,500,16,16,"On");
		}
		
		while(1)
			{
				
				if(state == 0)
				{
					break;
				}
				else 
				{
					if(adc_state == 1)
					{
						for(i=0;i<1024;i++)
							{
								adc_data[i]=Get_Adc(adc_ch);
							}
						delay_ms(100);
						LCD_Fill(0,55,799,300,WHITE);
						POINT_COLOR=BLUE;
						for(i=0;i<799;i++)
							{
									LCD_DrawLine(i,(adc_data[i]/15)+100,i+1,(adc_data[i+1]/15)+100);
							}
						POINT_COLOR=BLACK;
					}
					
					if(adc_state == 0)
						{
								
						}
				}
			}
	}
	
	