#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "beep.h"
#include "PWM.h"
#include "dht11.h"
#include "lcd.h"
#include "usart.h"
#include "esp.h"
#include "hc_sr04.h"
#include "timer.h"
#include "key.h"
#include "light.h"
#include "led.h"
#include "mq135.h"
#include "bh1750.h"

u8 Temperature,Shidu,konqi;
uint16_t T;
uint8_t Switch1=0;
uint8_t Switch2=0;
uint8_t lightnum;
uint8_t chuangnum=0;
uint8_t powernum=0;
extern char RECS[500];
extern char RECSS[500];
extern uint8_t fengnum;
extern uint8_t lightnumm;
extern uint8_t chuangnumm;
extern u8 lijianum;
extern u8 tiannum;
extern u8 lijianumm;
extern u8 jianum;
uint16_t value = 0;
uint8_t guangnum;
uint8_t mennum=0;

void Init(void)
{
	uint8_t Judge=0;
	LCD_Init();
    LCD_ShowString(10,40,200,16,16,"running...");
	USART1_Init(115200); //初始化串口
    beep_Init();

	do
	{
		Judge = esp_Init();
        LCD_ShowString(10,60,200,16,16,"error:");
        LCD_ShowNum(100,60,Judge,2,16);
	}while(Judge);

    GPIO_SetBits(GPIOC,GPIO_Pin_0);
    delay_ms(1000);
    GPIO_ResetBits(GPIOC,GPIO_Pin_0);
}


int main()
{
   Init();
   USART1_Init(115200);
   USART3_Init(115200);
    DHT11_GPIO_Config();
    beep_Init();
    PWM_Init();
    LCD_Init();
    esp_Init();
    Light_Init();
    Key_Init();
    LED_Init();
    Adc_Init();
    BH1750_Init();
    HC_Init();

    LCD_ShowString(10,130,200,16,16,"open:");
    LCD_ShowString(10,150,200,16,16,"Temperature:");
    LCD_ShowString(10,170,200,16,16,"Shidu:");
    LCD_ShowString(10,190,200,16,16,"S:");
    LCD_ShowString(10,230,200,16,16,"kongqi:");
    LCD_ShowString(10,250,200,16,16,"guang:");
    LCD_ShowString(10,210,200,16,16,"lightnum:");
	
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		

while(1){


if (!i2c_CheckDevice(BH1750_Addr))
		{
        guangnum=(int)(LIght_Intensity());             //获取光照强度
		}
   dht_Readdata(&Shidu,&Temperature);
   get_real_value(&value);
//    printf("燃气值=%d\r\n",value);

    T = HCSR04_Get();
    LCD_ShowNum(80,190,T,2,16);
    LCD_ShowNum(110,150,Temperature,2,16);
    LCD_ShowNum(80,170,Shidu,2,16);
    LCD_ShowNum(60,230,value,5,16);
    LCD_ShowNum(60,250,guangnum,5,16);

//    printf("%s\r\n",RECS);
//    MyUSART_SendString(RECSS);
     

     if(value>=400){
      PWM_Speed(3);
      GPIO_SetBits(GPIOC,GPIO_Pin_0);
      delay_ms(5000);
      GPIO_ResetBits(GPIOC,GPIO_Pin_0);
      powernum=100;
      Switch1=1;
  } 

   else if(value<30){
      PWM_Speed(0);
      powernum=0;
      Switch1=0;
  } 


    if(Temperature>=24)
  {
    PWM_Speed(1);
    Switch1=1;
    powernum=30;
    GPIO_ResetBits(GPIOC,GPIO_Pin_0);
  }
    if(Temperature>=26){
    PWM_Speed(2);
    Switch1=1;
    powernum=60;
    GPIO_ResetBits(GPIOC,GPIO_Pin_0);
  }
    if(Temperature>=27||fengnum=='1'){
    PWM_Speed(3);
    Switch1=1;
    powernum=100;

  }
   if(Temperature>=27&&lijianum!='0'){
    GPIO_SetBits(GPIOC,GPIO_Pin_0);
    delay_ms(5000);
    GPIO_ResetBits(GPIOC,GPIO_Pin_0);
  }


    if(T<=25){
        Switch2=1;
    }
    if(T>25 && lijianum!='1'&& jianum!='1'){
    Switch2=0;
    }

    if(jianum=='1'){
    mennum=1;
    }
    if(jianum=='1'&&T<=25){
    jianum='0';
   }
    if(jianum=='0'){
    mennum=0;
    }

if ((Light_Get() == 1 && T<=25)&&lightnumm!='0')
		{
		LED1_ON();
//      lightnum=1;    
}

if (Light_Get() == 1 && chuangnumm!='1'&&lijianum!='1')
    {
       PWM_Speed(4);
        chuangnum=0;
    }
		
if (Light_Get() == 0 &&chuangnumm!='0'&& guangnum>60)
		{
//      lightnum=0;
        PWM_Speed(5);
        chuangnum=1;
		}

if (Light_Get() == 0 &&lightnumm!='1')
		{
		LED1_OFF();
}


    if(lijianumm==1&&T<=25)
		{
        Switch2=1;
        beep_sys();  
}

//     if(guangnum>=120 && (tiannum == 2||lijianum=='1')){
//    USART3_SendStr("tian1");
//    }
//     if(guangnum>=60 && (tiannum == 2||lijianum=='1')){
//    USART3_SendStr("tian0");
//    }
//    else if(guangnum<40 && Shidu>80 &&(tiannum == 2||lijianum=='1')){
//    USART3_SendStr("tian3");
//    }

    if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==Bit_RESET){
        lightnum=1;
    }
    else{   
        lightnum=0;
    }

    LCD_ShowNum(90,210,lightnum,2,16);
    LCD_ShowNum(60,130,mennum,2,16);

  if(Esp_PUB() == 1)
			{
     LCD_ShowString(200,300,200,16,16,"fail");	
	}
  if(Esp_PUB() == 0)
			{
     LCD_ShowString(200,300,200,16,16,"ture");	
	}


}
}
