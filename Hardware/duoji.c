#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void duoji_Init(void)
{
	PWM_Init();
    PWM_Speed(4);
}

