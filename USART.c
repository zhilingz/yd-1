#include<stdio.h>
#include<string.h>
#include"stm32f4xx.h"
#include"sys.h"
#include"delay.h"
#define USART_REC_LEN 200
#define WRITE_CMD 0xF8//写命令
#define WRITE_DAT 0xFA//写数据
#define SID PFout(15)
#define SCLK PFout(14)
#define CS PGout(1)
static	GPIO_InitTypeDef   GPIO_InitStructure;
static	USART_InitTypeDef	   USART_InitStructure;
static	NVIC_InitTypeDef   NVIC_InitStructure;

static u8 USART_RX_BUF[USART_REC_LEN];
static u16 USART_TX_EN=1;
static u16 USART_RX_STA=0;
static u8 LCD_addr[4][8]={
{0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87},
{0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97},
{0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F},
{0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F}
};
void init_LCD_GPIO()
{
	//GPIO 初始化代码
	//使能(打开)端口G的硬件时钟，就是对端口F供电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	

	//初始化GPIO引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;		//第11根引脚
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_OUT;	//输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//推挽输出，增加输出电流能力。
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//高速响应
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//没有使能上下拉电阻

	GPIO_Init(GPIOG,&GPIO_InitStructure);
	//使能(打开)端口G的硬件时钟，就是对端口F供电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);


	//初始化GPIO引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;		//第11根引脚
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_OUT;	//输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//推挽输出，增加输出电流能力。
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//高速响应
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//没有使能上下拉电阻
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	CS=1;
	SID=1;
	SCLK=1;
}
void SendByte(u8 byte)
{
	u8 i;
	for(i = 0;i < 8;i++)
	{	
		if((byte << i) & 0x80) //0x80(1000 0000)
	{
		SID = 1;
	} else
	{
		SID = 0;
	} 
	SCLK = 0;
	delay_us(5);
	SCLK = 1;
	}
} 
void Lcd_WriteCmd(u8 Cmd )
{
	delay_ms(1);
	SendByte(WRITE_CMD); //11111,RW(0),RS(0),0
	SendByte(0xf0&Cmd);
	SendByte(Cmd<<4);
} 
void Lcd_WriteData(u8 Dat )
{
	delay_ms(1);
	SendByte(WRITE_DAT);
	SendByte(0xf0&Dat);
	SendByte(Dat<<4);
}
void init_LCD(void)
{
	delay_ms(50);
	Lcd_WriteCmd(0x30);
	delay_ms(1);
	Lcd_WriteCmd(0x30);
	delay_ms(1);
	Lcd_WriteCmd(0x0c);
	delay_ms(1);
	Lcd_WriteCmd(0x01);
	delay_ms(30);
	Lcd_WriteCmd(0x06);
} 
void LCD_Display_Words(uint8_t x,uint8_t y,uint8_t*str)
{
	Lcd_WriteCmd(LCD_addr[x][y]);
	while(*str>0)
	{
	Lcd_WriteData(*str);
	str++;
	}
}
void LCD_Clear(void)
{
	Lcd_WriteCmd(0x01);
	delay_ms(2);
}
void init_USART1(unsigned int bound)
{
	//打开USART1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	//打开GPIOA时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//GPIOA初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIOA端口复用
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	//USART初始化
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,  &USART_InitStructure);
	//中断方式，接收数据时产生中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//中断设置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn   ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	//开启USART1
	USART_Cmd(USART1, ENABLE);
}

//中断处理函数
void USART1_IRQHandler(void)
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		Res =USART_ReceiveData(USART1);
		
		//补充代码
		//对应操作 USART_RX_STA++;
		USART_RX_BUF[USART_RX_STA++]=Res;
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}	
int main(void)
{
	char USART_SendBuf[]="Hello!";
	int i;
	int j=0;
	int Len=strlen(USART_SendBuf);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_ms(168);
	init_LCD_GPIO();
	init_LCD();
	init_USART1(115200);
	delay_ms(168);
	while(1)
	{
		delay_ms(2000);
		if(USART_TX_EN)
		{
			for(i=0;i<Len;i++)
			{
				USART_SendData(USART1, USART_SendBuf[i]);
				while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)!=SET);
			} 
			USART_TX_EN=0;
		}
		if(USART_RX_STA==Len)
		{
			USART_TX_EN=1;
			USART_RX_STA=0;
			LCD_Display_Words(j,0,USART_RX_BUF);
			j++;
			if(j==4)
			{
				j=0;
				delay_ms(200);
				LCD_Clear();
			}
			
		}
		
	}

}
