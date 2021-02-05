#include <stdio.h>
#include <stdlib.h> // malloc
#include <stdint.h> // uint8_t
#include <string.h> // memset
#include <stdbool.h> 

/**************************************************************************
 *                      Private defines
 * ************************************************************************/

#define BUFFER_SIZE 8
#define DEBUG 0

/**************************************************************************
 *                      Private typedefines
 * ************************************************************************/

typedef struct
{
  uint8_t head;
  uint8_t tail;
  uint8_t bufferSize;
  uint8_t bufferCapacity;
  bool bufferFull;
  uint8_t* buffer;
  
}circularBuf_t;

typedef circularBuf_t* cbuf_handle_t;

/**************************************************************************
 *                      Function prototypes
 * ************************************************************************/
cbuf_handle_t circular_buf_init(uint8_t* extBuffer, size_t extBufferSize);
void circular_buf_reset(cbuf_handle_t cbuf);
void circular_buf_free(cbuf_handle_t cbuf, uint8_t* extBuffer);
void circular_buf_print_buffer(cbuf_handle_t cbuf);
uint8_t circular_buf_write(cbuf_handle_t cbuf, uint8_t *data, size_t length);
uint8_t circular_buf_read(cbuf_handle_t cbuf, uint8_t *data, size_t length);
void testprogram1(cbuf_handle_t cbuf);

/**************************************************************************
 *                      Main application
 * ************************************************************************/

/*
 0) Evaluate what you need for buffer size, head/tail size etc.
 1) Make an external buffer, of BUFFER_SIZE
 2) Create the circular buffer by calling init. and call the functions via the return cbuf_handle_t
 3) Remember to destroy the circular buffer and external buffer.
*/

int main()
{
    
    uint8_t* buffer  = calloc(BUFFER_SIZE, sizeof *buffer);
    
    if(buffer == NULL)
    {
        #if DEBUG
     printf("=========  DEBUG  =======\r\n");
     printf("Buffer failed to allocate in");
     printf (" %s", __FUNCTION__);
     printf("\r\n");
    #endif  
        
    }
    cbuf_handle_t cbuf = circular_buf_init(buffer, BUFFER_SIZE);
    
    circular_buf_free(cbuf, buffer);
    return 0;
}


/**************************************************************************
 *                      Functions
 * ************************************************************************/

cbuf_handle_t circular_buf_init(uint8_t* extBuffer, size_t extBufferSize)
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
    memset(extBuffer,0,extBufferSize);
    cbuf->buffer = extBuffer;
    cbuf->bufferSize = extBufferSize;
    cbuf->bufferCapacity = extBufferSize;
   
 
    circular_buf_reset(cbuf);
    
	return cbuf;
}

void circular_buf_reset(cbuf_handle_t cbuf)
{
    cbuf->head = 0;
    cbuf->tail = 0;
    cbuf->bufferFull = false;

    #if DEBUG
     printf("=========  DEBUG  =======\r\n");
     printf("Buffer has been reset by");
     printf (" %s", __FUNCTION__);
     printf("\r\n");
    #endif 
}

void circular_buf_free(cbuf_handle_t cbuf, uint8_t* extBuffer)
{
 free(extBuffer);
 free(cbuf);
 
  #if DEBUG
     printf("=========  DEBUG  =======\r\n");
     printf("Buffer and cbuf has been destroyed by");
     printf (" %s", __FUNCTION__);
     printf("\r\n");
    #endif 
}

uint8_t circular_buf_write(cbuf_handle_t cbuf, uint8_t *data, size_t length)
{
    if(cbuf->bufferFull)
    {
        return 0;
    }
    else
    {
        for(uint8_t i = 0; i < length; i++)
        {
            memcpy(&cbuf->buffer[cbuf->head], &data[i], 1);
            cbuf->head = (cbuf->head + 1) % cbuf->bufferSize;
            cbuf->bufferCapacity = (cbuf->bufferCapacity - 1) % cbuf->bufferSize;
                if((cbuf->head == cbuf->tail || cbuf->head == cbuf->bufferSize) && cbuf->bufferCapacity == 0)
                    {
                        cbuf->bufferFull = true;
                    }
        }  
    }
    
    return 1;
}

uint8_t circular_buf_read(cbuf_handle_t cbuf, uint8_t *data, size_t length)
{
    if(cbuf->bufferCapacity > 0 && cbuf->head == 0 && cbuf->tail == 0 && !cbuf->bufferFull)
    {
        return 0;
    }
    else
    {
        for(uint8_t i = 0; i < length; i++)
        {
            memcpy(&data[cbuf->tail], &cbuf->buffer[cbuf->tail], 1);
            memset(&cbuf->buffer[cbuf->tail], 0, 1);
            cbuf->tail = (cbuf->tail + 1) % cbuf->bufferSize;
            
            if(cbuf->head > 0 && cbuf->tail > 0 && cbuf->head == cbuf->tail)
            {
                cbuf->bufferCapacity = cbuf->bufferSize;
            }
            else
            {
                cbuf->bufferCapacity = (cbuf->bufferCapacity + 1) % cbuf->bufferSize;
                if(cbuf->bufferCapacity > 0)
                    {
                        cbuf->bufferFull = false;
                    } 
            }
        }
    }
    
    return 1;
}



/**************************************************************************
 *                      Test functions
 * ************************************************************************/


void circular_buf_print_buffer(cbuf_handle_t cbuf)
{
    printf("\r\n");
    printf("Head position %d\r\n", cbuf->head);
    printf("Tail position %d\r\n", cbuf->tail);
    printf("Buffer capacity %d\r\n", cbuf->bufferCapacity);
    printf("Buffer full? %d\r\n", cbuf->bufferFull);
    printf("\r\n");
}


void testprogram1(cbuf_handle_t cbuf)
{
    
    static uint8_t txbuffer[BUFFER_SIZE] = {1,2,3,4,5,6,7,8};
    static uint8_t txbuffer2[BUFFER_SIZE] = {7,8};
    static uint8_t rxbuffer[BUFFER_SIZE];

    
    circular_buf_write(cbuf, txbuffer, 8);
    
    
    printf("print internal buffer after writing to it\r\n");
    for(uint8_t i = 0; i < BUFFER_SIZE; i++)
    {
        printf("%d\t",cbuf->buffer[i]);
    }
    printf("\r\n");
    circular_buf_print_buffer(cbuf);
    printf("\r\n");
    
    circular_buf_read(cbuf, rxbuffer, 2);
    
    printf("print external buffer after reading from internal buffer\r\n");
    for(uint8_t i = 0; i < BUFFER_SIZE ; i++)
    {
        printf("%d\t", rxbuffer[i]);
    }
     printf("\r\n");
     
    printf("print internal buffer after reading to check if zeroed\r\n");
   for(uint8_t i = 0; i < BUFFER_SIZE; i++)
    {
        printf("%d\t",cbuf->buffer[i]);
    }
    printf("\r\n");
   
   
    circular_buf_print_buffer(cbuf);
   
   
    circular_buf_write(cbuf, txbuffer2, 2);
    
    printf("print internal buffer after writing new values from a new array\r\n");
    for(uint8_t i = 0; i < BUFFER_SIZE; i++)
    {
        printf("%d\t",cbuf->buffer[i]);
    }

    circular_buf_print_buffer(cbuf);
  
     printf("\r\n");
 
    circular_buf_read(cbuf, rxbuffer, 1);
    
    printf("print external buffer after reading values from internal buffer\r\n");
    for(uint8_t i = 0; i < BUFFER_SIZE ; i++)
    {
        printf("%d\t", rxbuffer[i]);
    }
    printf("\r\n");
     
    printf("print internal buffer after reading to check if zeroed\r\n");
   for(uint8_t i = 0; i < BUFFER_SIZE; i++)
    {
        printf("%d\t",cbuf->buffer[i]);
    }
    
    // make sure tail has caught up to head.
    circular_buf_print_buffer(cbuf);
  
  
}