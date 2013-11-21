/* ds_wifly.c ---
 *
 * Filename: ds_wifly.c
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

/* Commentary: IMPORTANT!!!!!!!
 *             RX Pin on the wifly module is Data IN to module
 *             TX Pin on the wifly module is Data OUT from module
 *
 *             When connecting pins to the MCU; MCU TX should be transmitting
 *             to the wifly's RX and MCU's RX should be receiving from wifly
 *             TX.
 *
 *             ST RX is pin 4 and ST TX is pin 3
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
#include <stm32f30x_rcc.h>
#include <ds_wifly.h>
#include <rover.h>

int wifly_sendchar(int ch) {

  if (!ch) return;
  if (ch == '\n') ch = '\r';
  if (ch == '\t') ch = '\n';
  while(queue_full(&wifly_txbuf));
  //printf("sending char %c\n", ch);
  enqueue(&wifly_txbuf, ch);
  return ch;

}

void wifly_sendstring(char *overlord) {
  //print_wifly_queue();
  if (overlord[0] == '$') {
    wifly_sendchar('$');
    wifly_sendchar('$');
    wifly_sendchar('$');
    return;
  }
  for (; *overlord != 0; overlord++) {
    wifly_sendchar(*overlord);
  }
}

int wifly_getchar(void) {
  int c;
  int i = 0;
  while (!(c = dequeue(&wifly_rxbuf)) && i++ < 100);
  return c;
}

void wifly_getline(char *buffme, int bsize) {

  int c;
  int i = i;

  memset(buffme, 0, bsize);

  while(i < bsize && buffme[i] != 0) {
    c = wifly_getchar();
    buffme[i] = c;
    i++;
  }
  if (buffme[i] != 0) buffme[i+1] = 0;

}
void wifly_getline_noblock(char *buffa, int size) {
  int ch = dequeue(&wifly_rxbuf);
  int i = 0;
  if (ch) {
    while (ch) {
      buffa[i++] = ch;
      ch = wifly_getchar();
    }
  }
  /*
  int ch;
  int i = 0;
  while (ch = dequeue(&wifly_rxbuf) && i < size) {
    buffa[i++] = ch;
  }
  int c;
  c=dequeue(&wifly_rxbuf);
  if (!c) return;
  int i = 1;
  buffa[0] = c;


  while(i < size) {
    c = wifly_getchar();
    if ( c == 0 || c == '\r' || c == '\n' ) {
      buffa[i] = c;
      break;
    }
    buffa[i] = c;
    i++;
  }
  //  if (buffa[i] != 0) buffa[i+1] = 0;
  */
}
void wifly_getlines_noblock(char *buffa, int size) {
  int c;
  memset(buffa, 0, size);
  int i = 0;

  while(i < size && buffa[i] != 0) {
    c = wifly_getchar();
    buffa[i] = c;
    i++;
  }
  //  if (buffa[i] != 0) buffa[i+1] = 0;


}
void initialize_wifly(void) {


  //GPIO Clocks Enable
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;

  // 3 = tx; 4 = rx
  //Initialize UART Pins  
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB,&GPIO_InitStructure);

  //Alternate Function Configuration
  GPIO_PinAFConfig(GPIOB,3,GPIO_AF_7);
  GPIO_PinAFConfig(GPIOB,4,GPIO_AF_7);

  //UART2 Clock Enable
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  //USART2 Configuration

  USART_InitTypeDef USART_InitStructure;

  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2 ,&USART_InitStructure);
  USART_Cmd(USART2 , ENABLE);

  //USART Interrupts
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x07;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x07;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);

}

void USART2_IRQHandler(void) {
  

  int ch = 0;

  if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE)){
    ch = USART_ReceiveData(USART2);
    enqueue(&wifly_rxbuf,ch);
  }
  
  if (USART_GetFlagStatus(USART2,USART_FLAG_TXE)) {
    ch = dequeue(&wifly_txbuf);
    if (ch) {
      USART_SendData(USART2,ch);
    }
    else {
      USART_ITConfig(USART2,USART_IT_TXE,DISABLE);
    }
  }

}


void flush_uart2(void) {
  USART_ITConfig(USART2,USART_IT_TXE,ENABLE);
}
void print_wifly_queue() {
  int i;
  printf("queue: [");
  for (i=0;i<QUEUE_SIZE;i++) {
    if (wifly_txbuf.buffer[i] != 0)
      putchar(wifly_txbuf.buffer[i]);
  }
  printf("]\n");
}
