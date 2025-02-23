#include "stm32f10x.h"                  // Device header
#include "delay.h"  

void  beep_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOC,GPIO_Pin_0);	 
}


void beep_sys(void)
{
 for(int i=0;i<=5;i++){
GPIO_SetBits(GPIOC,GPIO_Pin_0);
delay_ms(500);
GPIO_ResetBits(GPIOC,GPIO_Pin_0);
delay_ms(500);
}
}
