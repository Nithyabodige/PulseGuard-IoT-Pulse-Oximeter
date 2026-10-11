/*#include <string.h>
#include "project_pg.h"
#include "all_defines.h"

extern s8 buff[200];
extern u8 i;

void esp01_connectAP()
{
	cmdlcd(0x01);
	cmdlcd(0x80);
	strlcd("AT");
	delay_ms(1000);
	UART0_Str("AT\r\n");
	i=0;
	memset(buff,'\0',200);
	while(i<4);
	delay_ms(500);
	buff[i] = '\0';
	cmdlcd(0x01);
	cmdlcd(0x80);
	strlcd(buff);
	delay_ms(2000);
	if(strstr((char *)buff,"OK"))
	{
		cmdlcd(0xC0);
		strlcd("OK");
		delay_ms(1000);		
	}
	else
	{
		cmdlcd(0xC0);
		strlcd("ERROR");
		delay_ms(1000);		
		return;
	}
	
	
	cmdlcd(0x01);
	cmdlcd(0x80);
	strlcd("ATE0");
	delay_ms(1000);
	UART0_Str("ATE0\r\n");
	i=0;memset(buff,'\0',200);
	while(i<4);
	delay_ms(500);
	buff[i] = '\0';
	cmdlcd(0x01);
	cmdlcd(0x80);
	strlcd(buff);
	delay_ms(2000);
	if(strstr((char *)buff,"OK"))
	{
		cmdlcd(0xC0);
		strlcd("OK");
		delay_ms(1000);		
	}
	else
	{
		cmdlcd(0xC0);
		strlcd("ERROR");
		delay_ms(1000);		
		return;
	}
	
	
	cmdlcd(0x01);
	cmdlcd(0x80);
	strlcd("AT+CIPMUX");
	delay_ms(1000);
	UART0_Str("AT+CIPMUX=0\r\n");
	i=0;memset(buff,'\0',200);
	while(i<4);
	delay_ms(500);
	buff[i] = '\0';
	cmdlcd(0x01);
	cmdlcd(0x80);
	strlcd(buff);
	delay_ms(2000);
	if(strstr((char *)buff,"OK"))
	{
		cmdlcd(0xC0);
		strlcd("OK");
		delay_ms(1000);		
	}
	else
	{
		cmdlcd(0xC0);
		strlcd("ERROR");
		delay_ms(1000);		
		return;
	}
	
	cmdlcd(0x01);
	cmdlcd(0x80);
	strlcd("AT+CWQAP");
	delay_ms(1000);
	UART0_Str("AT+CWQAP\r\n");
	i=0;memset(buff,'\0',200);
	while(i<4);
	delay_ms(1500);
	buff[i] = '\0';
	cmdlcd(0x01);
	cmdlcd(0x80);
	strlcd(buff);
	delay_ms(2000);
	if(strstr((char *)buff,"OK"))
	{
		cmdlcd(0xC0);
		strlcd("OK");
		delay_ms(1000);		
	}
	else						 	
	{
		cmdlcd(0xC0);
		strlcd("ERROR");
		delay_ms(1000);		
		return;
	}
	
	
	
	cmdlcd(0x01);
	cmdlcd(0x80);
	strlcd("AT+CWJAP");
	delay_ms(1000);
	//need to change the wifi network name and password
	UART0_Str("AT+CWJAP=\"YOUR_WIFI_SSID\",\"YOUR_WIFI_PASSWORD\"\r\n");
	i=0;memset(buff,'\0',200);
	while(i<4);
	delay_ms(2500);
	buff[i] = '\0';
	cmdlcd(0x01);
	cmdlcd(0x80);
	strlcd(buff);
	delay_ms(2000);
	if(strstr((char *)buff,"WIFI CONNECTED"))
	{
		cmdlcd(0xC0);
		strlcd("OK");
		delay_ms(1000);		
	}
	else
	{
		cmdlcd(0xC0);
		strlcd("ERROR");
		delay_ms(1000);		
		return;
	}
	
}

void esp01_sendToThingspeak(s8 *val)
{
	cmdlcd(0x01);
	cmdlcd(0x80);
	strlcd("AT+CIPSTART");
	delay_ms(1000);
	UART0_Str("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
	i=0;memset(buff,'\0',200);
	while(i<5);
	delay_ms(2500);
	buff[i] = '\0';
	cmdlcd(0x01);
	cmdlcd(0x80);
	strlcd(buff);
	delay_ms(2000);
	if(strstr((char *)buff,"CONNECT") || strstr((char *)buff,"ALREADY CONNECTED"))
	{
		cmdlcd(0xC0);
		strlcd("OK");
		delay_ms(1000);
		
		cmdlcd(0x01);
		cmdlcd(0x80);
		strlcd("AT+CIPSEND");
		delay_ms(1000);
		UART0_Str("AT+CIPSEND=51\r\n");
		i=0;
		memset(buff,'\0',200);
		//while(buff[i] != '>');
		delay_ms(500);
		//need to change the thingspeak write API key accordind to your channel
		UART0_Str("GET /update?api_key=YOUR_THINGSPEAK_WRITE_API_KEY&field1=");
		UART0_Str(val);
		UART0_Str("\r\n\r\n");
		delay_ms(5000);
		delay_ms(5000);
		buff[i] = '\0';
		cmdlcd(0x01);
		cmdlcd(0x80);
		strlcd(buff);
		delay_ms(2000);
		if(strstr((char *)buff,"SEND OK"))
		{
			cmdlcd(0x01);
			strlcd("DATA UPDATED");
			delay_ms(1000);			
		}
		/*
		else if(!strstr(buff,"CLOSED"))
		{
			cmdlcd(0x01);
			cmdlcd(0x80);
			strlcd("AT+CIPCLOSE");
			delay_ms(1000);
			UART0_Str("AT+CIPCLOSE\r\n");
			i=0;memset(buff,'\0',200);
			while(i<5);
			delay_ms(2500);
			buff[i] = '\0';
			cmdlcd(0x01);
			cmdlcd(0x80);
			strlcd(buff);
			delay_ms(2000);
			if(strstr(buff,"OK"))
			{
				cmdlcd(0x01);
				cmdlcd(0x80);
				strlcd("OK");
				delay_ms(2000);				
			}
			else
			{
				cmdlcd(0x01);
				cmdlcd(0x80);
				strlcd("ERROR");
				delay_ms(2000);		
			}
		
		}*/
		/*else
		{
			cmdlcd(0x01);
			strlcd("DATA NOT UPDATED");
			delay_ms(1000);	
		}
		
	}
	else
	{
		cmdlcd(0xC0);
		strlcd("ERROR");
		delay_ms(1000);		
		return;
	}
		 
	
} */*/

