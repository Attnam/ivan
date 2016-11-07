/*
    stack: generic c stack and fifo list routines
    Copyright (c) 2004-2016 Adrian M. Gin

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License v2 as published by
    the Free Software Foundation;

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

*/


#include <stdint.h>
#include <stdlib.h>
#include "audio_stack.h"


//Retrieves the data but doesn't remove it from the queue.
void* FIFO_PeekData(STACK_t* stack)
{
    //buffer not empty?
    if (stack->readPtr != stack->writePtr)
    {
        //get byte from buffer, update read position and return
        return stack->memPtrArray[stack->readPtr];
    } else
    {
        return STACK_OVERFLOW; /* This is really trying to get a nonexistant byte */
    }

}


/* Critical means disable interrupts on entry and restore interrupt
 * state on exit */
void* FIFO_PopData(STACK_t* stack)
{

    void* ptr = FIFO_PeekData(stack);
    stack->readPtr = (stack->readPtr + 1) & (stack->size - 1);
    return ptr;
}


// LIFO is like reading from the writePtr
void* LIFO_PopData(STACK_t* stack)
{
    uint8_t bufferMask = (stack->size - 1);

    //buffer not empty?
    if (stack->writePtr != stack->readPtr)
    {
        void* ptr;
        ptr = stack->memPtrArray[(stack->writePtr - 1) & bufferMask];
        //get byte from buffer, update read position and return
        stack->writePtr = (stack->writePtr - 1) & bufferMask;
        return ptr;
    } else
    {
        return STACK_OVERFLOW; /* This is really trying to get a nonexistant byte */
    }
}

/* returns the size of the stack */
uint8_t STACK_PushData(STACK_t* stack, void* data)
{
    //is there space in the buffer?
    uint8_t bufferMask = (stack->size - 1);

    if ( ((stack->writePtr + 1) & bufferMask) != (stack->readPtr & bufferMask))
    {
        //wrap around write position
        //write the character
        stack->memPtrArray[stack->writePtr] = data;
        //update size info
        stack->writePtr = (stack->writePtr + 1) & bufferMask;
        return STACK_Len(stack);
    }

    return 0;
}

uint8_t STACK_Len(STACK_t* stack)
{
    if( stack->readPtr > stack->writePtr )
    {
        return stack->writePtr + stack->size - stack->readPtr;
    }
    return stack->writePtr - stack->readPtr;
}

uint8_t STACK_isEmpty(STACK_t* stack)
{
   return stack->writePtr == stack->readPtr;
}


void STACK_Clear(STACK_t* stack)
{
    stack->readPtr = 0;
    stack->writePtr = 0;
}
