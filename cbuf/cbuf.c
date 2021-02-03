/**  
 *              * Lessons learned *
 * To loop back to the beginning of a buffer use the (X % Y) method 
 *  if X and Y is equal the result will become 0
 *  say X is our headptr which is 1 and Y is our max buffer size of 8
 *  0 mod 8 = 0
 *  1 mod 8 = 1
 *  2 mod 8 = 2
 *   ....
 * 8 mod 8 = 0
 * 9 mod 8 = 1
 * 10 mod 8 = 2
 * 
 * 
 * if you dont declare arrays static, we will have garbage values when we assign the arrays to the internal buffer
 * static will always initialize all values to zero, if you assign some values the rest will still be 0.
 * 
 * Always memset buffers unless they are static.
**/


#include <stdio.h>
#include <stdlib.h> // malloc
#include <stdint.h> // uint8_t
#include <string.h> // memset

#define BUF_SIZE 8
#define DEBUG 0

typedef struct
{
  uint8_t head;
  uint8_t tail;
  uint8_t max;
  uint8_t isFull;
  uint8_t* buffer;
  
}circularBuf_t;

typedef circularBuf_t* cbuf_handle_t;

cbuf_handle_t circular_buf_init(uint8_t* buffer, size_t size);
void circular_buf_reset(cbuf_handle_t cbuf);
void circular_buf_free(cbuf_handle_t cbuf, uint8_t* buffer);
void circular_buf_print(cbuf_handle_t cbuf);
void circular_buf_print_buffer(cbuf_handle_t cbuf, size_t len);
void circular_buf_put_byte(cbuf_handle_t cbuf, uint8_t data, size_t len);
uint8_t circular_buf_read_byte(cbuf_handle_t cbuf, uint8_t *data, size_t len);
void writetest(cbuf_handle_t cbuf);

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
    
    static uint8_t rxbuf[8];
    
    //writetest(cbuf);
   
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
    memset(buffer,0,BUF_SIZE);
    cbuf->buffer = buffer;
    cbuf->max = size;
 
    circular_buf_reset(cbuf);
    
	return cbuf;
}

void circular_buf_reset(cbuf_handle_t cbuf)
{
    cbuf->head = 0;
    cbuf->tail = 0;
    cbuf->isFull = 0;
    memset(cbuf->buffer,0,BUF_SIZE);
    
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
  
   printf("Head value: %d\r\n", cbuf->head);
   printf("Tail value: %d\r\n", cbuf->tail);
   printf("Buffer status, isFull: %d\r\n", cbuf->isFull);
   
}

void circular_buf_print_buffer(cbuf_handle_t cbuf, size_t len)
{
    printf("Transmit buffer: \r\n");
    
    for(uint8_t i = 0; i < 8; i++)
    {
        printf("%d\t", cbuf->buffer[i]); 
     
    }
   printf("\r\n");
}

void circular_buf_put_byte(cbuf_handle_t cbuf, uint8_t data, size_t len)
{
        cbuf->buffer[cbuf->head] = data;
        cbuf->head = (cbuf->head + 1) % cbuf->max;
}


uint8_t circular_buf_read_byte(cbuf_handle_t cbuf, uint8_t *data, size_t len)
{
    
    data = cbuf->buffer;
    printf("%d\r\n", *data);
    cbuf->tail = (cbuf->tail + 1) % cbuf->max;
   
  
   return *data;
}


void writetest(cbuf_handle_t cbuf)
{
    
    printf("==== Write Test ===== \r\n");
    printf("\r\n");
    uint8_t txbuf[5] = {1,2,3,4,5};
    printf("Write 5 values 1..5, headptr = 5 \r\n");
    for(uint8_t i = 0; i < 5; i++){
    circular_buf_put_byte(cbuf, txbuf[i], 1);
    }
    circular_buf_print_buffer(cbuf, 8);
    
    printf("\r\n");
    printf("Write 5 more values 9..13, headptr = 2  \r\n");
    uint8_t txxbuf[5] = {9,10,11,12,13};
    for(uint8_t i = 0; i < 5; i++){
    circular_buf_put_byte(cbuf, txxbuf[i], 1);
    }
    circular_buf_print_buffer(cbuf, 8);
    
    printf("\r\n");
    printf("Write 7 more values 21..27, headptr = 1  \r\n");
    uint8_t txxxbuf[7] = {21,22,23,24,25,26,27};
    for(uint8_t i = 0; i < 7; i++){
    circular_buf_put_byte(cbuf, txxxbuf[i], 1);
    }
    
    circular_buf_print_buffer(cbuf, 8);
    printf("\r\n");
    printf("Buffer status \r\n");
    circular_buf_print(cbuf);
}