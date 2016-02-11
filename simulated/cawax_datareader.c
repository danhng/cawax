#include "cawax_datareader.h"
#include <string.h>

int countLines(const char * filename)
{
	printf("Calling countLines on %s\n", filename);
	FILE * fp = fopen(filename, "r");
	if (!fp)
	{
		char errorMessage[100];
		strcat(strcpy(errorMessage, "Error while opening file "), filename);
		perror(errorMessage);
		printf("Returning -1 for countLines on file %s.\n", filename);
		return -1;
	}
	size_t count = 0;
	char c;
	while ((c = fgetc(fp)) != EOF)
	{
		if (c == '\n')
			count++;
	}
	// seek back to the end of the file
	fseek(fp, -1, SEEK_END);
	// if last char is not an LF it means last line is EOF terminated and thus should still be count as a line.
	if (fgetc(fp) != '\n')
		count++;	
	if (ferror(fp)) {
		char errorMessage[100];
		strcat(strcpy(errorMessage, "Error while reading file "), filename);
		perror(errorMessage);
		printf("Returning -1 for countLines on file %s.\n", filename);
		return -1;
	}
	fclose(fp);
	printf("Finished calling countLines on %s. Result: %d\n", filename, count);
	return count;
}

LinkedList * readFile(const char * filename, size_t count)
{
	return NULL;
}
