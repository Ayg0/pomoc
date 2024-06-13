#include "pomodoro.h"

extern _pomodoroConf config;

void	ring(){
	static char *cmd = NULL;

	if (!cmd){
		cmd = malloc(strlen(config.soundPath) + 10);
		sprintf(cmd, "aplay -q %s", config.soundPath);
	}
	system(cmd);
}

void notifyStateChange(enum state next){
	char *next_state = next == WORK ? "\033[34mWork\033[0m" : "\033[32mRest\033[0m";
	char str[40];

	clearTerminal();
	sprintf(str, "Time is up! Time to %s", next_state);
	displayInfo(str, NULL);
	if (IS_FLAG_SET(config, FLAG_USE_SOUND))
		ring();
	sleep(2);
}