/* ds_wifly.h ---
 *
 * Filename: ds_wifly.h
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
 *
 */

/* Change log:
 *
 *
 */

/* Code: */

#include <queue.h>

queue_t wifly_txbuf;
queue_t wifly_rxbuf;

int wifly_getchar(void);
void wifly_getline(char *, int);
void wifly_getline_noblock(char *, int);
void wifly_sendto(char *ptr);
void initialise_wifly(void);
void print_wifly_queue(void);
