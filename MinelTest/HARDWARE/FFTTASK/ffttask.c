#include "task.h"
#include "ffttask.h"
#include "fir.h"
#include "iir.h"
#define FFT_LENGTH  4096   //FFT 长度，默认是 1024 点 FFT
static float fft_inputbuf[FFT_LENGTH*2];	//FFT输入数组
static float fft_outputbuf[FFT_LENGTH];	//FFT输出数组
static float fft_powerbuf[FFT_LENGTH];
static u32 adc_ch = 0;//全局变量：ADC通道,adc状态
static u16 idx[10] = {0};
static u16 Fs = 52720;   //采样频率
static float harmonic_f[10] = {0};
static float k1 = 19.686373f;
static float k2 = 12.665644f;
static float Am[5] = {0};
static u16 adcinput[800];
void FindIdx(void); //寻找各次谐波角标
float correct_f(u16 n,float f);
float correct_f_2(u16 idx);
float correct_f_3(u16 idx);
void calculate_f(void);
void calculate_A(void);
void ShowTask(void);
void fir_task(void);
void iir_task(void);
void FFTTask(void)
{
	u16 i;
	arm_cfft_radix4_instance_f32 scfft;//初始化FFT算法
	arm_cfft_radix4_init_f32(&scfft,FFT_LENGTH,0,1);
	LCD_Clear(WHITE);
	while(1)
	{
		for(i=0;i<FFT_LENGTH;i++)					//adc采集数据
			fft_inputbuf[2*i] = Get_Adc_ads1274(1);
		for(i=0;i<FFT_LENGTH;i++)
		{
			fft_inputbuf[2*i+1] = 0;
			fft_inputbuf[2*i] /= 500.f;
		}
		for(i=0;i<FFT_LENGTH;i++)					//加窗
			fft_inputbuf[2*i]*=0.5f*(1-cos(2*PI*((float)i)/(4096-1)));
		//fir_task();
		//iir_task();
		arm_cfft_radix4_f32(&scfft,fft_inputbuf);
		arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,FFT_LENGTH);//送FFT
		for(i=0;i<FFT_LENGTH;i++)  //估计信号功率谱
			fft_powerbuf[i] = pow(fft_outputbuf[i],2)/FFT_LENGTH;
		FindIdx();
		calculate_f();
		calculate_A();
		ShowTask();
	}
}
void fir_task(void)
{
	u16 i,j;
	static float outputdata[FFT_LENGTH];
	static float yn = 0;
	for(j=0;j<FFT_LENGTH;j++)
	{
		yn = 0;
		for(i=0;i<BL;i++)
		{
			if(j<i)
				break;
			yn += B[i]*fft_inputbuf[2*(j-i)];
		}
		outputdata[j] = yn;
	}
	for(i=0;i<FFT_LENGTH;i++)
		fft_inputbuf[2*i] = outputdata[i];
}
void iir_task(void)
{
	static float outputdata[FFT_LENGTH];
	static float inputdata[FFT_LENGTH];
	u16 i,j;
	for(i=0;i<FFT_LENGTH;i++)
		inputdata[i] = fft_inputbuf[2*i];
	outputdata[0] = inputdata[0];
	outputdata[1] = inputdata[1];
	for(i=0;i<MWSPT_NSEC;i++)
	{
		for(j=2;j<FFT_LENGTH;j++)
			outputdata[j] = NUM[i][0]*inputdata[j] + NUM[i][1]*inputdata[j-1] + NUM[i][2]*inputdata[j-2] - DEN[i][1]*outputdata[j-1] - DEN[i][2]*outputdata[j-2];
		for(j = 0;j<FFT_LENGTH;j++)
			outputdata[j] *= NL[i];
		for(j=0;j<FFT_LENGTH;j++)
			inputdata[j] = outputdata[j];
	}
	for(i=0;i<FFT_LENGTH;i++)
		fft_inputbuf[2*i] = outputdata[i]/MWSPT_NSEC;
}
void FindIdx(void) //寻找基波谐波角标，将频谱峰值角标存储至idx
{
	u16 i,j,n,k=2;
	float temp=0;
	for(i=0;i<10;i++)
	{
		idx[i]=0;
		harmonic_f[i] = 0;
	}
	//寻找各次谐波角标
	fft_outputbuf[1]=0; //加窗导致1处有尖峰，先置零
	for(i=2;i<=2048;i++)
	{
		if(fft_outputbuf[i]/2048.f>150.f)
		{
			if(fft_outputbuf[i] > fft_outputbuf[i+1] && fft_outputbuf[i] > fft_outputbuf[i-1])
				idx[1]=i;
			else
			{
				if(fft_outputbuf[i+1] > fft_outputbuf[i-1])
					idx[1]=i+1;
				else
					idx[1]=i-1;
			}
			break;
		}
	}
	for(i=idx[1]*3;i<=2048;i*=3)//寻找谐波角标
	{
		for(j=i-k-7;j<i+k+7;j++)
		{
			if(fft_outputbuf[j]/2048.f>(50/k) && fft_outputbuf[j]>temp)
			{
				temp=fft_outputbuf[j];
				n=j;
			}
		}
		if(temp/2048.f>(50/k))
		{
			idx[k]=n;
			k++;
			temp=0;
		}
	}
}
float correct_f(u16 n,float f) //基波频率修正函数
{
	u8 i;
	float temp;
	temp=fft_outputbuf[n];
	if(1 < fft_outputbuf[n] && fft_outputbuf[n] < 4096/2-1 )
		for(i=1;i<=2;i++)
			temp+=fft_outputbuf[n+i]+fft_outputbuf[n-i];
	f+=(fft_outputbuf[n+2]-fft_outputbuf[n-2])*k2/temp + (fft_outputbuf[n+1]-fft_outputbuf[n-1])*k1/temp;
	return f;
}
float correct_f_2(u16 idx) //频率修正函数2
{
	float average = 0;
	float ave[20] = {0};
	u16 i,j,k=0;
	for(i=1;i<=20;i++)
	{
		for(j=0;j<=20;j++)
			if(fft_outputbuf[idx+j]<=fft_outputbuf[idx+i] && fft_outputbuf[idx+i] <=fft_outputbuf[idx+j+1])
			{
				ave[i-1] = (fft_outputbuf[idx+j] - fft_outputbuf[idx-i]) / (fft_outputbuf[idx+j] -fft_outputbuf[idx+j+1]) + (float)(j-i);
				k++;
				break;
			}
	}
	for(i=0;i<20;i++)
		average += ave[i];
	average /= (2*k);
	return (float)idx+average;
}
float correct_f_3(u16 idx)
{
	u16 i;
	float res = 0,power_i = 0,power = 0;
	for(i=idx-3;i<=idx+3;i++)
	{
		power += fft_powerbuf[i];
		power_i += fft_powerbuf[i]*i;
	}
	res =power_i/power;	
	return res;
}
void calculate_f(void)
{
	u8 i;
	u16 n;
	float f;
	//计算基波频率
	n = idx[1];
	f = Fs/4096.f;
	
//	harmonic_f[0] = correct_f(n,f);
//	for(i=1;i<3;i++) //计算第二次、第三次谐波频率
//	{
//		n = idx[i+1];
//		f = n*Fs/4096.0;
//		harmonic_f[i] = f;
//	}
	
//	for(i=0;i<5;i++)
//	{
//		if(idx[i+1]!=0)
//		{
//			idx[i+1] = correct_f_2(idx[i+1]);
//			harmonic_f[i] = idx[i+1] * f;
//		}
//		else
//			break;
//	}
	for(i=0;i<10;i++)
	{
		if(idx[i+1] != 0)
			harmonic_f[i] = correct_f_3(idx[i+1]) * f;
		else
			break;
	}
}
void calculate_A(void)
{
	u16 i,j;
	float Sum[5] = {0};
	for(i=1;i<4;i++)
	{
		for(j=0;j<5;j++)
			Sum[j] += pow(fft_outputbuf[idx[j+1]+i],2) + pow(fft_outputbuf[idx[j+1]-i],2);
	}
	for(i=0;i<5;i++)
	{
		Sum[i] += pow(fft_outputbuf[idx[i+1]],2);
		Sum[i] /= 7;
		Sum[i] = sqrt(Sum[i]);
	}
	for(i=0;i<5;i++)
		Am[i] = 0.0020f*Sum[i]-161.1722f;
	
//	Am1=0.0005f*Sum1+1.7937f;
//	Am2=0.0005f*Sum2-1.7937f;
}
void ShowTask(void)
{
	u16 adcx;
	u16 i;
	delay_ms(500);
	LCD_Clear(WHITE);
	LCD_ShowString(10,40,260,24,24,"Fundamental Wave:");
	adcx = harmonic_f[0];
	LCD_ShowxNum(220,40,adcx,5,24,1);
	adcx=(idx[1]*Fs-adcx)*100/4096;
	LCD_ShowString(280,43,200,16,16,".");
	LCD_ShowxNum(285,40,adcx,3,24,0x80);
	LCD_ShowString(320,40,30,24,24,"Hz");
	LCD_ShowString(350,40,30,24,24,"Am:");
	LCD_ShowxNum(380,40,Am[0],5,24,1);
	LCD_ShowString(445,40,30,24,24,"mV");
	
	LCD_ShowString(10,70,260,24,24,"Second Harmonics:");
	adcx = harmonic_f[1];
	LCD_ShowxNum(220,70,adcx,5,24,1);
	adcx=(idx[2]*Fs-adcx)*100/4096;
	LCD_ShowString(280,73,200,16,16,".");
	LCD_ShowxNum(285,70,adcx,3,24,0x80);
	LCD_ShowString(320,70,30,24,24,"Hz");
	LCD_ShowString(350,70,30,24,24,"Am:");
	LCD_ShowxNum(380,70,Am[1],5,24,1);
	LCD_ShowString(445,70,30,24,24,"mV");
	
	LCD_ShowString(10,100,260,24,24,"Third Harmonics:");
	adcx = harmonic_f[2];
	LCD_ShowxNum(220,100,adcx,5,24,1);
	adcx=(idx[3]*Fs-adcx)*100/4096;
	LCD_ShowString(280,103,200,16,16,".");
	LCD_ShowxNum(285,100,adcx,3,24,0x80);
	LCD_ShowString(320,100,30,24,24,"Hz");
	
	LCD_ShowString(10,130,260,24,24,"Forth Harmonics:");
	adcx = harmonic_f[3];
	LCD_ShowxNum(220,130,adcx,5,24,1);
	adcx=(idx[3]*Fs-adcx)*100/4096;
	LCD_ShowString(280,133,200,16,16,".");
	LCD_ShowxNum(285,130,adcx,3,24,0x80);
	LCD_ShowString(320,130,30,24,24,"Hz");
	
	LCD_ShowString(10,160,260,24,24,"Fifth Harmonics:");
	adcx = harmonic_f[4];
	LCD_ShowxNum(220,160,adcx,5,24,1);
	adcx=(idx[3]*Fs-adcx)*100/4096;
	LCD_ShowString(280,163,200,16,16,".");
	LCD_ShowxNum(285,160,adcx,3,24,0x80);
	LCD_ShowString(320,160,30,24,24,"Hz");

	LCD_ShowString(10,190,260,24,24,"Sixth Harmonics:");
	adcx = harmonic_f[5];
	LCD_ShowxNum(220,190,adcx,5,24,1);
	adcx=(idx[3]*Fs-adcx)*100/4096;
	LCD_ShowString(280,193,200,16,16,".");
	LCD_ShowxNum(285,190,adcx,3,24,0x80);
	LCD_ShowString(320,190,30,24,24,"Hz");
	
	LCD_ShowString(10,220,260,24,24,"Seventh Harmonics:");
	adcx = harmonic_f[6];
	LCD_ShowxNum(220,220,adcx,5,24,1);
	adcx=(idx[3]*Fs-adcx)*100/4096;
	LCD_ShowString(280,223,200,16,16,".");
	LCD_ShowxNum(285,220,adcx,3,24,0x80);
	LCD_ShowString(320,220,30,24,24,"Hz");
	//显示频谱
	for(i=0;i<800;i++)
		LCD_DrawLine(i+20,460,i+20,460-(fft_outputbuf[i]/15000));
	//显示波形
//	for(i=0;i<800;i++)
//		adcinput[i]=Get_Adc(adc_ch)*3300/4096;
	
}
