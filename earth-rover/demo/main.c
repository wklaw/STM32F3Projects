/* main.c ---
 *
 * Filename: main.c
 * Description:
 * Author:
 * Maintainer:
 * Created:
 * Last-Updated:
 *                         By:
 *             Update #: 0
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
#include <ds_systick.h>
#include <ds_uart.h>
#include <ds_rtc.h>
#include <ds_wifly.h>
#include <ds_compass.h>
#include <rover.h>
#include <motors.h>
#include <ds_i2c.h>
#include <ds_gps.h>

extern volatile int rover_flag = 0;

char *line;

void scheduler(void);
int search_buffer(char *overlord, int overlord_length);
void buffer_reset(char * overlord, int overlord_length);
void parse_command(char *buffa, int overlord_length);

char buffa[QUEUE_SIZE];
char overlord[QUEUE_SIZE];
int SENDING = 1;
int nth_time = 0;
int mth_time = 0;
char gps[GPS_SENTENCE_SIZE];
char *direction;
int time;
RTC_TimeTypeDef RTC_TimeMain;
int startMinutes, startSeconds, startHours;
int minutes, seconds, hours;
float tiltHeading, accel_data[3], magneto_data[3];
int times_without;

int main(void) {

  initialize_wifly();
  initialize_usart();
  ds_systick_init();
  ds_delay_init();
  ds_rtc_init();
  motors_init();
  ds_i2c1_init();
  ds_compass_init();
  gps_initialise();

  init_queue(&wifly_rxbuf);
  init_queue(&wifly_txbuf);
  init_queue(&rxbuf);
  init_queue(&txbuf);

  buffa[0] = 0;

  direction = "80degW";
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeMain);
  startHours = RTC_TimeMain.RTC_Hours;
  startMinutes = RTC_TimeMain.RTC_Minutes;
  startSeconds = RTC_TimeMain.RTC_Seconds;
  times_without = 0;


  while(1) {
    
    if (systick_flag) {
      systick_flag = 0;
      scheduler();
    }

  }

}

void scheduler(void) {

  static int counter_100ms = 0;
  if(counter_100ms++ == 10) {
    counter_100ms = 0;
  }
  


  nonBlocking_getline(buffa, QUEUE_SIZE);
  if (buffa[0] != 0) {
    wifly_sendstring(buffa);
    buffer_reset(buffa, QUEUE_SIZE);
  }
  if (nth_time++ == 50) {
    gps_getSentence(gps, "$GPGGA");
    int i;
    for(i=0;gps[i];i++)if(gps[i]=='\n'||gps[i]=='\r')gps[i]=' ';
    tiltHeading = getTiltHeading(accel_data, magneto_data);
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeMain);
    hours = RTC_TimeMain.RTC_Hours - startHours;
    minutes = RTC_TimeMain.RTC_Minutes - startMinutes;
    seconds = RTC_TimeMain.RTC_Seconds - startSeconds;
    make_json(buffa, gps, tiltHeading, hours, minutes, seconds);
    printf("%s\n", buffa);
    wifly_sendstring(buffa);
    buffer_reset(buffa, QUEUE_SIZE);
    nth_time = 0;
  }

  wifly_getline_noblock(buffa, QUEUE_SIZE);
  if (buffa[0] != 0) {
    printf("%s\n", buffa);
    parse_two(buffa, QUEUE_SIZE);
    buffer_reset(buffa, QUEUE_SIZE);
    times_without = 0;
  }
 
  /*

  if (nth_time++ == 200) {
    ds_delay_mS(250);

    wifly_sendstring("$$$");
    ds_delay_mS(250);
  }
  if (nth_time++ == 201) {
    wifly_sendstring("exit\n");
    ds_delay_mS(100);
    nth_time = 0;
    ds_delay_mS(250);
    wifly_sendstring("test data\n");
    ds_delay_mS(250);
  }
    //wifly_sendstring("this is an UPDATE\n");
  */
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif

int search_buffer(char *overlord, int overlord_length) {
  int i = 0;
  int isThere = 0;
  while(i < overlord_length && overlord[i] != 0) {
    if(overlord[i] == '\r' || overlord[i] == '\n') {
      isThere = 1;
    }
    i++;
  }

  return isThere;
}

void buffer_reset(char *overlord, int overlord_length) {
  memset(overlord, 0, overlord_length);
}
