#include "usart.h"
#include "string.h"
#include "esp.h"
//USART1�ĳ�ʼ������

//USART1_TX:PA9����Ϊ���츴�����
//USART1_RX:PA10���ø�������
char RECS[500];
char RECSS[500];
unsigned char i;
unsigned char j;
u16 USART_RX_STA=0;       //����״̬���	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�

void USART1_Init(u32 baud)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
  GPIO_InitTypeDef         GPIO_InitStructure;
  //PA10-USART1_RX
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;    // ��������
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  //PA9-USART1_TX
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;          // �����������         
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9; //����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
 
  USART_InitTypeDef        USART_InitStructure;

  USART_InitStructure.USART_BaudRate            = baud;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  // ��Ӳ��������
  USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
  USART_InitStructure.USART_Parity              = USART_Parity_No;                 // ��У��λ
  USART_InitStructure.USART_StopBits            = USART_StopBits_1;
  USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
  USART_Init(USART1, &USART_InitStructure);
  
    NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel =USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE); 
  USART_Cmd(USART1, ENABLE);  // ��������
}

char* MyUSART_GetString(void)
{
	return RECS;
}

void MyUSART_SendString(char* str)
{
	uint8_t stri=0;
	while(str[stri] != '\0')
		USART_SendData (USART1,str[stri++]);
}

void USART1_IRQHandler()
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE))
	{
		RECS[i++]=USART_ReceiveData(USART1);
		if((RECS[i-2]=='\r')|(RECS[i-1]=='\n'))  
		{
			RECS[i-2]='\0';
			i = 0;
			CommandAnalyse();
            //CommandAnalyse2();
		}
	}
}

//void USART1_IRQHandler(void)                	//����1�жϷ������  ����
//{   if(USART_GetITStatus(USART1,USART_IT_RXNE))
//	{
//		RECS[i++]=USART_ReceiveData(USART1);
//		if((RECS[i-2]=='\r')|(RECS[i-1]=='\n'))  
//		{
//			RECS[i-2]='\0';
//			i = 0;
//			CommandAnalyse();
//		}
//		u8 Res;
//		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a �س����н�β)
//					Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
//					
//					if((USART_RX_STA&0x8000)==0)//����δ���
//					{
//									if(USART_RX_STA&0x4000)//���յ���0x0d
//									{
//											if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
//											else USART_RX_STA|=0x8000;	//��������� 
//									}
//									else //��û�յ�0X0D
//									{	
//											if(Res==0x0d)USART_RX_STA|=0x4000;
//											else
//											{
//													RECS[USART_RX_STA&0X3FFF]=Res ;
//													USART_RX_STA++;
//													if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
//											}		 
//									}
//					}   		 
//     } 	
//	
//} 




//����һ���ַ�
void Uart1_Send_Byte(u8 data)
{
  /*���͵������ݵ�USART1*/
  USART_SendData (USART1, data);                             
                               /*�������ݼĴ���Ϊ�ձ�־*/
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);               
}

//����һ���ַ�
u8 Uart1_Receive_Byte(void)
{
  u8 data;
  /*�ȴ����ܼĴ������ݷǿ�*/
  while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
 
  /*���յ�������*/
  data = USART_ReceiveData(USART1);
  return data;
}




//�����ַ���
void Uart1_SendString(char DataStr[])
{
  int i = 0;
 
  while((DataStr[i]) != '\0')              // �ַ���������
  {
    Uart1_Send_Byte(DataStr[i]);               
    i++;
  }
}




//�����ַ���
u8 Uart1_RecvString(char *DataStr)
{
  char data;
 
  do
  {
    data = Uart1_Receive_Byte();
    *DataStr = data;
    DataStr++;
  }   while(data != '\n');   // �����̵Ļس���'\r'����֮���ַ����������
  
  *DataStr = '\0';
  return 0;
}





//����������仰���Ϳ���ʹ��printf������
#pragma import(__use_no_semihosting_swi) //ȡ��������״̬

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;

int fputc(int ch, FILE *f) 
{
 //�滻Ϊ�������ض���ķ��͵��ֽں���
 Uart1_Send_Byte(ch);
  return (ch);
}

//int fputc_usart3(int ch,FILE *f )   //printf�ض���  
//{
//	USART_SendData(USART3,(uint8_t)ch);
//	while(USART_GetFlagStatus (USART3,USART_FLAG_TC) == RESET);
//	return ch;
//}

int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}


void _ttywrch(int ch) {
  //�滻Ϊ�������ض���ķ��͵��ֽں���
 Uart1_Send_Byte(ch);
}


void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */

}







/*------------------------------------------------------------------------------*/

//USART3�ĳ�ʼ������
//USART3_TX:PB10����Ϊ���츴�����
//USART3_RX:PB11���ø�������
void USART3_Init(u32 baud)
{
	//1.����GPIOBʱ��,USART3ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	//2.PB10��PB11��ʼ��
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING ;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//3.USART3��ʼ��
	USART_InitTypeDef USART_InitStruct;
	
	USART_InitStruct.USART_BaudRate            = baud;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode                = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_Parity              = USART_Parity_No;
	USART_InitStruct.USART_StopBits            = USART_StopBits_1;
	USART_InitStruct.USART_WordLength          = USART_WordLength_8b;
	
	USART_Init(USART3,&USART_InitStruct);
	
	//4.ʹ��USART3�Ŀ����жϺͽ����ж�
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);
	
	//5.USART3�ж�ͨ������
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel                   = USART3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 1;
	
	NVIC_Init(&NVIC_InitStruct);
	
	//6.ʹ��USART3
	USART_Cmd(USART3,ENABLE);
}

//USART3�����ַ�������
void USART3_SendStr(char *buff)
{
  while(*buff!='\0')
	{
		while(!USART_GetFlagStatus(USART3,USART_FLAG_TXE));
		USART_SendData(USART3,*buff);
		buff++;
	}
}

//void Uart3_Send_Byte(u8 data)
//{
//  /*���͵������ݵ�USART1*/
//  USART_SendData (USART3, data);                             
//                               /*�������ݼĴ���Ϊ�ձ�־*/
//  while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);               
//}


//WIFI_DEV WiFi_Rev={0};
//void USART3_IRQHandler(void)
//{
//u8 data=0;
//if(USART_GetITStatus(USART3,USART_IT_RXNE))
//{
//data=USART_ReceiveData(USART3);
//WiFi_Rev.Rev_Buff[WiFi_Rev.Rev_Cnt++]=data;
//}
//if(USART_GetITStatus(USART3,USART_IT_IDLE))
//{
//USART3->SR;
//USART3->DR;
//WiFi_Rev.Rev_Buff[WiFi_Rev.Rev_Cnt]='\0';
//printf("%s\r\n",WiFi_Rev.Rev_Buff);
//WiFi_Rev.Rev_Cnt=0;
//WiFi_Rev.Rev_Flag=1;
//}
//}
void USART3_IRQHandler(void)
{
if(USART_GetITStatus(USART3,USART_IT_RXNE))
	{ 
		RECSS[j++]=USART_ReceiveData(USART3);
		if((RECSS[j-2]=='\r')|(RECSS[j-1]=='\n'))  
	     {
			RECSS[j-2]='\0';
			j = 0;
            Uart1_SendString(RECSS);
			CommandAnalyse2();
		}
	}

}
