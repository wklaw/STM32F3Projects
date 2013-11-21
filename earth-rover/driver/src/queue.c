#include <stdio.h>
#include "queue.h"

void init_queue(queue_t *qt) {
  qt->head = 0;
  qt->tail = 0;
  int i;
  for (i = 0; i < QUEUE_SIZE; i++) {
    qt->buffer[i] = -1;
  }
}
int enqueue(queue_t *qt, int x) {
  if (queue_full(qt)) {
    return 0;
  }
  else {
    qt->buffer[qt->tail] = x;
    if (++(qt->tail) == QUEUE_SIZE) qt->tail = 0;
    return 1;
  }
}
int dequeue(queue_t *qt) {
  if (queue_empty(qt)) {
    return 0;
  }
  else {
    int ans = qt->buffer[qt->head];
    qt->buffer[qt->head] = -1;
    if (++(qt->head) == QUEUE_SIZE) qt->head = 0;
    return ans;
  }
}
int queue_empty(queue_t *qt) {
  return qt->head == qt->tail && qt->buffer[qt->head] < 0;
}
int queue_full(queue_t *qt) {
  return qt->head == qt->tail && qt->buffer[qt->head] >= 0;
}

