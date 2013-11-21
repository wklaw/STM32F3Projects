#ifndef _ROVER_H_
#define _ROVER_H_

extern volatile int rover_flag;
/*
char STR1[] = "{\"gps\": ";
char STR2[] = ", \"direction\": ";
char STR3[] = ", \"time\": ";
*/

void parse_command(char *, int);
void parse_two(char *, int);
void make_json(char *, char *, float, int, int, int);

#endif


