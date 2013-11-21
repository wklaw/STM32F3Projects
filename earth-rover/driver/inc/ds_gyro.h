/* ds_l3dg20.h --- 
 * 
 * Filename: ds_l3dg20.h
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Thu Jan 24 05:43:27 2013 (-0500)
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

/* This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301, USA.
 */

/* Code: */
#include <stm32f30x.h>

#define GYRO_Sensitivity_500dps   (float) 57.1429f

#define GYRO_CS_LOW()   GPIO_ResetBits(GPIOE, GPIO_Pin_3);
#define GYRO_CS_HIGH()  GPIO_SetBits(GPIOE, GPIO_Pin_3);

void ds_gyro_interface_init();
void ds_gyro_init(void);
static uint8_t ds_gyro_sendbyte(uint8_t);
void ds_gyro_read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);
void ds_gyro_write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
void ds_gyro_getdata(float *pfData);
float getX();
float getY();
float getZ();
int isButtonPressed();
/* ds_l3dg20.h ends here */
