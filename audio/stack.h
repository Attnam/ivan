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


