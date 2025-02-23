#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "DHT11.h"
				

void DHT11_GPIO_Config(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOA, &GPIO_InitStructure);		  

	GPIO_SetBits(GPIOA, GPIO_Pin_11);	 
}

void DHT11_Mode_IPU(void)
{
 	  GPIO_InitTypeDef GPIO_InitStructure;
	  GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_11;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 
	  GPIO_Init(GPIOA, &GPIO_InitStructure);	 
}

void DHT11_Mode_Out_PP(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);	 	 
}

uint8_t Read_Byte(void)   //读取数据 0  1
{
	int i, Tempe=0;

	for (i=0; i<8; i++)    
	{	 
		while ( GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11) == Bit_RESET); //50us 低电平 结束
		
        delay_us(40);  //通过检测 x us后的电平即可区别这两个状 ，x 大于大于数据0持续的时间即可
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)== Bit_SET) //x us后为高电平 1
		{
	        while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)== Bit_SET); //等待数据1的高电平结束

			Tempe |= (int)(0x01 << (7 - i));  //把第（7-i）位置 1
		}
		else  // //x us后为低电平 0
		{			   
			Tempe &= (int) ~ (0x01<<(7-i)); 
		}
	}
	return (int)Tempe;
}

void dht_Readdata(uint8_t *Shidu,uint8_t *Temperature){
    int DATA[5]={0,0,0,0,0};
	DHT11_Mode_Out_PP();
    GPIO_ResetBits(GPIOA, GPIO_Pin_11);
	delay_ms(18);

	GPIO_SetBits(GPIOA, GPIO_Pin_11); 
	delay_us(30);

	DHT11_Mode_IPU();

	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)== Bit_RESET)     
	{
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11) == Bit_RESET);
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11) == Bit_SET);

        for(int i=0;i<5;i++){
        DATA[i]=Read_Byte();
  }
        DHT11_Mode_Out_PP(); //读取结束，引脚改为输出模式
		GPIO_SetBits(GPIOA, GPIO_Pin_11);  //高电平
        delay_us(100);

    if(( DATA[0]+ DATA[1]+ DATA[2]+ DATA[3])== DATA[4]){
     *Shidu=DATA[0];
     *Temperature=DATA[2];
  }

	
}
} 	
