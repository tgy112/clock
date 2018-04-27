/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * �ļ���  ��main.c
 * ����    ����3.5.0�汾���Ĺ���ģ�塣         
 * ʵ��ƽ̨��Ұ��STM32������
 * ��汾  ��ST3.5.0
 *
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
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
u8 USART_RX_BUF[200];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	 

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
					printf("20%x��",g8563_Store[5]);
					printf("%x��",g8563_Store[4]);
					printf("%x��",g8563_Store[3]);
					printf("%xʱ",g8563_Store[2]);
					printf("%x��",g8563_Store[1]);
					printf("%x��\r\n",g8563_Store[0]);
				
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
						printf("TIME SET OK��\r\n");
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


void USART1_IRQHandler(void)                	//����1�жϷ������
	{
	u8 Res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		
		if((USART_RX_STA&0x8000)==0)//����δ���
			{
			if(USART_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a){USART_RX_STA=0;printf("�������ݣ�");}//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1)){USART_RX_STA=0;printf("�������ݣ�");}//�������ݴ���,���¿�ʼ����	  
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

