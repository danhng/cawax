#include "cawax_datareader.h"
#include "cawax.h"
#include <string.h>

/*
Read the required tokens from a sample string.
- Includes: Time, order(optional), x, y, z
*/
char ** readTokens(char * string, int count);

int countLines(const char * filename)
{
	printf("Calling countLines on %s\n", filename);
	FILE * fp = fopen(filename, "r");
	if (!fp)
	{
		char errorMessage[100];
		strcat(strcpy(errorMessage, "	Error while opening file "), filename);
		perror(errorMessage);
		printf("	Returning -1 for countLines on file %s.\n", filename);
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
		strcat(strcpy(errorMessage, "	Error while reading file "), filename);
		perror(errorMessage);
		printf("	Returning -1 for countLines on file %s.\n", filename);
		return -1;
	}
	fclose(fp);
	printf("	Finished calling countLines on %s. Result: %d\n", filename, count);
	return count;
}

LinkedList * readFile(const char * filename, int count)
{
	printf("Calling readFile on file %s, count %d\n", filename, count);
	//int lines = countLines(filename);

	if (INVALID_NEGATIVE_PARAM(count)) {
		printf("	Err: Negative count parameter: %d.", count);
		return NULL;
	}

	//TODO free
	LinkedList * list = makeList();

	FILE * fp = fopen(filename, "r");
	if (!fp)
	{
		char errorMessage[100];
		strcat(strcpy(errorMessage, "	Error while opening file "), filename);
		perror(errorMessage);
		printf("Returning NULL for readFile %s count %d", filename, count);
		return NULL;
	}
	//allocate memory for analysing samples
	char * currentSample = (char *)malloc(SAMPLE_BYTES);

	while (fgets(currentSample, sizeof(currentSample), fp))
	{
		printf("Next sample read is: %s\n", currentSample);

		//TODO free
		char *tokens[] = (char **)malloc(SAMPLE_COMPONENTS_COUNT * sizeof(char *));
		//time
		tokens[0] = (char *)malloc(9);
		// order
		tokens[1] = (char *)malloc(9);
		//x y z
		tokens[2] = (char *)malloc(9);
		tokens[3] = (char *)malloc(9);
		tokens[4] = (char *)malloc(9);

		//TODO 
	}
	return NOT_IMPLEMENTED_PTR;
}

	char ** readTokens(char ** bufTokens, char * tokenString, int count) {
		int indexCounter = 0;

		//  make a tmp token string 
		// TODO free
		char * tmpTokenString = (char *)malloc(strlen(tokenString) + 1);
		strncpy(tmpTokenString, tokenString, sizeof(tmpTokenString)-1);
		
		//#define TIME_INDEX 1
		//#define SAMPLE_INDEX 2
		//#define X_INDEX 4
		//#define Y_INDEX 5
		//#define Z_INDEX 6

		char * currentToken;
		currentToken = strtok(tmpTokenString, ",");
		if (INDEX_A_TARGET_TOKEN(indexCounter)) {

		}
		while (currentToken = strtok)

	}




	

	
	


}
