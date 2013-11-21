#include <stm32f30x.h>
#include <ds_accel.h>
#include <ds_i2c.h>

void ds_accel_init() {

    uint8_t ctrl1 = 0x47; //address 0x20
    uint8_t ctrl2 = 0x90; //address 0x21
    uint8_t ctrl4 = 0x08; // address 0x23

    ds_i2c1_write(accel, 0x20, &ctrl1);
    ds_i2c1_write(accel, 0x21, &ctrl2);
    ds_i2c1_write(accel, 0x23, &ctrl4);

}
void ds_accel_read(float * accel_data) {

    int16_t raw_data[3];
    uint8_t buffer[6];

    ds_i2c1_read(accel, 0x28, buffer, 6);

    int i;
    for (i=0; i<3; i++) {
        raw_data[i]=((int16_t)((uint16_t)buffer[2*i+1] << 8) + buffer[2*i])/(uint8_t)16;
        accel_data[i]=(float)raw_data[i]/1000.0;
    }


}


