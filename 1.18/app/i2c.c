/*
  程序说明: CT117E嵌入式竞赛板GPIO模拟I2C总线驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT117E嵌入式竞赛板
  日    期: 2011-8-9
*/

#include "stm32f10x.h"

/** I2C 总线接口 */
#define I2C_PORT GPIOB
#define SDA_Pin	GPIO_Pin_7
#define SCL_Pin GPIO_Pin_6

#define FAILURE 0
#define SUCCESS 1

//配置SDA信号线为输入模式
void SDA_Input_Mode()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = SDA_Pin;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 

  	GPIO_Init(I2C_PORT, &GPIO_InitStructure);
}

//配置SDA信号线为输出模式
void SDA_Output_Mode()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = SDA_Pin;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

  	GPIO_Init(I2C_PORT, &GPIO_InitStructure);
}

//
void SDA_Output( uint16_t val )
{
	if ( val ) {
		GPIO_SetBits(I2C_PORT,SDA_Pin);
	} else {
		GPIO_ResetBits(I2C_PORT,SDA_Pin);
	}
}

//
void SCL_Output( uint16_t val )
{
	if ( val ) {
		GPIO_SetBits(I2C_PORT,SCL_Pin);
	} else {
		GPIO_ResetBits(I2C_PORT,SCL_Pin);
	}
}

//
uint8_t SDA_Input()
{
	return GPIO_ReadInputDataBit( I2C_PORT, SDA_Pin);
}

//延时程序
void delay1(unsigned int n)
{
	unsigned int i;
	for ( i=0;i<n;++i);
}

//I2C总线启动
void I2CStart(void)
{
	SDA_Output(1);delay1(500);
	SCL_Output(1);delay1(500);
	SDA_Output(0);delay1(500);
	SCL_Output(0);delay1(500);
}

//I2C总线停止
void I2CStop(void)
{
	SCL_Output(0); delay1(500);
	SDA_Output(0); delay1(500);
	SCL_Output(1); delay1(500);
	SDA_Output(1); delay1(500);

}

//等待应答
unsigned char I2CWaitAck(void)
{
	unsigned short cErrTime = 5;
	SDA_Input_Mode(); 
	delay1(500);
	SCL_Output(1);delay1(500);
	while(SDA_Input())
	{
		cErrTime--;
		delay1(500);
		if (0 == cErrTime)
		{
			SDA_Output_Mode();
			I2CStop();
			return FAILURE;
		}
	}
	SDA_Output_Mode();
	SCL_Output(0);delay1(500); 
	return SUCCESS;
}

//发送应答位
void I2CSendAck(void)
{
	SDA_Output(0);delay1(500);
	delay1(500);
	SCL_Output(1); delay1(500);
	SCL_Output(0); delay1(500);

}

//
void I2CSendNotAck(void)
{
	SDA_Output(1);
	delay1(500);
	SCL_Output(1); delay1(500);
	SCL_Output(0); delay1(500);

}

//通过I2C总线发送一个字节数据
void I2CSendByte(unsigned char cSendByte)
{
	unsigned char  i = 8;
	while (i--)
	{
		SCL_Output(0);delay1(500); 
		SDA_Output(cSendByte & 0x80); delay1(500);
		cSendByte += cSendByte;
		delay1(500); 
		SCL_Output(1);delay1(500); 
	}
	SCL_Output(0);delay1(500); 
}

//从I2C总线接收一个字节数据
unsigned char I2CReceiveByte(void)
{
	unsigned char i = 8;
	unsigned char cR_Byte = 0;
	SDA_Input_Mode(); 
	while (i--)
	{
		cR_Byte += cR_Byte;
		SCL_Output(0);delay1(500); 
		delay1(500); 
		SCL_Output(1);delay1(500); 
		cR_Byte |=  SDA_Input(); 
	}
	SCL_Output(0);delay1(500); 
	SDA_Output_Mode();
	return cR_Byte;
}

//I2C总线初始化
void i2c_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SDA_Pin | SCL_Pin;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 // **

  	GPIO_Init(I2C_PORT, &GPIO_InitStructure);

}

extern u8 c8563_Store[6];
void writeData(u8 address,u8 mdata)
{
		I2CStart();
   		I2CSendByte(0xa2); /*写命令*/
	I2CWaitAck();
		I2CSendByte(address); /*写地址*/
	I2CWaitAck();
	    I2CSendByte(mdata); /*写数据*/
	I2CWaitAck();
		I2CStop();
}

void ReadData1(u8 address,u8 count,u8 * buff) /*多字节*/
{  
		u8 i;
   		I2CStart();
   		I2CSendByte(0xa2); /*写命令*/
	I2CWaitAck();
	    I2CSendByte(address); /*写地址*/
	I2CWaitAck(); 
	    I2CStart();
	    I2CSendByte(0xa3); /*读命令*/
	I2CWaitAck();
	    for(i=0;i<count;i++)
	    {
	    		buff[i]=I2CReceiveByte();
	       		if(i<count-1) 
						I2CSendAck();
	    }
	    I2CSendNotAck();
        I2CStop();
}  

u8 g8563_Store[7];
void P8563_Read()
{   
		u8 time[7];
    	ReadData1(0x02,0x07,time);
	    g8563_Store[0]=time[0]&0x7f; /*秒 */
	    g8563_Store[1]=time[1]&0x7f; /*分 */
	    g8563_Store[2]=time[2]&0x3f; /*小时 */
		g8563_Store[3]=time[3]&0x3f; /*日 */
	    g8563_Store[4]=time[5]&0x1f; /*月 */
		g8563_Store[5]=time[6]; /*年  */
	
}

void P8563_gettime()
{
    	P8563_Read();
    	if(g8563_Store[0]==0)
      			P8563_Read(); /*如果为秒=0，为防止时间变化，再读一次*/
}	

void P8563_settime()
{
	    //uchar i;
	    writeData(8,g8563_Store[0]); //年 
	 	writeData(7,g8563_Store[1]); //月 
		writeData(5,g8563_Store[2]); //日 
		writeData(4,g8563_Store[3]); //时 
	   	writeData(3,g8563_Store[4]); //分  
		writeData(2,g8563_Store[5]); //秒 
}

uint8_t x24c02_read(uint8_t address)
{
	unsigned char val;
	
	I2CStart(); 
	I2CSendByte(0xa2);
	I2CWaitAck(); 
	
	I2CSendByte(address);
	I2CWaitAck(); 
	
	I2CStart();
	I2CSendByte(0xa3); 
	I2CWaitAck();
	val = I2CReceiveByte(); 
	I2CWaitAck();
	I2CStop();
	
	return(val);
}

void p8563_init(void)
{
	u8 i;
	
	for(i=0;i<=5;i++) g8563_Store[i]=c8563_Store[i]; /*初始化时间*/
        P8563_settime();
}
