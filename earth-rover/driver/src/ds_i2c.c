#include <stm32f30x.h>
#include <ds_i2c.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_i2c.h>


void ds_i2c1_init() {
    // set clocks
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE); 
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOB,7,GPIO_AF_4);
    GPIO_PinAFConfig(GPIOB,6,GPIO_AF_4);

    I2C_InitTypeDef I2C_InitStructure;

    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
    I2C_InitStructure.I2C_DigitalFilter = 0x00;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_Timing = 0x00902025;
    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_Cmd(I2C1, ENABLE);



}
void ds_i2c1_read(uint8_t device, uint8_t reg, uint8_t* buffer, uint16_t numbytes) {
    
     while (I2C_GetFlagStatus(I2C1, I2C_ISR_BUSY) != RESET); 

     I2C_TransferHandling(I2C1, device, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
     while (I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS) == RESET);

     if (numbytes>1) {
           reg |=0x80;                                                                             
     }

    I2C_SendData(I2C1,reg);                    
    while (I2C_GetFlagStatus(I2C1, I2C_ISR_TC) == RESET);

    I2C_TransferHandling(I2C1, device, numbytes, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
    while (numbytes--) {
        while(I2C_GetFlagStatus(I2C1, I2C_ISR_RXNE) == RESET);
        *buffer++ = I2C_ReceiveData(I2C1);
    }

    while (I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET); 
    I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);

}
void ds_i2c1_write(uint8_t device, uint8_t reg, uint8_t* value) {

    while (I2C_GetFlagStatus(I2C1, I2C_ISR_BUSY) != RESET);

    I2C_TransferHandling(I2C1, device, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
    while (I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS) == RESET);

    I2C_SendData(I2C1,reg);
    while(I2C_GetFlagStatus(I2C1, I2C_ISR_TCR) == RESET);

    I2C_TransferHandling(I2C1, device, 1, I2C_AutoEnd_Mode, I2C_No_StartStop);
    while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS) == RESET);

    I2C_SendData(I2C1, *value);

    while(I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET);
    I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);


}


