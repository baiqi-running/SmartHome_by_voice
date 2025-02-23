#ifndef __USART_H_
#define __USART_H_

#include "stm32f10x.h"
#include "stdio.h"

typedef struct{
	char Rev_Buff[1000];
	u16 Rev_Cnt;
  u8 Rev_Flag;
}WIFI_DEV;

extern WIFI_DEV WiFi_Rev;
#define USART_REC_LEN 500
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;   
//extern char RECS[500];

void USART1_Init(u32 baud);
char* MyUSART_GetString(void);
void MyUSART_SendString(char* str);
void USART1_SendByte(u8 data);
//u8 USART1_RecvByte(void);

void USART3_Init(u32 baud);
void USART3_SendStr(char *buff);
//void Uart3_Send_Byte(u8 data);
void USART3_IRQHandler(void);
#endif
