/*
@author Danh Thanh Nguyen <d.t.nguyen@newcastle.ac.uk>

Many code patterns in this project are thanks to the openmovement project of Newcastle University.
See: https://github.com/digitalinteraction/openmovement for more details.
*/

#include "cawax_datareader.h"
#include "cawax.h"
#include "cawax_maths.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*
Read the required tokens from a sample string.
- Includes: Time, order(optional), x, y, z
Return 0 if success, a non-zero value otherwise
*/
int readTokens(char ** bufTokens, char * tokenString);

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
	printf("Finished calling countLines on %s. Result: %d\n", filename, count);
	return count;
}

LinkedList * readFile(const char * filename, int count, int * samplesRead)
{
	printf("Calling readFile on file %s, count %d\n", filename, count);
	//int lines = countLines(filename);

	if (INVALID_NEGATIVE_PARAM(count)) {
		printf("	Err: Negative count parameter: %d.", count);
		return NULL;
	}

	// open the file
	FILE * fp = fopen(filename, "r");
	if (!fp)
	{
		char errorMessage[100];
		strcat(strcpy(errorMessage, "	Error while opening file "), filename);
		perror(errorMessage);
		printf("	Returning NULL for readFile %s count %d", filename, count);
		return NULL;
	}

	//TODO free
	LinkedList * list = makeList();
	int samplesCounter = 0;
	//allocate memory for analysing samples
	char * currentSample = (char *)malloc(SAMPLE_BYTES);

	//TODO free
	char **tokens = (char **)malloc(SAMPLE_COMPONENTS_COUNT * sizeof(char *));
	////time
	//tokens[0] = (char *)malloc(SAMPLE_COMPONENT_SIZE);
	//// order
	//tokens[1] = (char *)malloc(SAMPLE_COMPONENT_SIZE);
	////x y z
	//tokens[2] = (char *)malloc(SAMPLE_COMPONENT_SIZE);
	//tokens[3] = (char *)malloc(SAMPLE_COMPONENT_SIZE);
	//tokens[4] = (char *)malloc(SAMPLE_COMPONENT_SIZE);
	for (int i = 0; i < SAMPLE_COMPONENTS_COUNT; i++) {
		tokens[i] = (char *)malloc(SAMPLE_COMPONENT_SIZE);
	}

	// loop over lines
	while (fgets(currentSample, SAMPLE_BYTES - 1, fp))
	{
		//printf("	Next sample read is: %s\n", currentSample);
		if (readTokens(tokens, currentSample)) {
			samplesCounter++;
			for (int i = 0; i < SAMPLE_COMPONENTS_COUNT; i++) {
				printf("token %d: %-15s // ", i, tokens[i]);
			}
			printf("\n");

			/*DO THE DATA PARSING*/
			// tl needed
			char * tl;

			/*=============================================================
			parse the time
			==============================================================*/
			// old time received, does not work anymore
			//CAWAX_TIME_MSM mili = 0;
			//CAWAX_TIME_MSM sec = 0;
			//CAWAX_TIME_MSM minute = 0;
			//char * mili_s = (char *) malloc(4);
			//
			//sscanf(tokens[0], "%d:%d.%s", &minute, &sec, mili_s);
			////printf("mili_s is: %s\n", mili_s);
			//mili = strtol(mili_s, &tl, 10) *  pow(10, 3 - strlen(mili_s));
			//free(mili_s);
			//printf("min: %d, sec: %d, %mili: %d\n", minute, sec, mili);

			//CAWAX_TIME_MSM time = CAWAX_TIME_FROM_MSM(minute, sec, mili);
			//printf("\n********** Time extracted is: %d, revalidate: min is: %d, sec is: %d, mili is: %d \n", time, 
			//	CAWAX_TIME_GET_MINUTE(time), CAWAX_TIME_GET_SECOND(time), CAWAX_TIME_GET_MILLISECOND(time));

			// correct new time (internal time of the imu)
			INTERNAL_TIME second = 0;
			INTERNAL_TIME micro = 0;
			// needs to handle round micro values
			char * micro_s = (char *) malloc(7);
			sscanf(tokens[TIME_INTERNAL_BUF_INDEX], "%d.%s", &second, micro_s);
			micro = strtol(micro_s, &tl, 10) *  pow(10, 6 - strlen(micro_s));
			free(micro_s);
			INTERNAL_TIME internalTime = INTERNAL_TIME_FROM_S(second, micro);
			//printf("\n********** Time extracted is: %d, revalidate: sec is: %d, micro is: %d \n", internalTime, INTERNAL_TIME_GET_SECOND(internalTime), INTERNAL_TIME_GET_MICRO(internalTime));

			/* =============================================================
			parse the order
			================================================================*/
			sample_th order;
			sscanf(tokens[SAMPLE_INDEX_BUF_INDEX], "%d", &order);
			//printf("sample order: %d\n", order);

			/* =============================================================
			parse the accelerometer values x, y, z
			================================================================*/
			acc x;
			acc y;
			acc z;
			sscanf(tokens[X_BUF_INDEX], "%lf", &x);
			sscanf(tokens[Y_BUF_IDNEX], "%lf", &y);
			sscanf(tokens[Z_BUF_INDEX], "%lf", &z);
			//printf("x: %.6f, y: %.6f, z: %.6f \n", x, y, z);
			
			// todo add a param for optional computation
			
			Sample sample = { internalTime, order, x, y, z, 0 };
			sample.rmq = rootMeanSquare(&sample);
			add(sample, list);
		}
		//printf("Samples read so far: %d\n", ++samplesCounter);
	}
	*samplesRead = samplesCounter;
	printf("Calling readFile on file %s finished. %d samples read.\n", filename, samplesCounter);
	//free char** tokens and elements
	free(currentSample);
	for (int i = 0; i < SAMPLE_COMPONENTS_COUNT; i++) {
		free(tokens[i]);
	}
	free(tokens);

	return list;
}

int readTokens(char ** bufTokens, char * tokenString) {
		//printf("Calling readTokens with tokenString %s\n", tokenString);
		int indexCounter = 0;
		int bufIndexCounter = 0;
		char * delimeters = ",";

		//  make a tmp token string 
		// TODO free
		char * tmpTokenString = (char *)malloc(strlen(tokenString) + 1);
		strcpy(tmpTokenString, tokenString);
		//printf("	tmpToken string is: %s\n", tmpTokenString);

		char * currentToken;
		currentToken = strtok(tmpTokenString, delimeters);
		//printf("	Current token is: %s\n", currentToken);
		if (INDEX_A_TARGET_TOKEN(indexCounter)) {
			bufTokens[bufIndexCounter++] = currentToken;
		}
		++indexCounter;

		//while (currentToken = strtok(NULL, delimeters) && indexCounter < Z_INDEX && bufIndexCounter <= SAMPLE_COMPONENTS_COUNT) {
			while ((currentToken = strtok(NULL, delimeters))  && indexCounter <= Z_INDEX & bufIndexCounter < SAMPLE_COMPONENTS_COUNT)  {
			//printf("	Current token is: %s, indexCounter is: %d\n", currentToken, indexCounter);
			if (INDEX_A_TARGET_TOKEN(indexCounter)) {
				strcpy(bufTokens[bufIndexCounter++], currentToken);
				// printf("	Current target token is: %s, bufCounter is: %d\n", currentToken, bufIndexCounter);
			}
			++indexCounter;
		}
			//printf("BufIndexCount is: %d\n", bufIndexCounter);
		// if not enough data is read then abort this tokens reading
			if (bufIndexCounter != (SAMPLE_COMPONENTS_COUNT)) {
				printf("Error reading sample %s: Insufficient target tokens. Abort the reading.\n", tokenString);
				return 0;
			}
		free(tmpTokenString);
		return 1;
	}

