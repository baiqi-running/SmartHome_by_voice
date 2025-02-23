#include "usart.h"
#include "string.h"
#include "esp.h"
//USART1的初始化函数

//USART1_TX:PA9配置为推挽复用输出
//USART1_RX:PA10配置浮空输入
char RECS[500];
char RECSS[500];
unsigned char i;
unsigned char j;
u16 USART_RX_STA=0;       //接收状态标记	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节

void USART1_Init(u32 baud)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
  GPIO_InitTypeDef         GPIO_InitStructure;
  //PA10-USART1_RX
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;    // 浮空输入
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  //PA9-USART1_TX
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;          // 复用推挽输出         
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9; //发送
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
 
  USART_InitTypeDef        USART_InitStructure;

  USART_InitStructure.USART_BaudRate            = baud;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  // 无硬件控制流
  USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
  USART_InitStructure.USART_Parity              = USART_Parity_No;                 // 无校验位
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
  USART_Cmd(USART1, ENABLE);  // 开启串口
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

//void USART1_IRQHandler(void)                	//串口1中断服务程序  接收
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
//		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a 回车换行结尾)
//					Res =USART_ReceiveData(USART1);	//读取接收到的数据
//					
//					if((USART_RX_STA&0x8000)==0)//接收未完成
//					{
//									if(USART_RX_STA&0x4000)//接收到了0x0d
//									{
//											if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
//											else USART_RX_STA|=0x8000;	//接收完成了 
//									}
//									else //还没收到0X0D
//									{	
//											if(Res==0x0d)USART_RX_STA|=0x4000;
//											else
//											{
//													RECS[USART_RX_STA&0X3FFF]=Res ;
//													USART_RX_STA++;
//													if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
//											}		 
//									}
//					}   		 
//     } 	
//	
//} 




//发送一个字符
void Uart1_Send_Byte(u8 data)
{
  /*发送单个数据到USART1*/
  USART_SendData (USART1, data);                             
                               /*发送数据寄存器为空标志*/
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);               
}

//接收一个字符
u8 Uart1_Receive_Byte(void)
{
  u8 data;
  /*等待接受寄存器数据非空*/
  while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
 
  /*接收单个数据*/
  data = USART_ReceiveData(USART1);
  return data;
}




//发送字符串
void Uart1_SendString(char DataStr[])
{
  int i = 0;
 
  while((DataStr[i]) != '\0')              // 字符串结束符
  {
    Uart1_Send_Byte(DataStr[i]);               
    i++;
  }
}




//接收字符串
u8 Uart1_RecvString(char *DataStr)
{
  char data;
 
  do
  {
    data = Uart1_Receive_Byte();
    *DataStr = data;
    DataStr++;
  }   while(data != '\n');   // 当键盘的回车键'\r'按下之后，字符串输入完毕
  
  *DataStr = '\0';
  return 0;
}





//加上下面这句话，就可以使用printf函数了
#pragma import(__use_no_semihosting_swi) //取消半主机状态

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;

int fputc(int ch, FILE *f) 
{
 //替换为开发者重定向的发送单字节函数
 Uart1_Send_Byte(ch);
  return (ch);
}

//int fputc_usart3(int ch,FILE *f )   //printf重定向  
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
  //替换为开发者重定向的发送单字节函数
 Uart1_Send_Byte(ch);
}


void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */

}







/*------------------------------------------------------------------------------*/

//USART3的初始化函数
//USART3_TX:PB10配置为推挽复用输出
//USART3_RX:PB11配置浮空输入
void USART3_Init(u32 baud)
{
	//1.开启GPIOB时钟,USART3时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	//2.PB10和PB11初始化
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING ;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//3.USART3初始化
	USART_InitTypeDef USART_InitStruct;
	
	USART_InitStruct.USART_BaudRate            = baud;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode                = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_Parity              = USART_Parity_No;
	USART_InitStruct.USART_StopBits            = USART_StopBits_1;
	USART_InitStruct.USART_WordLength          = USART_WordLength_8b;
	
	USART_Init(USART3,&USART_InitStruct);
	
	//4.使能USART3的空闲中断和接收中断
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);
	
	//5.USART3中断通道配置
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel                   = USART3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 1;
	
	NVIC_Init(&NVIC_InitStruct);
	
	//6.使能USART3
	USART_Cmd(USART3,ENABLE);
}

//USART3发送字符串函数
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
//  /*发送单个数据到USART1*/
//  USART_SendData (USART3, data);                             
//                               /*发送数据寄存器为空标志*/
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
