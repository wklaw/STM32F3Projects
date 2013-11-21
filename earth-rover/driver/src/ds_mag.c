#include <stm32f30x.h>
#include <ds_mag.h>
#include <ds_i2c.h>

void ds_mag_init() {

    uint8_t CRA = 0x14; //Address 0x00
    uint8_t CRB = 0xE0; //Address 0x01
    uint8_t MR = 0x00; //Address 0x02

    ds_i2c1_write(magneto, 0x00, &CRA);
    ds_i2c1_write(magneto, 0x01, &CRB);
    ds_i2c1_write(magneto, 0x02, &MR);

}
void ds_mag_read(float *magneto_data) {

    uint8_t buffer[6];

    ds_i2c1_read(0x3C, 0x03, buffer,2);   // Read X Axis
    ds_i2c1_read(0x3C, 0x07, buffer+2,2); // Read Y Axis
    ds_i2c1_read(0x3C, 0x05, buffer+4,2); // Read Z Axis (notice that Z is out of order in the chip).

    int i;
    //Converts buffer data for X and Y axis
    for (i=0; i<2; i++) {
        magneto_data[i]=(float)((int16_t)(((uint16_t)buffer[2*i] << 8) + buffer[2*i+1]))/230;
    }
    //Converts buffer data for Z axis
    magneto_data[2]=(float)((int16_t)(((uint16_t)buffer[4] << 8) + buffer[5]))/205;

}


