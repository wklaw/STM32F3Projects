#include <motors.h>
#include <stm32f30x_gpio.h>

void motors_init() {
  // PD10 = right motor forward
  // PD11 = right motor backward
  // PA9 = left motor forward
  // PA10 = left motor backward

  // GPIOD Clock Enable
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;

  // right motors
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD,&GPIO_InitStructure);

  // left motors
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  stop_all_motors();

}
void stop_all_motors() {
  // let's get HIGH
  GPIO_SetBits(GPIOA, GPIO_Pin_9);
  GPIO_SetBits(GPIOA, GPIO_Pin_10);
  GPIO_SetBits(GPIOD, GPIO_Pin_11);
  GPIO_SetBits(GPIOD, GPIO_Pin_10);
}
void left_motor_forward() {
  GPIO_SetBits(GPIOA, GPIO_Pin_10);
  GPIO_ResetBits(GPIOA, GPIO_Pin_9);
}
void right_motor_forward(void) {
  GPIO_ResetBits(GPIOD, GPIO_Pin_11);
  GPIO_SetBits(GPIOD, GPIO_Pin_10);
}
void left_motor_backward(void) {
  GPIO_SetBits(GPIOA, GPIO_Pin_9);
  GPIO_ResetBits(GPIOA, GPIO_Pin_10);
}
void right_motor_backward(void) {
  GPIO_ResetBits(GPIOD, GPIO_Pin_10);
  GPIO_SetBits(GPIOD, GPIO_Pin_11);
}
