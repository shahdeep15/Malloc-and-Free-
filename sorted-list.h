//  Deep Shah, Shashank Seeram
//
//
	
	#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <stdlib.h>

struct node
{
  // counts the number of persistant pointers
  int ptrCounter;
  // the pointer to the data
  void *content;
  // pointer to the next node.
  struct node *next;
};
typedef struct node Node;


struct SortedList
{
  // pointer to the start of the list
  struct node *front;
  // The compare function
  int (* compareFunct) (void *, void *);
  // The destroy function
  void (* destroyFunct) (void *);
};
typedef struct SortedList* SortedListPtr;


struct SortedListIterator
{
  // pointer to the current node the iterator is on
  struct node *current;
  // pointer to the sorted list the iterator is operating on
  SortedListPtr sortedList;
};
typedef struct SortedListIterator* SortedListIteratorPtr;

typedef int (*CompareFuncT)( void *, void * );

typedef void (*DestructFuncT)( void * );

SortedListPtr SLCreate(CompareFuncT cf);

void SLDestroy(SortedListPtr list);

void *SLFind(SortedListPtr list, void *newObj);

int SLInsert(SortedListPtr list, void *newObj);

int SLRemove(SortedListPtr list, void *newObj);

#endif
