# include <stdio.h>
# include <stdlib.h>
# include <string.h>

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
		if(decode(str1[i]) > decode(str2[i]))
			return 1;
		if(decode(str2[i]) > decode(str1[i]))
			return -1;
		i++;
	}
	if(str1[i] == ' ' && str2[i] == ' ')
		return 0;
	return -1;
}

int main()
{
	int len = 1000;
	char* store;			// stores input from STDIN in chars

	store = (char*)malloc(len * sizeof(char));

	// Problem providing storage
	if(store == NULL)
	{
		fputs("Problem providing storage!", stderr);
		exit(1);
	}

	int pos = 0;			// keeps track of current position in store array
	int wordCount = 0;

	char ch = 'a';

	// read STDIN to store
	while(ch != EOF)
	{
		if(pos == len)
		{
			store = realloc(store, 2 * len * sizeof(char));

			// Problem providing storage
			if(store == NULL)
			{
			fputs("Problem providing storage!", stderr);
			exit(1);
			}
			len*=2;
		}

		ch = getchar();
		if(ch != EOF)
		{
			store[pos] = ch;
			pos++;
		}
		// New word!
		if(ch == ' ')
			wordCount++;
	}

	// take care of case when last word is not properly formated (no space)
	if(store[pos - 1] != ' ')
	{
		if(pos >= len)
		{
			store = realloc(store, 2 * len * sizeof(char));
			// Problem providing storage
			if(store == NULL)
			{
			fputs("Problem providing storage!", stderr);
			exit(1);
			}
			len*=2;
		}
		store[pos] = ' ';
		pos++;
		wordCount++;
	}

	// Store only pointers to first chars in each word, convinient for sorting
	char** sepStore = (char**) malloc(2*wordCount * sizeof(char*));

	// Problem providing storage
	if(sepStore == NULL)
	{
		fputs("Problem providing storage!", stderr);
		exit(1);
	}

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
		while(sepStore[i][j] != ' ')
		{
			putchar(sepStore[i][j]);
			j++;
		}
		putchar(' ');
	}

	// deallocate memory
	free(sepStore);
	free(store);
}