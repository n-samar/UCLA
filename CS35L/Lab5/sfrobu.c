#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int opt = 0;

// decode single character
char decode(const char ch)
{
	return (char)(ch^42);		// 42 = 00101010 base 10
}


// compare function for frobcmp
int frobcmp(void const *str1Void, void const *str2Void)
{
	char* str1 = *((char **)str1Void);
	char* str2 = *((char **)str2Void);
	//printf("%s", str1);
	int i = 0;
	while(str1[i] != ' ')
	{
		//putchar(decode(str1[i]));
		if(str2[i] == ' ')
			return 1;
		char dec1 = decode(str1[i]);
		char dec2 = decode(str2[i]);
		if(opt)
		{
			if(dec1 > -1 && dec1 < UCHAR_MAX)
				dec1 = toupper(dec1);
			if(dec2 > -1 && dec2 < UCHAR_MAX)
				dec2 = toupper(dec2);
		}
		if(dec1 > dec2)
			return 1;
		if(dec2 > dec1)
			return -1;
		i++;
	}
	if(str1[i] == ' ' && str2[i] == ' ')
		return 0;
	return 1;
}

int main(int argc, char** argv)
{
	char readbuf[2];
	int len;
	char* store;			// stores input from STDIN in chars
	struct stat buf;

	if(argc == 2 && strlen(argv[1]) == 2 
		&& argv[1][0] == '-' && argv[1][1] == 'f')
		opt = 1;

	else if(argc > 1)
	{
		fprintf(stderr, "Improperly formated options.");
	}
	// Fileinfo
    ssize_t ret = fstat(STDIN_FILENO, &buf);
    size_t fileSize = buf.st_size + 1;
    
	store = (char*)malloc(fileSize * sizeof(char));


	int pos = 0;			// keeps track of current position in store array
	int wordCount = 0;

	char ch = 'a';

	// read STDIN to store
	while(read(STDIN_FILENO, readbuf, 1))
	{
		if(pos == fileSize)
		{
			store = realloc(store, 2 * fileSize * sizeof(char));

			// Problem providing storage
			if(store == NULL)
			{
			fputs("Problem providing storage!", stderr);
			exit(1);
			}
			fileSize*=2;
		}
		ch = readbuf[0];
		store[pos] = ch;
		pos++;
		// New word!
		if(ch == ' ')
			wordCount++;
	}

	// take care of case when last word is not properly formated (no space)
	if(store[pos - 1] != ' ')
	{
		store[pos] = ' ';
		pos++;
		wordCount++;
	}

	// Store only pointers to first chars in each word, convinient for sorting
	char** sepStore = (char**) malloc(2*wordCount * sizeof(char*));


	int wordPos = 0;		// keep track of current word in word list
	sepStore[0] = store;
	for(int i = 0; i < pos-1; i++)
	{
		if(store[i] == ' ')
		{
			wordPos++;
			sepStore[wordPos] = &store[i+1];
			//putchar(*sepStore[wordPos]);
		}
	}
	wordPos++;
	qsort(sepStore, wordPos, sizeof(char*), frobcmp);

	// Output sorted list of words to STDOUT
	for(int i = 0; i < wordPos; i++)
	{
		int j = 0;
		for(j = 0; sepStore[i][j] != ' '; j++)
		{
			write(STDOUT_FILENO, &sepStore[i][j], 1);
		}
		write(STDOUT_FILENO, &sepStore[i][j], 1);
	}

	// deallocate memory
	free(sepStore);
	free(store);
}