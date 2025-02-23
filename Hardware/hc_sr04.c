#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "timer.h"

uint16_t Time;

void HC_Init(void)
{

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //触发测距引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //下拉
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //信号回响引脚 Echo
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
}

void HCSR04_Start(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
	delay_us(90);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	TIM_Init();
   }

uint16_t HCSR04_Get(void)
{


    HCSR04_Start();
	delay_ms(100);
	return ((Time * 0.0001) * 34000) / 2;

}

 