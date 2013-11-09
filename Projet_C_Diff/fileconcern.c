#include "Header.h"
#include <stdio.h>
#include <stdlib.h>

//Options fonctions of the diff command

void help_option()
{
	printf("Usage: diff [OPTION]... FICHIERS");
	printf("Comparer les fichiers ligne par ligne.");

	//suite des d�tails d'options
}


// fonction strcomp source wiki

int strcomp(const char* s1, const char* s2)
{
	while ((*s1++ == *s2++) && (*s1 != '\0'));
	return (*((unsigned char *)--s1) < *((unsigned char *)--s2)) ? -1 : (*(unsigned char *)s1 != *(unsigned char *)s2);
}


int myStrlen(char *s)
{
	char *start;
	start = s;
	while (*s != 0)
	{
		++s;
	}
	return s - start;
}


/*Count lines
*/
int linesOfFile(char* file)
{
	FILE* fileParam = NULL;

	fileParam = fopen(file, "r");

	int  c; 
	unsigned long newline_count = 0;

	/* count the newline characters */
	while ((c = fgetc(fileParam)) != EOF) {
		if (c == '\n')
			newline_count++;
	}

	fclose(fileParam);
	return newline_count;


}


