#include "pomodoro.h"

_pomodoroConf config;
int	currentLineNumber = 0;

void	parseError(char *msg){
	fprintf(stderr, "Error on line %d: %s\n", currentLineNumber, msg);
	exit(1);
}

_tokenValue getTokenValue(char *line){
	_tokenValue tokenVal;
	char *token = strtok(line, "=");
	char *value = strtok(NULL, "=");


	tokenVal.token = trimSpaces(token);
	tokenVal.value = trimSpaces(value);
	
	return tokenVal;
}

char *validTokens[] = {
	"workTime",
	"restTime",
	"soundPath",
	"catColor",
	"tripleRest",
	"showSession",
	"notifySound",
	NULL
};

int	checkToken(char *token){
	int i = 0;
	while (validTokens[i]){
		if (strcmp(token, validTokens[i]) == 0)
			return i;
		i++;
	}
	return -1;
}

int	getTrueFalse(char *value){

	turnLowerCase(value);

	if (strcmp(value, "true") == 0)
		return 1;
	else if (strcmp(value, "false") == 0)
		return 0;
	parseError("Invalid value for token: true or false expected.");
	return -1;
}

int	setResetFlag(char *value, int flag){
	if (getTrueFalse(value) == 1)
		SET_FLAG(config, flag);
	else
		RESET_FLAG(config, flag);
	return 0;
}

char *getAnsiColor(char *value){
	turnLowerCase(value);

	char *colors[] = {
		"red",
		"green",
		"yellow",
		"blue",
		"magenta",
		"cyan",
		"white",
		NULL
	};
	char *ansiColors[] = {
		"\033[31m",
		"\033[32m",
		"\033[33m",
		"\033[34m",
		"\033[35m",
		"\033[36m",
		"\033[37m",
	};
	for (int i = 0; colors[i]; i++){
		if (strcmp(value, colors[i]) == 0)
			return ansiColors[i];
	}
	parseError("Invalid color in config file");
	return NULL;
}

int	updateValue(int tokenIndex, char *value){
	switch (tokenIndex){
		case 0:
			config.workTime = atoi(value);
			break;
		case 1:
			config.RestTime = atoi(value);
			break;
		case 2:
			config.soundPath = strdup(value);
			break;
		case 3:
			config.catColor = getAnsiColor(value);
			break;
		case 4:
			setResetFlag(value, FLAG_LONG_REST);
			break;
		case 5:
			setResetFlag(value, FLAG_SHOW_SESSION);
			break;
		case 6:
			setResetFlag(value, FLAG_USE_SOUND);
			break;
	}
	return 0;
}

int	parseLine(char *line){
	int	tokenIndex;

	_tokenValue tokenVal =  getTokenValue(line);
	tokenIndex = checkToken(tokenVal.token);
	if (tokenIndex == -1)
		parseError("Invalid token in config file");
	updateValue(tokenIndex, tokenVal.value);
	return 0;
}

char	*fromAnsiToColor(char *color){
	char *colors[] = {
		"RED",
		"GREEN",
		"YELLOW",
		"BLUE",
		"MAGENTA",
		"CYAN",
		"WHITE",
		NULL
	};
	char *ansiColors[] = {
		"\033[31m",
		"\033[32m",
		"\033[33m",
		"\033[34m",
		"\033[35m",
		"\033[36m",
		"\033[37m",
	};
	for (int i = 0; colors[i]; i++){
		if (strcmp(color, ansiColors[i]) == 0)
			return colors[i];
	}
	return NULL;
}

void	debugConfig(){
	FILE *LOG = fopen("pomodoro.log", "w");

	fprintf( LOG, "------------------------------   Values   ------------------------------\n");
	fprintf( LOG, "workTime: %d\n", config.workTime);
	fprintf( LOG, "RestTime: %d\n", config.RestTime);
	fprintf( LOG, "catColor: %s\n", fromAnsiToColor(config.catColor));
	fprintf( LOG, "soundPath: %s\n", config.soundPath);
	fprintf( LOG, "------------------------------   flags    ------------------------------\n");
	fprintf( LOG, "long_rest: %s\n", IS_FLAG_SET(config, FLAG_LONG_REST) ? "true" : "false");
	fprintf( LOG, "show_session: %s\n", IS_FLAG_SET(config, FLAG_SHOW_SESSION)? "true" : "false");
	fprintf( LOG, "use_sound: %s\n", IS_FLAG_SET(config, FLAG_USE_SOUND) ? "true" : "false");
	fprintf( LOG, "----------------------------------------------------------------------\n");	

	fclose(LOG);
}

int	parseConf(char *configPath){
	FILE *configFile;
	char *line = NULL;
	size_t len = 0;
	int read;
	
	configFile = fopen(configPath, "r");
	if (configFile == NULL){
		perror("Error opening config file");
		exit(1);
	}
	
	while ((read = getline(&line, &len, configFile)) != -1){
		currentLineNumber++;
		if (line[0] == '#')
			continue;
		parseLine(line);
	}
	debugConfig();
	fclose(configFile);
	free(line);

	return 0;
}