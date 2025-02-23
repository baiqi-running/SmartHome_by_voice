#include "stm32f10x.h"                  // Device header


extern uint16_t Time;

void TIM_Init(void)
{
    Time = 0;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    
    TIM_InternalClockConfig(TIM2);		//TIM2使用内部时钟
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_Period=7200-1;
    TIM_TimeBaseInitStructure.TIM_Prescaler=0;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update|TIM_IT_Trigger, ENABLE);		//使能中断


    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitTypeDef    NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
    
    NVIC_Init(&NVIC_InitStructure);
    TIM_Cmd(TIM2, ENABLE);		//打开定时器


}



void TIM2_IRQHandler(void)
{

if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{

		if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 1)
		{
		 Time ++;
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	
	}
}
