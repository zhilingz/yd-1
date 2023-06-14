#include "stm32f4xx.h"
#define PGout(x)    *(unsigned int *)(PERIPH_BB_BASE+((unsigned int)(&GPIOG->ODR)-PERIPH_BASE)*32+x*4)

static GPIO_InitTypeDef GPIO_InitStructure;

void delay(void)
{
	uint32_t i=0x2000000;
	
	while(i--);
}
/*

#define GPIOF               ((GPIO_TypeDef *) GPIOF_BASE)
#define GPIOF_BASE            (AHB1PERIPH_BASE + 0x1400)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000)
#define PERIPH_BASE           ((uint32_t)0x40000000) < Peripheral base address in the alias region       
#define PERIPH_BB_BASE        ((uint32_t)0x42000000) < Peripheral base address in the bit-band region   
*/

int main(void)
{
	//使能(打开)端口G的硬件时钟，就是对端口F供电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);


	//初始化GPIO引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;		//第11根引脚
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_OUT;	//输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//推挽输出，增加输出电流能力。
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//高速响应
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//没有使能上下拉电阻

	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	
	while(1)
	{
		//PG9引脚输出高电平
		//GPIO_SetBits(GPIOG,GPIO_Pin_11);
		PGout(11)=1;
		delay();
		
		//PG9引脚输出低电平
		//GPIO_ResetBits(GPIOG,GPIO_Pin_11);
		PGout(11)=0;
		delay();	
	}



}
