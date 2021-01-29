#include <stdio.h>
#include <stdlib.h> // malloc
#include <stdint.h> // uint8_t

#define BUF_SIZE 8
#define DEBUG 0
typedef struct
{
  uint8_t head;
  uint8_t tail;
  size_t capacity;
  uint8_t isFull;
  uint8_t* buffer;
  
}circularBuf_t;



typedef circularBuf_t* cbuf_handle_t;

cbuf_handle_t circular_buf_init(uint8_t* buffer, size_t size);
void circular_buf_reset(cbuf_handle_t cbuf);
void circular_buf_free(cbuf_handle_t cbuf, uint8_t* buffer);
void circular_buf_print(cbuf_handle_t cbuf);
void circular_buf_isFull(cbuf_handle_t cbuf);

int main()
{
    
    uint8_t* buffer  = calloc(BUF_SIZE, sizeof *buffer);
    if(buffer == NULL)
    {
        #if DEBUG
     printf("=========  DEBUG  =======\r\n");
     printf("Buffer failed to allocate in");
     printf (" %s", __FUNCTION__);
     printf("\r\n");
    #endif  
        
    }
    
    cbuf_handle_t cbuf = circular_buf_init(buffer, BUF_SIZE);
    
    circular_buf_print(cbuf);
    
    circular_buf_free(cbuf, buffer);
    
    return 0;
}


cbuf_handle_t circular_buf_init(uint8_t* buffer, size_t size)
{
    cbuf_handle_t cbuf = malloc(sizeof(circularBuf_t));

    if(cbuf == NULL)
    {
      #if DEBUG
     printf("=========  DEBUG  =======\r\n");
     printf("Circular buffer failed to allocate in");
     printf (" %s", __FUNCTION__);
     printf("\r\n");
    #endif  
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
    cbuf->isFull = 0;
    
    #if DEBUG
     printf("=========  DEBUG  =======\r\n");
     printf("Buffer has been reset by");
     printf (" %s", __FUNCTION__);
     printf("\r\n");
    #endif 
}

void circular_buf_free(cbuf_handle_t cbuf, uint8_t *buffer)
{
 free(buffer);
 free(cbuf);
 
  #if DEBUG
     printf("=========  DEBUG  =======\r\n");
     printf("Buffer and cbuf has been destroyed by");
     printf (" %s", __FUNCTION__);
     printf("\r\n");
    #endif 
}

void circular_buf_print(cbuf_handle_t cbuf)
{
   printf("Buffer capacity: %lu\r\n", cbuf->capacity);
   printf("Head value: %d\r\n", cbuf->head);
   printf("Tail value: %d\r\n", cbuf->tail);
   printf("Buffer status, isFull: %d", cbuf->isFull);
   
}

// Whats better to check if empty or !full ??
void circular_buf_isFull(cbuf_handle_t cbuf)
{
    
    if((cbuf->capacity == 0) && cbuf->head == cbuf->tail)
    {
        cbuf->isFull = 1;
    }
    
    else
    {
        cbuf->isFull = 0;
    }
}