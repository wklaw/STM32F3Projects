/* ds_mag.h --- 
 * 
 * Filename: ds_mag.h
 * Description: 
 * Author: 
 * Maintainer: 
 * Created: Thu Jan 31 23:34:21 2013 (-0500)
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

#define magneto (0x3C)

void ds_mag_init();
void ds_mag_read(float *magneto_data);

/* ds_mag.h ends here */
