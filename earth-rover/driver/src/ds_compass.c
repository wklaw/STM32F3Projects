#include <stm32f30x.h>
#include <ds_mag.h>
#include <ds_i2c.h>
#include <ds_accel.h>
#include <ds_compass.h>
#include <math.h>


void ds_compass_init(void) {

  ds_mag_init();
  ds_accel_init();



}

float getPitch(float * accel_data) {
  float ax = accel_data[0];
  float ay = accel_data[1];
  float az = accel_data[2];
  return 180 * atan(ax / sqrt(ay * ay + az * az)) / PI;

}
float getRoll(float * accel_data){
  float ax = accel_data[0];
  float ay = accel_data[1];
  float az = accel_data[2];
  return 180 * atan(ay / sqrt(ax * ax + az * az)) / PI;

}
float getYaw(float * magneto_data) {
  return magneto_data[1] * 270 - 10;
}

float getTiltHeading(float *accel_data, float *magneto_data) {
  ds_accel_init();
  ds_mag_init();

  float pitch = asin(- accel_data[0]);
  float roll = asin(accel_data[1] /cos(pitch));

  float xh = magneto_data[0] * cos(pitch) + magneto_data[2] * sin(pitch);
  float yh = magneto_data[0] * sin(roll) * sin(pitch) + magneto_data[1] * cos(roll) - magneto_data[2] * sin(roll) * cos(pitch);
  float zh = -magneto_data[0] * cos(roll) * sin(pitch) + magneto_data[1] * sin(roll) + magneto_data[2] * cos(roll) * cos(pitch);

  float heading = 180 * atan2(yh, xh)/PI;
  
  if (yh >= 0)
    return heading;
  else
    return (360 + heading);

}
