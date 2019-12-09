# include <stdio.h>
# include <stdlib.h>
# include <string.h>

int main(int argc, char * argv[])
{
	if(argc != 3)
	{
		fprintf(stderr, "Error: incorrect number of arguments");
		exit(1);
	}

	char* s1 = argv[1];
	char* s2 = argv[2];
	int i;
	
	//checks length
	if(strlen(s1) != strlen(s2))
	{
		fprintf(stderr, "Error: Arguments not of equal length");
		exit(1);
	}

	for(int i = 0; i < strlen(s1) - 1; i++)
		for(int j = i + 1; j < strlen(s1); j++)
			if(s1[i] == s2[j])
			{
				fprintf(stderr, "Error: from string has duplicate bytes");
				exit(1);
			}

	char ch;
	while(read(0, &ch, 1) > 0)
	{
		int i;
		for(i = 0; s1[i] != '\0' && ch != s1[i]; i++);

		if(s1[i] != '\0')
			ch = s2[i];

		write(1, &ch, 1);
	}

	return 0;
}