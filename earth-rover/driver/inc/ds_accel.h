/* ds_accel.h --- 
 * 
 * Filename: ds_accel.h
 * Description: 
 * Author: 
 * Maintainer: 
 * Created: Thu Jan 31 21:59:16 2013 (-0500)
 * Version: 
 * Last-Updated: 
 *           By: 
 *     Update #: 0
 * URL: 
 * Doc URL: 
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change Log:
 * 
 * 
 */

/* Code: */
#include <stm32f30x.h>

#define accel (0x32)

void ds_accel_init();
void ds_accel_read(float *);

/* ds_accel.h ends here */
