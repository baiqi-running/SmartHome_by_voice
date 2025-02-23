#include "stm32f10x.h"                  // Device header
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include "delay.h"
#include "lcd.h"
#include "cjson.h"
#include "pwm.h"
extern u8 Temperature;

extern u8 Shidu;
extern uint8_t Switch1;
extern uint8_t Switch2;
extern uint8_t lightnum;
extern uint8_t chuangnum;
extern uint8_t powernum;
extern uint16_t value;
extern uint8_t guangnum;
extern uint16_t T;
uint8_t jingnum;
u8 lightnumm;
uint8_t fengnum;
u8 chuangnumm;
u8 lijianum;
u8 tiannum;
u8 lijianumm;
u8 jianum;

extern char RECS[500];

extern char RECSS[500];
const char* WIFI ="LCC";
const char* WIFIASSWORD="loch12345";
const char* ClintID="mqtt_stm32|securemode=2\\,signmethod=hmacsha1\\,timestamp=1701499743597|";
const char* username="mqtt_stm32&k0eabxXyONS";
const char* passwd="55D42F5391FB973B33AF4EB796D4B93AF9BB482F";
const char* Url="k0eabxXyONS.iot-as-mqtt.cn-shanghai.aliyuncs.com";
const char* pubtopic="/sys/k0eabxXyONS/mqtt_stm32/thing/event/property/post";
const char* subtopic="/sys/k0eabxXyONS/mqtt_stm32/thing/service/property/set";//订阅
//const char* subtopic="/sys/k0eabxXyONS/mqtt_stm32/thing/event/property/post _reply";
char* func1="temp";
 char* func2="humi";
 char* func3="pwm";
 char* func4="people";
 char* func5="light";
 char* func6="chuanglian";
 char* func7="power";
 char* func8="value";
 char* func9="konqi";  
 

char esp_Init(void)
{
	memset(RECS,0,sizeof(RECS));
	printf("AT+RST\r\n");  //重启
	delay_ms(2000);
	
	memset(RECS,0,sizeof(RECS));
	printf("ATE0\r\n");    //关闭回显
	delay_ms(10);
	if(strcmp(RECS,"OK"))
		return 1;
	
	printf("AT+CWMODE=1\r\n"); //Station模式
	delay_ms(1000);
	if(strcmp(RECS,"OK"))
		return 2;
	
	memset(RECS,0,sizeof(RECS));
	printf("AT+CWJAP=\"%s\",\"%s\"\r\n",WIFI,WIFIASSWORD); //连接热点
	delay_ms(2000);
	if(strcmp(RECS,"OK"))
		return 3;
	
	memset(RECS,0,sizeof(RECS));
	printf("AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"\r\n",ClintID,username,passwd);//用户信息配置
	delay_ms(10);
	if(strcmp(RECS,"OK"))
		return 4;
	
	memset(RECS,0,sizeof(RECS));
	printf("AT+MQTTCONN=0,\"%s\",1883,1\r\n",Url); //连接服务器
	delay_ms(1000);
	if(strcmp(RECS,"OK"))
		return 5;
	
	printf("AT+MQTTSUB=0,\"%s\",1\r\n",subtopic); //订阅消息
	delay_ms(500);
	if(strcmp(RECS,"OK"))
		return 5;

	memset(RECS,0,sizeof(RECS));
	return 0;


}
//功能：esp发送消息
//参数：无
//返回值：0：发送成功；1：发送失败
char Esp_PUB(void)
{
	memset(RECS,0,sizeof(RECS));
	printf("AT+MQTTPUB=0,\"%s\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"%s\\\":%d\\,\\\"%s\\\":%d\\,\\\"%s\\\":%d\\,\\\"%s\\\":%d\\,\\\"%s\\\":%d\\,\\\"%s\\\":%d\\,\\\"%s\\\":%d\\,\\\"%s\\\":%d}}\",0,0\r\n",pubtopic,func1,Temperature,func2,Shidu,func3,Switch1,func4,Switch2,func5,lightnum,func6,chuangnum,func7,powernum,func9,value);
	//while(RECS[0]);//等待ESP返回数据
	delay_ms(500);//延时等待数据接收完成
	if(strcmp(RECS,"ERROR")==0)
		return 1;
	return 0;
}



void CommandAnalyse(void)
{
	if(strncmp(RECS,"ev",2)==0)
	{
		uint8_t i=0;
		while(RECS[i++] != '\0')             
		{
			if(strncmp((RECS+i),func5,5)==0)
			{
				while(RECS[i++] != 'e');       
			lightnumm=RECS[i+2];
             if(lightnumm == '1'){
                  GPIO_ResetBits(GPIOB,GPIO_Pin_1);

                }
             if(lightnumm == '0'){
                  GPIO_SetBits(GPIOB,GPIO_Pin_1);

                }
			}


		    if(strncmp((RECS+i),func6,10)==0)
			{
				while(RECS[i++] != 'v');
				  chuangnumm=RECS[i+6];
                if(chuangnumm == '1'){
                  chuangnum =1;
                  PWM_Speed(5);

                }
               if(chuangnumm == '0'){
                  chuangnum =0;
                  PWM_Speed(4);

                }
			}

            if(strncmp((RECS+i),func3,3)==0)
			{
				while(RECS[i++] != 'u');
				  fengnum=RECS[i+3];
                if(fengnum == '1'){
                  Switch1 =1;
                  PWM_Speed(3);
                  powernum=100;

                }
               if(fengnum == '0'){
                  Switch1 =0;
                  PWM_Speed(0);
                  powernum=0;
                }
			}


		}
}
}




void CommandAnalyse2(void)
{
	if(strncmp(RECSS,"ed",2)==0||strncmp(RECSS,"led",3)==0)
	{
		uint8_t i=0;
				while(RECSS[i++] != ':');       
				lightnumm=RECSS[i];
             if(lightnumm == '1'){
                  GPIO_ResetBits(GPIOB,GPIO_Pin_1);
                  lightnum =1;
                }
			}
             if(lightnumm == '0'){
                  GPIO_SetBits(GPIOB,GPIO_Pin_1);
                  lightnum =0;
                }
			


        if(strncmp(RECSS,"duoji",5)==0||strncmp(RECSS,"uoji",4)==0)
        {
                uint8_t i=0;
				while(RECSS[i++] != ':');       
				chuangnumm=RECSS[i];
             if(chuangnumm == '1'){
                  chuangnum=1;
                  PWM_Speed(5);

                }
             if(chuangnumm == '0'){
                 chuangnum=0;
                  PWM_Speed(4);
                }
            }


       if(strncmp(RECSS,"feng",4)==0||strncmp(RECSS,"eng",3)==0)
        {
                uint8_t i=0;
				while(RECSS[i++] != ':');       
				fengnum=RECSS[i];
             if(fengnum == '1'){
                  powernum=100;
                  Switch1 =1;
                  PWM_Speed(3);

                }
             if(fengnum == '0'){
                 powernum=0;
                 Switch1 =0;
                  PWM_Speed(0);
                }
            }


    if(strncmp(RECSS,"jing",4)==0||strncmp(RECSS,"ing",3)==0)
        {
                uint8_t i=0;
				while(RECSS[i++] != ':');       
				lijianum=RECSS[i];
             if(lijianum == '1'){
                  chuangnum=1;
                  PWM_Speed(5);
                  GPIO_SetBits(GPIOB,GPIO_Pin_1);
                   lijianumm=1;
                }
             if(lijianum == '0'){
                   lijianumm=0;
                GPIO_ResetBits(GPIOC,GPIO_Pin_0);
                }
     if(guangnum>=120 && (tiannum == 2||lijianum=='1')){
    USART3_SendStr("tian1");
    }
     if(guangnum>=60&&guangnum<120 && (tiannum == 2||lijianum=='1')){
    USART3_SendStr("tian0");
    }
    else if(guangnum<40 && Shidu>80 &&(tiannum == 2||lijianum=='1')){
    USART3_SendStr("tian3");
    }

}

    if(strncmp(RECSS,"tian",4)==0||strncmp(RECSS,"ian",3)==0)
        {
                uint8_t i=0;
				while(RECSS[i++] != ':');       
				tiannum=RECSS[i];
             if(tiannum == '2'){
                 tiannum = 2;
     if(guangnum>=120){
    USART3_SendStr("tian1");
    }
     if(guangnum>=60&&guangnum<120){
    USART3_SendStr("tian0");
    }
    else if(guangnum<40 && Shidu>80){
    USART3_SendStr("tian3");
            }
               

}
}	

if(strncmp(RECSS,"jia",3)==0||strncmp(RECSS,"ia",2)==0)
        {
                uint8_t i=0;
				while(RECSS[i++] != ':');       
				jianum=RECSS[i];
             if(jianum == '1'){

//                Switch2=1;
                }

             if(jianum == '0'){
                 Switch2=0;
                }
            }


}

//    cJSON *json, *json_items, *json_light, *json_value;
//    json = cJSON_Parse(RECS);
//    json_items = cJSON_GetObjectItem(json, "items");
//    if(json_items != NULL){
//    printf("null");
//}
//    json_light = cJSON_GetObjectItem(json_items, "light");
//    json_value = cJSON_GetObjectItem(json_light, "value");
//    if(json_value->type == 1)
//    {
//        GPIO_ResetBits(GPIOB,GPIO_Pin_1);
//        lightnum = json_value->valueint;
//   //      printf("num: %d",json_value->valueint);  
//    }
//      if(json_value->type == 0)
//        {
//        GPIO_SetBits(GPIOB,GPIO_Pin_1);
//        lightnum = json_value->valueint;
//    //     printf("num: %d",json_value->valueint);
//        }

//}
