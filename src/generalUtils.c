#include "pomodoro.h"

void clearTerminal() {
    printf("\033[H\033[J");
}

// Function to get the length of a string ignoring ANSI escape codes
size_t	strlenIgnoreAnsi(char *s){
	size_t len = 0;
	for (size_t i = 0; s[i]; i++){
		if (s[i] == '\033'){
			while (s[i] && s[i] != 'm')
				i++;
		}
		else
			len++;
	}
	return len;
}

// turn a string to lower case
void	turnLowerCase(char *s){
	for (int i = 0; s[i]; i++)
		s[i] = tolower(s[i]);
}

// Function to remove leading and trailing spaces from a string
char *trimSpaces(char *s){
	while (isspace(*s))
		s++;
	while (isspace(s[strlen(s) - 1]))
		s[strlen(s) - 1] = 0;
	return s;
}