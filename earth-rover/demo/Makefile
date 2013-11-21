TEMPLATEROOT = ..

# compilation flags for gdb

CFLAGS  = -O1 -g
ASFLAGS = -g 
LDLIBS = -lm

# object files

OBJS=  $(STARTUP) stm32f30x_misc.o stm32f30x_usart.o stm32f30x_gpio.o
OBJS+= stm32f30x_rcc.o stm32f30x_rtc.o stm32f30x_pwr.o stm32f30x_tim.o stm32f30x_i2c.o
OBJS+= ds_systick.o queue.o newlib_stubs.o ds_delay.o main.o ds_uart.o ds_wifly.o motors.o rover.o
OBJS+= ds_rtc.o ds_accel.o ds_mag.o ds_i2c.o ds_compass.o ds_gps.o

# include common make file

include $(TEMPLATEROOT)/Makefile.common


