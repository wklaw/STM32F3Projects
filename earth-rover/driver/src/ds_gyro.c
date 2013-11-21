/* ds_gyro.c --- 
 *
 * Filename: ds_gyro.c
 * Description: 
 * Author: 
 * Maintainer: 
 * Created: Fri Jan 25 08:34:50 2013
 * Last-Updated: 
 *           By: 
 *     Update #: 0
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

#include <stm32f30x.h>
#include <stm32f30x_rcc.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_spi.h>
#include <ds_gyro.h>


/* Code: */


int isButtonPressed() {
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_SET;
}



void ds_gyro_interface_init(void) {
    // GPIOC Clock Enable
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOE, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 , ENABLE);

    // MOSI, MISO, SCK init
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    // chip select init
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOE,&GPIO_InitStructure);

    // Alternate Function Configuration
    GPIO_PinAFConfig(GPIOA,5,GPIO_AF_5);
    GPIO_PinAFConfig(GPIOA,6,GPIO_AF_5);
    GPIO_PinAFConfig(GPIOA,7,GPIO_AF_5);

    SPI_InitTypeDef SPI_InitStructure;

    // SPI1 init
    SPI_I2S_DeInit(SPI1);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_Init(SPI1, &SPI_InitStructure);
    SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);
    SPI_Cmd(SPI1, ENABLE);

    
}
// calls interface initialization from above, and then
// writes to control registers 1 and 4
void ds_gyro_init(void) {
    ds_gyro_interface_init();
    // CTRL1 register
    // bit 7-6 datarate: 0
    // bit 5-4 bandwidth: 3
    // bit 3 power mode: active
    // bit 2-0 axes enabled: x, y, z enabled
    uint8_t ctrl1 = 0x3f;
    // CTRL4 register
    // bit 7 block update: continuous
    // bit 6 endianness: lsb first
    // bit 5-4 full scale: 500 dps
    uint8_t ctrl4 = 0x10;
    uint8_t * p1 = &ctrl1;
    uint8_t * p4 = &ctrl4;
    ds_gyro_write(p1, (uint8_t) 0x20, (uint16_t) 0x01);
    ds_gyro_write(p4, (uint8_t) 0x23, (uint16_t) 0x01);
}

// pBuffer is what we're reading into;
// ReadAddr contains the read address and the read/write and single/multiple bits;
// NumByteToRead tells us how many bytes we're trying to read
void ds_gyro_read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead) {
    if (NumByteToRead > 0x01) {
          ReadAddr |= (uint8_t)(0x80 | 0x40); // If sending more that one byte set multibyte commands
    }
    else {
          ReadAddr |= (uint8_t) (0x80); // Else just set the read mode 
    }
    GYRO_CS_LOW();
    ds_gyro_sendbyte(ReadAddr);

    while(NumByteToRead > 0x00) {
        *pBuffer = ds_gyro_sendbyte(((uint8_t)0x00));
        NumByteToRead--;
        pBuffer++;
    }
    GYRO_CS_HIGH();
}

// pBuffer is what we're writing out of;
// ReadAddr contains the read address and the read/write and single/multiple bits;
// NumByteToWrite tells us how many bytes we're trying to write
void ds_gyro_write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite) {
    if (NumByteToWrite > 0x01) {
        WriteAddr |= (uint8_t) 0x40;
    }
    GYRO_CS_LOW();
    ds_gyro_sendbyte(WriteAddr);

    while (NumByteToWrite > 0x00) {
        ds_gyro_sendbyte(*(pBuffer++));
        NumByteToWrite--;
    }
    GYRO_CS_HIGH();
}
void ds_gyro_getdata(float *pfData) {
    uint8_t tmpbuffer[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    int16_t RawData[3];
    ds_gyro_read(tmpbuffer,(uint8_t)0x28,(uint16_t)6);
    int i;
    for (i = 0; i < 3; i++) {
        RawData[i] = (int16_t) (((uint16_t)tmpbuffer[2*i+1] << 8) + tmpbuffer[2*i]);
        pfData[i] = (float) RawData[i]/GYRO_Sensitivity_500dps;
    }
}
float getX(float *pfData) {
    return *pfData;
}
float getY(float *pfData) {
    return *(pfData + 1);
}
float getZ(float *pfData) {
    return *(pfData + 2);
}

static uint8_t ds_gyro_sendbyte(uint8_t byte) {
    // FIXME: this is where the problem is
    // Everything hangs here, and we can't figure out why.
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); 
    SPI_SendData8(SPI1, byte);

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    return (uint8_t)SPI_ReceiveData8(SPI1);
}


/* ds_gyro.c ends here */
