/* ds_uart.c ---
 *
 * Filename: ds_uart.c
 * Description:
 * Author:
 * Maintainer:
 * Created: Tue Jan 15 11:12:30 2013
 * Last-Updated:
 *                     By:
 *         Update #: 0
 * Keywords:
 * Compatibility:
 *
 */

/* Commentary:
 *
 *
 *
 */

/* Change log:
 *
 *
 */

/* Code: */
#include <stm32f30x.h>
#include <stm32f30x_usart.h>
#include <stm32f30x_tim.h>
#include <stm32f30x_misc.h>
#include <ds_uart.h>
#include <ds_wifly.h>
#include <rover.h>

int putchar(int c) {

  while (queue_full(&txbuf));
  enqueue(&txbuf, c);
  return c;

}



int getchar(void) {
  int c;
  while (!(c = dequeue(&rxbuf)));
  return c;
}


void getline(char *buffme, int bsize) {

  int c;
  int i = i;

  memset(buffme, 0, bsize);

  while(i < bsize) {
    c = getchar();
    if ( c == '\n' ) {
      break;
    }
    buffme[i] = c;
    i++;
  }
  if (buffme[i] != 0) buffme[i+1] = 0;

}

void putstring(char * c) {
  for (; c != 0; c++) {
    putchar(*c);
  }
}

void initialize_usart(void) {
  // GPIOC Clock Enable
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);


  GPIO_InitTypeDef GPIO_InitStructure;

  // TX pin initialization
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC,&GPIO_InitStructure);

  // RX Pin Initialization
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC , &GPIO_InitStructure);

  // Alternate Function Configuration
  GPIO_PinAFConfig(GPIOC,4,GPIO_AF_7);
  GPIO_PinAFConfig(GPIOC,5,GPIO_AF_7);

  // UART1 Clock Enable
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  // UART1 Configuration
  USART_InitTypeDef USART_InitStructure;

  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1 ,&USART_InitStructure);
  USART_Cmd(USART1 , ENABLE);

  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x08;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x08;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
}
void USART1_IRQHandler(void) {

  int ch = 0;

  /*
   * definitions already in .h file Should these variables below not call on
   *the global? Possible danger of mutation regardless.
   *Bad to name local variables the same as global ones.
   *
   queue_t rxbuf;
   queue_t txbuf;
   *
   */

  if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE)){
    ch = USART_ReceiveData(USART1);
    enqueue(&rxbuf,ch);
  }

  if (USART_GetFlagStatus(USART1,USART_FLAG_TXE)) {
    ch = dequeue(&txbuf);
    if (ch) {
      USART_SendData(USART1,ch);
    }
    else {
      USART_ITConfig(USART1,USART_IT_TXE,DISABLE);
    }
  }
}

void flush_uart(void) {

  USART_ITConfig(USART1,USART_IT_TXE,ENABLE);

}


int nonBlocking_getchar(void) {

  int ch;
  ch=dequeue(&rxbuf);
  return ch;

}

int nonBlocking_getline(char *buffa, int size) {
  int c;
  c=dequeue(&rxbuf);
  if (!c) return;
  int i = 1;
  buffa[0] = c;


  while(i < size) {
    c = getchar();
    if ( c == '\n' ) {
      buffa[i] = c;
      break;
    }
    buffa[i] = c;
    i++;
  }

  /* buffa[i] = 0; */
  /* if (buffa[i] != 0) buffa[i+1] = 0; */


}
/* ds_uart.c ends here */
