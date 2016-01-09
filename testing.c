// Deep Shah, Shashank Seeram
//
//

#include "malloc.h"
#include <stdlib.h>
#include <stdio.h>
#define malloc(x) myMalloc(x, __FILE__ , __LINE__)
#define free(x) myFree(x, __FILE__, __LINE__)

void non_alloc_free() {
	printf("\n=== Start non-allocated pointer free test ===\n\n");
	printf("Expected output: failed to free non allocated pointer\n\n");
	int p;
	free(&p);
	printf("\n=== End non-allocated pointer free test ===");
	printf("\n\n");
}

void non_malloc_free() {
	printf("\n=== Start non-malloc'd pointer free test ===\n\n");
	printf("Expected output: successful malloc, failed to free\n\n");
	char *ptr = (char *)malloc(100);
	free(ptr + 50);
	printf("\n=== End non-malloc'd pointer free test ===");
	printf("\n\n");
}

void double_malloc_free() {
	printf("\n=== Start double malloc free test ===\n\n");
	printf("Expected output: successful malloc, successful free, failed to free\n\n");
	char *ptr = (char *)malloc(500);
	free(ptr);
	free(ptr);
	printf("\n=== End double malloc free test ===");
	printf("\n\n");
}

void valid_malloc() {
	printf("\n=== Start valid malloc test ===\n\n");
	printf("Expected output: successful malloc, successful free, successful malloc, successful free\n\n");
	char * ptr = (char *)malloc(200); 
	free(ptr);
	ptr = (char *)malloc(200); 
	free(ptr);
	printf("\n=== End valid malloc test ===");
	printf("\n\n");
}


int main(int argc, char const *argv[]) {
	non_alloc_free();
	non_malloc_free();
	double_malloc_free();
	valid_malloc();
	return 0;
}
