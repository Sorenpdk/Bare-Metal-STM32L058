#include <stdio.h>
#include <stdlib.h> // malloc
#include <stdint.h> // uint8_t

#define BUF_SIZE 8


typedef struct
{
  uint8_t head;
  uint8_t tail;
  size_t capacity;
  uint8_t* buffer;
  
}circularBuf_t;

typedef circularBuf_t* cbuf_handle_t;

cbuf_handle_t circularBuf_init(uint8_t* buffer, size_t size);
void circular_buf_reset(cbuf_handle_t cbuf);

int main()
{
    uint8_t* buffer  = malloc(BUF_SIZE * sizeof(uint8_t));
    
    cbuf_handle_t cbuf = circularBuf_init(buffer, BUF_SIZE);

    printf("%lu\r\n", cbuf->capacity);
    printf("%d\r\n", cbuf->head);
    // free buffer free cbuf
    return 0;
}


cbuf_handle_t circularBuf_init(uint8_t* buffer, size_t size)
{
    cbuf_handle_t cbuf = malloc(sizeof(circularBuf_t));

    if(cbuf == NULL)
    {
        // Allocation failed.. do something
    }
    
    cbuf->buffer = buffer;
    cbuf->capacity = size;
    circular_buf_reset(cbuf);
    
	return cbuf;
}

void circular_buf_reset(cbuf_handle_t cbuf)
{
    cbuf->head = 0;
    cbuf->tail = 0;
}