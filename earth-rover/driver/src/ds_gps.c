/* ds_gps.c ---
 *
 * Filename: ds_gps.c
 * Description:
 * Author: Wilfred Law
 * Maintainer:
 * Created:
 * Last-Updated:
 *                         By:
 *             Update #: 0
 * Keywords:
 * Compatibility:
 *
 */

/* Commentary: PINS!
 *             BLACK : GROUND
 *             RED: 3.3v
 *             WHITE: TX
 *             USART
 *
 */

/* Change log:
 *
 *
 */
/* Code: */

#include <stm32f30x.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_usart.h>
#include <stm32f30x_tim.h>
#include <stm32f30x_misc.h>
#include <ds_gps.h>

void gps_initialise(void) {

  //GPIO Clocks Enable
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;

  //Initialize UART Pins
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC,&GPIO_InitStructure);

  //Alternate Function Configuration
  GPIO_PinAFConfig(GPIOC,11,GPIO_AF_7);

  //UART3 Clock Enable
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

  //USART3 Configuration

  USART_InitTypeDef USART_InitStructure;

  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate = 57600;
  USART_InitStructure.USART_Mode = USART_Mode_Rx;
  USART_Init(USART3 ,&USART_InitStructure);
  USART_Cmd(USART3 , ENABLE);

  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x06;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x06;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);



}

void USART3_IRQHandler(void) {

  int ch = 0;
  if (USART_GetFlagStatus(USART3, USART_FLAG_RXNE)){
    ch = USART_ReceiveData(USART3);
    enqueue(&gps_buffer, ch);
  }


}

int gps_getchar(void) {
  int ch;
  while(!(ch=dequeue(&gps_buffer)));
  return ch;
}

int gps_compare(char *gps_data, char *needed_str) {

  int i = 0;
  while(gps_data[i] == needed_str[i] && i < 6) {
    i++;
  }
  if(i == 6) {
    return 1;
  } else {
    return 0;
  }

}

void gps_strcopy(int *original, int *tocopy) {
  int i = 0;
  char c = original[0];
  while(c) {
    c = original[i];
    tocopy[i] = original[i];
    i++;
  }

}

int gps_getSentence(char *destination, char *sentenceType) {

  char ch = gps_getchar();
  while(ch != '$') {
    ch = gps_getchar();
  }
  destination[0] = ch;

  int parseStatus = 1;
  int linesRead = 0;
  int index = 0;
  memset(destination, 0, GPS_SENTENCE_SIZE);
  while(parseStatus) {

    ch = gps_getchar();
    destination[index] = ch;
    index++;

    if(ch == 10 || ch == '\r' || ch == '\n') {
      if( gps_compare(destination, sentenceType)) {
        parseStatus = 0;
      } else {
        memset(destination, 0, GPS_SENTENCE_SIZE);
        index = 0;
      }
      linesRead++;
    }

  }


}

void gps_getGPGGA_data(char *latitude, char *longtitude, char *altitude, char *gpsSentence) {

  int commasPassed = 0;
  int index = 0;
  int parsing = 1;
  int lat = 0;
  int longt = 0;
  int alt = 0;
  while(parsing) {
    if(gpsSentence[index] == ',') {
      commasPassed++;
      index++;
    }
    switch(commasPassed) {
    case 2:
      latitude[lat] = gpsSentence[index];
      lat++;
      index++;
      break;
    case 4:
      longtitude[longt] = gpsSentence[index];
      longt++;
      index++;
      break;
    case 9:
      altitude[alt] = gpsSentence[index];
      alt++;
      index++;
      break;
    case 10:
      parsing = 0;
    default :
      index++;
    }
  }

}
