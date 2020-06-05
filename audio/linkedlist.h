/*
    linkedlist: generic c linked list routines
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

#ifdef __cplusplus    /* Insert start of extern C construct */
extern "C" {
#endif

#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H

#include <stdint.h>
#include <stdlib.h>

typedef struct LIST_NODE_t
{
  struct LIST_NODE_t* next;
  struct LIST_NODE_t* prev;
   void* data;
} LIST_NODE_t;

typedef struct
{
   LIST_NODE_t* first;
   LIST_NODE_t* last;
} LINKED_LIST_t;

void LL_DeleteList(LINKED_LIST_t* linkedList);
void LL_DeleteListAndData(LINKED_LIST_t* linkedList);

LIST_NODE_t* LL_NewNode(void* data);
void* LL_PopData(LINKED_LIST_t* linkedList);
void LL_AppendData(LINKED_LIST_t* linkedList, void* data);
void LL_InsertAfter(LINKED_LIST_t* linkedList, LIST_NODE_t* node, LIST_NODE_t* newNode);
void LL_InsertBefore(LINKED_LIST_t* linkedList, LIST_NODE_t* node, LIST_NODE_t* newNode);
void LL_InsertBeginning(LINKED_LIST_t* linkedList, LIST_NODE_t* newNode);
void LL_InsertEnd(LINKED_LIST_t* linkedList, LIST_NODE_t* newNode);
void LL_Remove(LINKED_LIST_t* linkedList, LIST_NODE_t* node);
uint16_t LL_Count(LINKED_LIST_t* linkedList);
LIST_NODE_t* LL_ReturnNodeFromIndex(LINKED_LIST_t* linkedList, uint16_t item);
void* LL_ReturnNodeDataFromIndex(LINKED_LIST_t* linkedList, uint16_t item);


//For FreeRTOS
//#define LL_Malloc(size)    pvPortMalloc(size)
//#define LL_Free(handle)    vPortFree(handle)

#define LL_Malloc(size)    malloc(size)
#define LL_Free(handle)    free(handle)

#endif

#ifdef __cplusplus  /* Insert end of extern C construct. */
}                   /* The C header file can now be */
#endif              /* included in either C or C++ code. */
