

#ifndef __STACK_H
#define __STACK_H

#include <stdint.h>

#define STACK_OVERFLOW	(NULL)

/* Size has to be a size of power of 2 */
typedef struct
{
    void** memPtrArray; //memPtrsArray = uint8_t array[sizeof(void*) * 10];
    volatile uint16_t readPtr;
    volatile uint16_t writePtr;
    volatile uint8_t size;
} STACK_t;

void* FIFO_PeekData(STACK_t* stack);
void* LIFO_PopData(STACK_t* stack);
void* FIFO_PopData(STACK_t* stack);
uint8_t STACK_PushData(STACK_t* stack, void* data);

uint8_t STACK_Len(STACK_t* stack);
uint8_t STACK_isEmpty(STACK_t* stack);
void    STACK_Clear(STACK_t* stack);


#endif


