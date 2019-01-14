#include<stdio.h>
#include<string.h>

/* Searches given place in file for search string. If search string is found, returns 1. 0 O.W. */
int scan(FILE *inputFile, unsigned char buffer[1], char *searchString, int len, int posBefore) {

	int pos = 1;	
	int matchFound = 2;
	int numMatches = 1;
	
	/* Loops through file, testing if next character matches the next search character. */
	while (fread(buffer, 1, 1, inputFile)==1 && matchFound == 2) {
		if (buffer[0] == (searchString[pos] & 0xff)) {
			numMatches++;
			pos++;
		} else {
			matchFound = 0;
		}	

		if (numMatches == len) {
			matchFound = 1;
		}
	}
	
	/* Moves file pointer to original position so main method can continue copying and searching */
	long posCur = ftell(inputFile);
	int posMove = posCur - posBefore;
	fseek(inputFile, -posMove, SEEK_CUR);
	
	return matchFound;
}

int main(int argc, char *argv[]) {
	
	int isClosed;	
	FILE *inputFile;
	FILE *outputFile;
	char *inputFilename = argv[1];
	char *outputFilename = argv[3];
  	char *searchString = argv[2];
	printf("The search string is: %s\n", searchString);
	int len = strlen(searchString);

	/* Attempts to open file */
	if ((inputFile = fopen(inputFilename, "rb")) == NULL) {
		printf("Error opening file!\n");
	}
	if (( outputFile = fopen(outputFilename, "wb")) == NULL) {
		printf("Error opening file!\n");
	}

	/*Find length of file*/
	fseek(inputFile, 0, SEEK_END);
	long size = ftell(inputFile);
	rewind(inputFile);

	printf("The size of the file is: %lu\n", size);
	
	int numWords = 0;
	unsigned char buffer[1];

	/* Reads file, copies file, and searches for search string. */
	while (fread(buffer, 1, 1, inputFile)==1) {
		fwrite (buffer, 1, 1, outputFile);

		/* If current buffer position matches the first char of search string, continue searching for search string */
		if (buffer[0] == (searchString[0] & 0xff)) {
			int posBefore = ftell(inputFile);
			int matchFound = scan(inputFile, buffer, searchString, len, posBefore);
			if (matchFound) {
				numWords++;
			}
		}
	}

	/* Reports number of matches and attempts to close file */
	printf("Number of matches found: %d\n", numWords);

	isClosed = fclose(inputFile);
	if(isClosed < 0) 
		perror("close");
	isClosed = fclose(outputFile);
	if (isClosed)
		perror("close");

	return 0;	
}
