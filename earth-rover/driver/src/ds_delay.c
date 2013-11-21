/* ds_delay.c ---
 *
 * Filename: ds_delay.c
 * Description:
 * Author:
 * Maintainer:
 * Created: Fri Feb 15 09:15:30 2013
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
#include <stm32f30x_tim.h>

void ds_delay_init(void) {

  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;

  uint16_t PrescalerValue = 0;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  PrescalerValue = (uint16_t) ((SystemCoreClock) / 36000000) - 1;
  TIM_TimeBaseStructure.TIM_Period = 65535;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 36000;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);
  TIM_Cmd(TIM3, ENABLE);

}

void ds_delay_uS(uint16_t uS_count) {

  if (uS_count >= 1000) {
    uS_count = 1000;
  }
  uS_count *= 36;
  TIM_Cmd(TIM3, DISABLE);
  TIM_SetCounter(TIM3,0);
  TIM_SetCompare1(TIM3,uS_count);
  TIM_Cmd(TIM3, ENABLE);
  while (TIM_GetFlagStatus(TIM3,TIM_FLAG_CC1)==RESET);
  TIM_ClearFlag(TIM3,TIM_FLAG_CC1);
  TIM_Cmd(TIM3, DISABLE);

}
void ds_delay_mS(uint16_t c) {
  while (c > 0) {
    ds_delay_uS(1000);
    c--;
  }
}
