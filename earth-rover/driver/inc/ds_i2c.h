/* ds_i2c.h --- 
 * 
 * Filename: ds_i2c.h
 * Description: 
 * Author: 
 * Maintainer: 
 * Created: Thu Jan 31 20:54:21 2013 (-0500)
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

void ds_i2c1_init();
void ds_i2c1_read(uint8_t device, uint8_t reg, uint8_t* buffer, uint16_t numbytes);
void ds_i2c1_write(uint8_t device, uint8_t reg, uint8_t* value);

/* ds_i2c.h ends here */
