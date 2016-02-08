#include "cawax_datastructs.h"
#include <stdlib.h>
#include <stdio.h>

int main(void) {
	
	Sample * sample = (Sample *) malloc(sizeof(Sample));
	sample->x = 0.1;
	sample->y = 0.2;
	sample->z = 0.3;
	LinkedList * list = makeList();
	printf("APPENDING TEST...\n");
	for (int i = 0; i < 100; i++) {
		add(*sample, list);
	}
	toStringList(list);
	printf("\n");
	printf("SANDWICHING TEST...\n");
	for (int i = 0; i < 100; i++) {
		addI(i, *sample, list);
	}
	toStringList(list);

	

	
	puts("Finished.");
	getchar();

}