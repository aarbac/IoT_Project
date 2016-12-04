#include <stdbool.h>
#include <stdint.h>

#include "circbuffer.h"

int circBufPush(circBuf_t *c, uint8_t data)
{
    int next = c->head + 1;
    if (next >= c->maxLen)
        next = 0;

    // Circular buffer is full
    if (next == c->tail)
        return -1;  // quit with an error

    c->buffer[c->head] = data;
    c->head = next;
    return 0;
}

int circBufPop(circBuf_t *c, uint8_t *data)
{
    // if the head isn't ahead of the tail, we don't have any characters
    if (c->head == c->tail)
        return -1;  // quit with an error

    *data = c->buffer[c->tail];
    c->buffer[c->tail] = 0;  // clear the data (optional)

    int next = c->tail + 1;
    if(next >= c->maxLen)
        next = 0;

    c->tail = next;

    return 0;
}

void circBufPrint(circBuf_t *c)
{
    int aux, aux1;
    aux  = c->head;
    aux1 = c->maxLen;
    while(aux1>0)
    {
        printf("Element #%d = %d\n", aux, c->buffer[aux]);
        aux=(aux+1)%c->maxLen;
        aux1--;
    }
    return;
}
