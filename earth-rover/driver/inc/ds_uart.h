/* ds_uart.h --- 
 * 
 * Filename: ds_uart.h
 * Description: 
 * Author: 
 * Maintainer: 
 * Created: Tue Jan 15 11:13:19 2013
 * Last-Updated: 
 *           By: 
 *     Update #: 0
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
#include <queue.h>
/* Code: */

queue_t txbuf;
queue_t rxbuf;

int putchar(int);
int getchar(void);
void getline(char *buffme, int bsize);
void putstring(char *);

void initialize_usart(void);
int nonBlocking_getchar(void);

/* ds_uart.h ends here */
