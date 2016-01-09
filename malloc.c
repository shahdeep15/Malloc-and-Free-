// Deep Shah, Shashank Seeram
//
//

#include "malloc.h"
#include "sorted-list.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

SortedListPtr sl;

void myFree(void * p, char * fn, int ln){
	memEntry *ptr;		//Memory Entry pointer
	memEntry *before;	//Memory Entry for the previous block
	memEntry *after;	//Memory Entry for the next block
    
	if(sl == NULL) { //Error checking
		printf("Error: No malloced memory in the list. Failed to free.\n");
		return;
	} if(SLFind(sl, p) == NULL) {
		printf("Error: This memory pointer was never malloced in the list. Failed to free.\n");
		return;
	}
    
	ptr = (memEntry *)((char *)p - sizeof(memEntry));

	if((before = ptr->prev) != 0 && before->isFree){
		before->size += sizeof(memEntry) + ptr->size;	// Combining with the previous Memory Entry free block
		before->next  = ptr->next;
		ptr->isFree   = 1;
		before->next  = ptr->next;
		if(ptr->next != 0)
		{
			ptr->next->prev = before;
		}
		SLRemove(sl, p);
	} else {   
		if (ptr->isFree == 0) {
		    SLRemove(sl, p);
		    ptr->isFree = 1;
		    before = ptr;
		    printf("Freed block 0x%x\n", p); //Successfully freed
		} else {
			printf("Error: Attempted to double free a pointer. Failed to free.\n"); //When user tries to free pointer twice
		}
	}
	if((after = ptr->next) != 0 && after->isFree){
		before->size += sizeof(memEntry) + after->size;	// Merging with the next Memory Entry free block
		before->next = after->next;
		before->isFree = 1;
		if(after->next != 0) {
			after->next->prev = before;
		}
		
		SLRemove(sl, p);
	}
}

void * myMalloc(unsigned int size, char * fn, int ln) {
	void *ret_ptr;	//Return pointer
	
	memEntry *ptr; //Memory Entry Pointer
	memEntry *after;
	
	static memEntry *head = 0;	//points to front of Memory Entry list
	static memEntry *tail = 0;	//points to back of Memory Entry list
    
	ptr = head;
	while(ptr != 0)
	{
		if(ptr->isFree == 0 || (ptr->size < size)) {	//This block is not free or is too small
			ptr = ptr->next;
		} else if(ptr->size < (size + sizeof(memEntry))) {	//This block is not big enough to cut up
			ptr->isFree = 0;
			ret_ptr = (char *)ptr + sizeof(memEntry);
			SLInsert(sl, ret_ptr); 	// Adding this block to the list of blocks.
			printf("Alloc\'d block 0x%x\n", ret_ptr);
			return ret_ptr;
		} else { // Creating the split-up block to come after our newly allocated block.
			after = (memEntry *)((char *)ptr + sizeof(memEntry) + size); //offset from where ptr was; accounts for size of a Memory Entry and the size of the block
			after->prev = ptr;
			after->next = ptr->next;
			if(ptr->next != 0){
				ptr->next->prev = after;
			}
			ptr->next = after;
			after->size = ptr->size - sizeof(memEntry) - size;
			after->isFree = 1;
			ptr->size = size;
			ptr->isFree = 0;
			if (ptr == tail) {
				tail = after;
			}
			ret_ptr = (char *)ptr + sizeof(memEntry);
			SLInsert(sl, ret_ptr); 	// Adding this block to the list of blocks.
			printf("Alloc\'d block 0x%x\n", ret_ptr);
			return ret_ptr;
		}
	}
	ptr = (memEntry *)sbrk(size + sizeof(memEntry)); //Extending the heap by (size + sizeof(memEntry) bytes if there isn't enough space in any free blocks
	if(ptr == (void *)-1) {
		printf("Error: Malloc failed in file %s at line %d\n", fn, ln);
		return 0;
	} else if(tail == 0) { // tail is null, adds first one
		ptr->prev = 0;
		ptr->next = 0;
		ptr->size = size;
		ptr->isFree = 0;
		head = ptr;
		tail = ptr;
		ret_ptr = (char *)ptr + sizeof(memEntry);
		sl = SLCreate(ptrcmp);
		SLInsert(sl, ret_ptr); 	// Adding this block to the list of blocks.
		printf("Alloc\'d block 0x%x\n", ret_ptr);
		return ret_ptr;
	} else	{ // otherwise add to existing list
		ptr->prev = tail;
		ptr->next = tail->next;
		ptr->size = size;
		ptr->isFree = 0;
		tail->next = ptr;
		tail = ptr;
		ret_ptr = (char *)ptr + sizeof(memEntry);
		SLInsert(sl, ret_ptr); 	// Adding this block to the list of blocks.
		printf("Alloc\'d block 0x%x\n", ret_ptr);
		return ret_ptr;
	}
	printf("Error: Malloc failed in file %s at line %d\n", fn, ln);
	return 0;
}
int ptrcmp(void * ptr1, void * ptr2){ //Compare pointers in the SortedList
	return ptr1 - ptr2;
}
