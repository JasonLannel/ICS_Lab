#ifndef QUEUE_H
#define QUEUE_H

#include "mm.h"

void queue_init();
int queue_add(MMData data);
int queue_get(MMData *ret);
void queue_close();

#ifndef GRADING

#define MAX_QUEUE_LEN 2

#endif // GRADING

#endif //QUEUE_H