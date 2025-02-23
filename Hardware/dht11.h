#ifndef _DHT11_H
#define __DHT11_H



typedef struct
{
	uint8_t  humi_int;		//湿度的整数部分
	uint8_t  humi_deci;	 	//湿度的小数部分
	uint8_t  Temperature_int;	 	//温度的整数部分
	uint8_t  Temperature_deci;	 	//温度的小数部分
	uint8_t  check_sum;	 	//校验和
		                 
}DHT11_Data_TypeDef;

void DHT11_GPIO_Config(void);
void DHT11_Mode_IPU(void);
void DHT11_Mode_Out_PP(void);
uint8_t Read_Byte(void);
void dht_Readdata(uint8_t *Shidu,uint8_t *Temperature);

#endif
