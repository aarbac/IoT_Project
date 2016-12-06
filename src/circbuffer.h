#ifndef HEADER_CIRCBUFFER
#define HEADER_CIRCBUFFER

//Circular Buffer Instantiation
#define CIRCBUF_DEF(x,y) extern uint8_t x_space[y]; circBuf_t x = { x_space,0,0,y}

typedef struct
{
    uint8_t * const buffer;
    int head;
    int tail;
    const int maxLen;
}circBuf_t;

int circBufPush(circBuf_t *c, uint8_t data);
int circBufPop(circBuf_t *c, uint8_t *data);
void circBufPrint(circBuf_t *c);

#endif
