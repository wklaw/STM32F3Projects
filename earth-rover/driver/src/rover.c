#include <rover.h>

void parse_command(char *buffa, int buffa_length) {
  int i;
  for (i=0;buffa[i]==' '||buffa[i]=='\n'||buffa[i]=='\r'&&i<buffa_length;i++);
  if (buffa[i] == 0||i==buffa_length) return;
  switch (buffa[i]) {
    case ('f'):
      printf("hi");
      left_motor_forward();
      right_motor_forward();
      return;
    case('b'):
      left_motor_backward();
      right_motor_backward();
      return;
    case('l'):
      left_motor_forward();
      right_motor_backward();
      return;
    case('r'):
      right_motor_forward();
      left_motor_backward();
      return;
  }
}
void parse_two(char *buffa, int buffa_length) {
  int i;
  /*
  for (i=0;buffa[i]==' '||buffa[i]=='\n'||buffa[i]=='\r'&&i<buffa_length;i++);
  if (buffa[i] == 0 || i==buffa_length) return;
  */
  //printf("dude\n");
  if (buffa[0] != '@') return;
  switch (buffa[1]) {
    case ('f'):
      //printf("hi\n");
      left_motor_forward();
      right_motor_forward();
      return;
    case('b'):
      //printf("bye\n");
      left_motor_backward();
      right_motor_backward();
      return;
    case('l'):
      //printf("why\n");
      left_motor_forward();
      right_motor_backward();
      return;
    case('r'):
      //printf("try\n");
      right_motor_forward();
      left_motor_backward();
      return;
    case('s'):
      stop_all_motors();
      return;
  }
}
void make_json(char *buffa, char *gps, float tiltHeading, int hours, int minutes, int seconds) {
  sprintf(buffa, "{\\\"gps\\\": \\\"%s\\\", \\\"direction\\\": \\\"%3.3f\\\", \\\"time\\\": %2d:%2d:%2d}", gps, tiltHeading, hours, minutes, seconds);
  /*
  int i = 0;
  int j;
  for (j = 0; STR1[j] != 0; j++, i++) {
    buffa[i] = STR1[j];
  }
  for (j = 0; gps[j] != 0; j++, i++) {
    buffa[i] = gps[j];
  }
  for (j = 0; STR2[j] != 0; j++, i++) {
    buffa[i] = STR1[j];
  }
  for (j = 0; direction[j] != 0; j++, i++) {
    buffa[i] = direction[j];
  }
  for (j = 0; STR3[j] != 0; j++, i++) {
    buffa[i] = STR1[j];
  }
  for (j = 0; time[j] != 0; j++, i++) {
    buffa[i] = time[j];
  }
  buffa[i] = '}';
  */
}
