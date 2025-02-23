#ifndef __mq135_H
#define __mq135_H 			   
#include "sys.h"

void  Adc_Init(void);
u16 Get_Adc(u8 ch) ; 
u16 Get_Adc_Average(u8 ch,u8 times);
void get_real_value(uint16_t *value);
#endif
