/* ds_gps.h ---
 *
 * Filename: ds_gps.h
 * Description:
 * Author: WILFRED
 * Maintainer:
 * Created:
 * Last-Updated:
 *                         By:
 *             Update #: 0
 * Keywords:
 * Compatibility:
 *
 */

/* Commentary: PINS!
 *             BLACK : GROUND
 *             RED: 3.3v
 *             WHITE: TX
 *
 *
 */

/* Change log:
 *
 *
 */
/* Code: */

#include <queue.h>

#ifndef _DS_GPS_H_
#define _DS_GPS_H_

#define GPS_SENTENCE_SIZE 128


queue_t gps_buffer;

void gps_initialise(void);
int gps_compare(char *gps_data, char *needed_str);
void gps_strcopy(int *original, int *tocopy);
int gps_getchar(void);
int gps_getSentence(char *destination, char *sentenceType);
void gps_getGPGGA_data(char *latitude, char *longtitude, char *altitude, char *gpsSentence);

#endif
