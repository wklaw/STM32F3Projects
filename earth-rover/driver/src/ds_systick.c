/* ds_systick.c --- 
 * 
 * Filename: ds_systick.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Thu Feb  7 09:20:13 2013 (-0500)
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

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <stm32f30x_rcc.h>
#include <stm32f30x_gpio.h>
#include <ds_systick.h>
#include <ds_uart.h>

extern volatile int systick_flag = 0;

void ds_systick_init(void) {
    SysTick_Config(SystemCoreClock/100);
}

void SysTick_Handler(void) {
    static state = 0;
    state ^= 1;
    systick_flag = state;
    if (state) {
        GPIOE->BSRR = 0x8000;
    } else {
        GPIOE->BRR = 0x8000;
    }

    //Added for lab5: usart interrupt
    flush_uart();
    flush_uart2();

}

/* ds_systick.c ends here */
