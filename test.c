#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include "vector.h"

bool vAppend_test() {
	Vector vector = vNew();
	vAppend(vector, (void*)1);
	vAppend(vector, (void*)2);
	vAppend(vector, (void*)3);
	printf("\nexpecting 3 %p\n", vGet(2, vector));
	vRemove(1, vector);
	printf("\nexpecting 3 %p\n", vGet(1, vector));
	printf("\n%d\n", vLen(vector));
	vInsert(1, vector, (void*)2);
	printf("\nexpecting 2 %p\n", vGet(1, vector));
	printf("\n%d\n", vLen(vector));
	vDelete(vector);
	return true;
}

int main(int arc, char** argv) {
	printf("%d", vAppend_test());
	//system("./calc < program.txt > temp_o.txt");
}