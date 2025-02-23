#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //引脚重映射

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM4);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;   //不分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   //向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 200 - 1;		    //ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;    //重复计数器
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //使能
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR占空比
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);//通道二
    //TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //通道重载
	TIM_Cmd(TIM4, ENABLE);
}

//void PWM_SetCompare2(uint16_t Compare)
//{
//	TIM_SetCompare2(TIM4, Compare);
//}


void PWM_Speed(int n)
{   if(n==0)
		TIM_SetCompare3(TIM4,0);
	if(n==1)
		TIM_SetCompare3(TIM4,60);
	if(n==2)
		TIM_SetCompare3(TIM4,120);
	if(n==3)
		TIM_SetCompare3(TIM4,200);
    if(n==4)
        TIM_SetCompare4(TIM4, 195);//0度
    if(n==5)
        TIM_SetCompare4(TIM4, 175);//180度
}
