/* ds_rtc.c --- 
 * 
 * Filename: ds_rtc.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Fri Feb  8 04:23:50 2013 (-0500)
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
#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>
#include <stm32f30x_rtc.h>
#include <stm32f30x_pwr.h>

#include <ds_rtc.h>

void ds_rtc_init(void) {

    RTC_TimeTypeDef  RTC_TimeStructure;
    RTC_DateTypeDef  RTC_DateStructure;
    RTC_InitTypeDef  RTC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_BackupAccessCmd(ENABLE);   
    RCC_BackupResetCmd(ENABLE);
    RCC_BackupResetCmd(DISABLE);
    PWR_BackupAccessCmd(ENABLE);
    RCC_LSICmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
    RCC_RTCCLKCmd(ENABLE);
    RTC_WaitForSynchro(); 
    
    RTC_StructInit(&RTC_InitStructure);
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
    RTC_InitStructure.RTC_AsynchPrediv = 88;
    RTC_InitStructure.RTC_SynchPrediv = 470;
    RTC_Init(&RTC_InitStructure);
    
    RTC_DateStructure.RTC_Year = 13;
    RTC_DateStructure.RTC_Month = RTC_Month_February;
    RTC_DateStructure.RTC_Date = 8;
    RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Friday;
    RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
    
    RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
    RTC_TimeStructure.RTC_Hours   = 0x09;
    RTC_TimeStructure.RTC_Minutes = 0x05;
    RTC_TimeStructure.RTC_Seconds = 0x00;  
    RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
    
}


/* ds_rtc.c ends here */
