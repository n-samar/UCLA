

// decode single character
char decode(const char ch)
{
	return (char)(ch^42);		// 42 = 00101010 base 10
}

int main(int argc, char** argv)
{
	int ch;
	while((ch = getchar()) != -1)
	{
		putchar(decode(ch));
	}
}