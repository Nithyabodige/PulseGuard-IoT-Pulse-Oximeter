#include <LPC21xx.h>
#include <string.h>
#include <stdio.h>

#include "project_pg.h"
#include "all_defines.h"

/* -------------------------------------------------
   External variables/functions
   ------------------------------------------------- */

extern s8 buff[200];
extern volatile u8 i;

extern void UART0_Str(s8 *str);
extern void delay_ms(unsigned int ms);

extern void cmdlcd(unsigned char cmd);
extern void strlcd(s8 *str);

/* -------------------------------------------------
   ThingSpeak API key
   Replace this with your own API key
   ------------------------------------------------- */

#define THINGSPEAK_API_KEY   ""

/* -------------------------------------------------
   Function prototypes
   ------------------------------------------------- */

void esp01_connectAP(void);
void esp01_sendToThingspeak(u16 hr, u16 spo2);

static void ESP_ClearBuffer(void);
//static void ESP_Wait(unsigned int ms);
static u8 ESP_WaitFor(s8 *expected, unsigned int timeout_ms);


/* =================================================
   Clear ESP receive buffer
   ================================================= */

static void ESP_ClearBuffer(void)
{
    i = 0;

    memset((char *)buff, '\0', 200);
}


/* =================================================
   Small delay
   ================================================= */

//static void ESP_Wait(unsigned int ms)
//{
 //   delay_ms(ms);
//}


/* =================================================
   Wait for expected string in UART buffer

   Returns:
   1 -> string received
   0 -> timeout
   ================================================= */

static u8 ESP_WaitFor(s8 *expected, unsigned int timeout_ms)
{
    unsigned int elapsed = 0;

    while(elapsed < timeout_ms)
    {
        buff[i] = '\0';

        if(strstr((char *)buff, (char *)expected) != NULL)
        {
            return 1;
        }

        delay_ms(10);
        elapsed += 10;
    }

    buff[i] = '\0';

    return 0;
}


/* =================================================
   Connect ESP-01 to Wi-Fi
   ================================================= */

void esp01_connectAP(void)
{
    /* ---------------------------------------------
       1. Test ESP
       --------------------------------------------- */

    cmdlcd(0x01);
    cmdlcd(0x80);
    strlcd("ESP AT TEST");

    ESP_ClearBuffer();

    UART0_Str("AT\r\n");

    if(ESP_WaitFor("OK", 3000))
    {
        cmdlcd(0xC0);
        strlcd("ESP OK");
    }
    else
    {
        cmdlcd(0xC0);
        strlcd("ESP ERROR");
        delay_ms(2000);
        return;
    }
			    
    delay_ms(1000);


    /* ---------------------------------------------
       2. Disable echo
       --------------------------------------------- */

    cmdlcd(0x01);
    cmdlcd(0x80);
    strlcd("DISABLE ECHO");

    ESP_ClearBuffer();

    UART0_Str("ATE0\r\n");

    ESP_WaitFor("OK", 3000);

    delay_ms(500);


    /* ---------------------------------------------
       3. Single connection mode
       --------------------------------------------- */

    cmdlcd(0x01);
    cmdlcd(0x80);
    strlcd("CIPMUX=0");

    ESP_ClearBuffer();

    UART0_Str("AT+CIPMUX=0\r\n");

    ESP_WaitFor("OK", 3000);

    delay_ms(500);


    /* ---------------------------------------------
       4. Disconnect previous Wi-Fi
       --------------------------------------------- */

    ESP_ClearBuffer();

    UART0_Str("AT+CWQAP\r\n");

    delay_ms(1000);


    /* ---------------------------------------------
       5. Connect to Wi-Fi
       --------------------------------------------- */

    cmdlcd(0x01);
    cmdlcd(0x80);
    strlcd("CONNECT WIFI");

    ESP_ClearBuffer();

    UART0_Str("AT+CWJAP=\"YOUR_WIFI_SSID\",\"YOUR_WIFI_PASSWORD\"\r\n");

    if(ESP_WaitFor("WIFI CONNECTED", 15000))
    {
        cmdlcd(0xC0);
        strlcd("WIFI CONNECTED");
    }
    else
    {
        /* Some firmware may not return exactly
           WIFI CONNECTED, so also check OK */

        buff[i] = '\0';

        if(strstr((char *)buff, "OK") != NULL)
        {
            cmdlcd(0xC0);
            strlcd("WIFI OK");
        }
        else
        {
            cmdlcd(0xC0);
            strlcd("WIFI ERROR");

            delay_ms(2000);
            return;
        }
    }

    delay_ms(2000);


    /* ---------------------------------------------
       6. Show ready
       --------------------------------------------- */

    cmdlcd(0x01);
    cmdlcd(0x80);
    strlcd("ESP READY");

    delay_ms(1500);
}


/* =================================================
   Send HR + SpO2 to ThingSpeak
   ================================================= */
  //modified
/*void esp01_sendToThingspeak(u16 hr, u16 spo2)
{
    char data[150];
    char cmd[40];

    unsigned int len;


     ---------------------------------------------
       Display uploading message
       --------------------------------------------- 

    cmdlcd(0x01);
    cmdlcd(0x80);
    strlcd("Uploading...");
    delay_ms(500);


     ---------------------------------------------
       Build HTTP GET request

       Field1 = Heart Rate
       Field2 = SpO2
       --------------------------------------------- 

    sprintf(data,
            "GET /update?api_key=%s&field1=%u&field2=%u\r\n\r\n",
            THINGSPEAK_API_KEY,
            hr,
            spo2);


     ---------------------------------------------
       Calculate exact number of bytes
       --------------------------------------------- 

    len = strlen(data);


     ---------------------------------------------
       Connect to ThingSpeak server
       --------------------------------------------- 

    ESP_ClearBuffer();

    UART0_Str("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");


    ---------------------------------------------
       Wait for connection
       --------------------------------------------- 

    if(!(ESP_WaitFor("CONNECT", 8000)))
    {
        buff[i] = '\0';

        if(strstr((char *)buff, "ALREADY CONNECTED") == NULL)
        {
            cmdlcd(0x01);
            cmdlcd(0x80);
            strlcd("TCP ERROR");

            delay_ms(2000);
            return;
        }
    }


     ---------------------------------------------
       Prepare CIPSEND command
       --------------------------------------------- 

    sprintf(cmd, "AT+CIPSEND=%u\r\n", len);


    ESP_ClearBuffer();

    UART0_Str((s8 *)cmd);


    ---------------------------------------------
       Wait for > prompt
       --------------------------------------------- 

    if(!ESP_WaitFor(">", 5000))
    {
        cmdlcd(0x01);
        cmdlcd(0x80);
        strlcd("NO SEND PROMPT");

        delay_ms(2000);

         Try closing connection 
        UART0_Str("AT+CIPCLOSE\r\n");

        return;
    }												 


     ---------------------------------------------
       Send actual HTTP GET request
       --------------------------------------------- 

    ESP_ClearBuffer();

    UART0_Str((s8 *)data);


     ---------------------------------------------
       Wait for SEND OK
       --------------------------------------------- 

    if(ESP_WaitFor("SEND OK", 8000))
    {
        cmdlcd(0x01);
        cmdlcd(0x80);
        strlcd("SEND ERROR");

        delay_ms(1500);
		UART0_Str("AT+CIPCLOSE\r\n");
		return;
    }
	cmdlcd(0x01);
    cmdlcd(0x80);
    strlcd("DATA SENT");
	delay_ms(500);
	ESP_ClearBuffer();
	 if(ESP_WaitFor("200 OK", 10000))
    {
        cmdlcd(0x01);
        cmdlcd(0x80);
        strlcd("THINGSPEAK");
		cmdlcd(0xc0);
       	strlcd("UPLOAD OK");
    }
    else
    {
        cmdlcd(0x01);
        cmdlcd(0x80);
        strlcd("THINGSPEAK");
		cmdlcd(0xc0);
		strlcd("UPLOAD FAIL");
	}
     delay_ms(2000);

        UART0_Str("AT+CIPCLOSE\r\n");

        delay_ms(1000);


    ---------------------------------------------
       wait for Thingspeak HTTP Response
       --------------------------------------------- 
	  if(ESP_WaitFor("200 OK",1000))
	  {
	  	cmdlcd(0x01);
		cmdlcd(0x80);
		strlcd("THINGSPEAK");
		cmdlcd(0xc0);
		strlcd("UPLOAD OK");
	  }
	  else
	  {
		cmdlcd(0x01);
		cmdlcd(0x80);
		strlcd("THINGSPEAK");
		cmdlcd(0xc0);
		strlcd("UPLOAD FAILED");
	  }
 

    delay_ms(2000);


    ---------------------------------------------
       Close TCP connection
       --------------------------------------------- 

    ESP_ClearBuffer();

    UART0_Str("AT+CIPCLOSE\r\n");

    delay_ms(1000);
}													 */
void esp01_sendToThingspeak(u16 hr, u16 spo2)
{

    char data[150];

    char cmd[40];

    unsigned int len;

    /* -----------------------------------------

       Display uploading

       ----------------------------------------- */

    cmdlcd(CLEAR_LCD);

    cmdlcd(GOTO_LINE1_POS0);

    strlcd("Uploading...");

    delay_ms(500);

    /* -----------------------------------------

       Build HTTP GET request

       ----------------------------------------- */

    sprintf(data,

            "GET /update?api_key=%s&field1=%u&field2=%u\r\n\r\n",

            THINGSPEAK_API_KEY,

            hr,

            spo2);

    len = strlen(data);

    /* -----------------------------------------

       Connect to ThingSpeak

       ----------------------------------------- */

    ESP_ClearBuffer();

    UART0_Str("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");

    if(!ESP_WaitFor("CONNECT", 10000))
    {

        buff[i] = '\0';

        if(strstr((char *)buff, "ALREADY CONNECTED") == NULL)
        {

            cmdlcd(CLEAR_LCD);

            cmdlcd(GOTO_LINE1_POS0);

            strlcd("TCP ERROR");

            delay_ms(2000);

            return;

        }

    }

    /* -----------------------------------------

       CIPSEND

       ----------------------------------------- */

    sprintf(cmd, "AT+CIPSEND=%u\r\n", len);

    ESP_ClearBuffer();

    UART0_Str((s8 *)cmd);

    /* -----------------------------------------

       Wait for > prompt

       ----------------------------------------- */

    if(!ESP_WaitFor(">", 5000))
    {

        cmdlcd(CLEAR_LCD);

        cmdlcd(GOTO_LINE1_POS0);

        strlcd("NO SEND PROMPT");

        delay_ms(2000);

        UART0_Str("AT+CIPCLOSE\r\n");

        return;

    }

    /* -----------------------------------------

       Send HTTP request

       ----------------------------------------- */

    ESP_ClearBuffer();

    UART0_Str((s8 *)data);

    /*

     * Give ESP enough time to send the request

     * and receive ThingSpeak response.

     *i/

    //delay_ms(3000);

    //buff[i] = '\0';
    /*-----------------------------------------
   		Wait for SEND OK
   ----------------------------------------- */

	if(!ESP_WaitFor("SEND OK", 8000))
	{
    		cmdlcd(CLEAR_LCD);
    		cmdlcd(GOTO_LINE1_POS0);
    		strlcd("SEND ERROR");

    		delay_ms(1500);

    		UART0_Str("AT+CIPCLOSE\r\n");

    		return;
	}
	/* -----------------------------------------
   		SEND OK received
   ----------------------------------------- */

	cmdlcd(CLEAR_LCD);
	cmdlcd(GOTO_LINE1_POS0);
	strlcd("DATA SENT");

	delay_ms(500);


/*
 * IMPORTANT:
 * DO NOT call ESP_ClearBuffer() here.
 *
 * ThingSpeak response may already be
 * present in buff.
 */


    /* -----------------------------------------

       wait for ThingSpeak HTTP response

       ----------------------------------------- */

    if(ESP_WaitFor("200 OK",10000))
    {

        cmdlcd(CLEAR_LCD);

        cmdlcd(GOTO_LINE1_POS0);

        strlcd("THINGSPEAK");

        cmdlcd(GOTO_LINE2_POS0);

        strlcd("UPLOAD OK");

    }
    else
    {

        /*

         * ThingSpeak may still have received the

         * request even if the complete HTTP response

         * was not captured by our UART buffer.

         */

        cmdlcd(CLEAR_LCD);

        cmdlcd(GOTO_LINE1_POS0);

        strlcd("DATA SENT");

        cmdlcd(GOTO_LINE2_POS0);

        strlcd("CHECK CHANNEL");
	/* cmdlcd(CLEAR_LCD);
	 cmdlcd(GOTO_LINE1_POS0);
	 strlcd("NO SERVER RESP");
	 cmdlcd(GOTO_LINE2_POS0);
	 strlcd("CHECK CHANNEL"); */

    }

    delay_ms(2000);

    /* -----------------------------------------

       Close TCP connection

       ----------------------------------------- */

    ESP_ClearBuffer();

    UART0_Str("AT+CIPCLOSE\r\n");

    delay_ms(1000);

}
