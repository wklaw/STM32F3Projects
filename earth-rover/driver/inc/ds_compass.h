#define PI 3.1415926535

void ds_compass_init(void);
float getPitch(float *accel_data);
float getRoll(float *accel_data);
float getYaw(float *magneto_data);
float getTiltHeading(float *accel_data, float *magneto_data);
