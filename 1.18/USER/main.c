/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * 文件名  ：main.c
 * 描述    ：用3.5.0版本建的工程模板。         
 * 实验平台：野火STM32开发板
 * 库版本  ：ST3.5.0
 *
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/
#include "stm32f10x.h"
#include "i2c.h"
#include "delay.h"
#include "usart.h"
#include "sys.h"
#include "led.h"
#include "pwm.h"
#include "TIM.h"
#include <stdlib.h>

void q1(u8 x);
void q2(u8 x);
void q3(u8 x);
void q4(u8 x);
extern u8 g8563_Store[7];
u8 c8563_Store[6]={0x18,0x02,0x18,0x19,0x48,0x45},ready=0;
u8 ran,ran1,r1,r2,r3,r4,last_hour,last_min;
u8 USART_RX_BUF[200];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	 

extern volatile u32 time;

#define ne PBout(9)


int main(void)
{
	u8 x=0;
	u16 i=0;
	u32 j=0,k=0;
	i2c_init();
	uart_init(9600);
	delay_init();
	LED_GPIO_Config();
	TIM1_GPIO_Config();
	TIM_Configuration();
	TIM2_Configuration();
	START_TIME;
	
////	p8563_init();
	
	ne = 0;
	
	P8563_gettime();
	
	r1=r2=r3=r4=1;
	PBout(8) = 1;

		delay_ms(1000);
		r1=0;
		delay_ms(1000);
		r2=0;
		delay_ms(1000);
		r3=0;
		delay_ms(1000);
		r4=0;
	
	last_hour = g8563_Store[2];
	last_min  = g8563_Store[1];
  	  while(1)
			{
				
				delay_ms(1);
				i++;
				if(i>=1000)
				{
					i = 0;
					r1=r2=r3=r4=0;
					P8563_gettime();
					
					if(g8563_Store[2] == 0x07 && g8563_Store[1] == 0x12){g8563_Store[2] = 0x05;g8563_Store[1] = 0x20;}
					if(g8563_Store[2] == 0x05 && g8563_Store[1] == 0x20){g8563_Store[2] = 0x07;g8563_Store[1] = 0x12;}
					
					if(last_hour != g8563_Store[2])
					{
						if(g8563_Store[2]%16 - last_hour%16 != 1)r1 = r2 = 1;
						else r2 = 1;
					}
					if(last_min != g8563_Store[1])
					{
						if(g8563_Store[1]%16  - last_min%16 != 1)r3 = r4 = 1;
						else r4 = 1;
					}
					
					last_hour = g8563_Store[2];
					last_min  = g8563_Store[1];
					printf("20%x年",g8563_Store[5]);
					printf("%x月",g8563_Store[4]);
					printf("%x日",g8563_Store[3]);
					printf("%x时",g8563_Store[2]);
					printf("%x分",g8563_Store[1]);
					printf("%x秒\r\n",g8563_Store[0]);
				
					PBout(8) = 1; //170v switch
					ne = ~ne;			//neon lamp 
					
					if((USART_RX_STA&0x00ff) == 14)
					{
//						c8563_Store[6]={0x18,0x02,0x18,0x19,0x48,0x45};
						c8563_Store[0] = (USART_RX_BUF[2]-0x30)*0x10+USART_RX_BUF[3]-0x30;
						c8563_Store[1] = (USART_RX_BUF[4]-0x30)*0x10+USART_RX_BUF[5]-0x30;
						c8563_Store[2] = (USART_RX_BUF[6]-'0')*0x10+USART_RX_BUF[7]-0x30;
						c8563_Store[3] = (USART_RX_BUF[8]-'0')*0x10+USART_RX_BUF[9]-0x30;
						c8563_Store[4] = (USART_RX_BUF[10]-'0')*0x10+USART_RX_BUF[11]-0x30;
						c8563_Store[5] = (USART_RX_BUF[12]-'0')*0x10+USART_RX_BUF[13]-0x30;
						p8563_init();
						printf("TIME SET OK！\r\n");
					}
					USART_RX_STA = 0;
				}
				
//			if(i>5000){k=0;j+=1;}
				k++;
				if(k>1){j++;k=0;}
			if(j>1900)j=0;
			if(j>950)		
			TIM1->CCR1 = 1900-j;
			else TIM1->CCR1 = j;
				
			}
	
}


 void TIM2_IRQHandler(void)
{
	
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
	{	
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);    
  		 time++;
			 ran = rand()%10;
		ran1 = rand()%3;
		
		
		 if(time == 50 && ready == 0)
		 {
		   time=0;	
			 if(r4)q4(ran);		else q4(g8563_Store[1]%16);
			 if(r3)q3(ran+1);	else q3(g8563_Store[1]/16);
			 if(r2)q2(ran-1);	else q2(g8563_Store[2]%16);
			 if(r1)q1(ran1);	else q1(g8563_Store[2]/16);
		}
	}		 	
}


void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a){USART_RX_STA=0;printf("错误数据！");}//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1)){USART_RX_STA=0;printf("错误数据！");}//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 

} 
	
void q4(u8 x)
{
	PBout(03) = (x &0x01);
	PAout(11) = (x &0x02)>>1;
	PAout(12) = (x &0x04)>>2;
	PAout(15) = (x &0x08)>>3;	
}
void q3(u8 x)
{
	PAout(00) = (x &0x01);
	PAout(01) = (x &0x02)>>1;
	PAout(02) = (x &0x04)>>2;
	PAout(03) = (x &0x08)>>3;	
}
void q2(u8 x)
{
	PAout(04) = (x &0x01);
	PAout(07) = (x &0x02)>>1;
	PAout(06) = (x &0x04)>>2;
	PAout(05) = (x &0x08)>>3;	
}

void q1(u8 x)
{
	if(x==0){PBout(13) = 1;PBout(14) = 0;PBout(15) = 0;}
	if(x==1){PBout(13) = 0;PBout(14) = 1;PBout(15) = 0;}
	if(x==2){PBout(13) = 0;PBout(14) = 0;PBout(15) = 1;}
}

