#include "pomodoro.h"

extern int session;
extern _pomodoroConf config;

char *encouragments[] = {
	"Keep Going...",
	"Almost there...",
	"Stay focused...",
	"Keep it up...",
	"Stay on target...",
	"Keep pushing...",
	"Stay strong...",
	"Keep moving...",
	"Stay motivated...",
	"Keep grinding...",
	"Stay determined...",
	"Keep hustling...",
	"Stay committed...",
};

char *rests[] = {
	"Take a break...",
	"Relax...",
	"Rest up...",
	"Take it easy...",
	"Chill out...",
	"Rest and relax...",
	"Take a breather...",
	"Unwind...",
	"Take a load off...",
	"Kick back...",
	"Take a rest...",
	"Recharge...",
	"Take a breather...",
};

char *getRandomHelper(enum state state){
	return state == WORK ? encouragments[rand() % 13] : rests[rand() % 13];
}


void displayInfo(char *mainStr, char *helpers) {
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	clearTerminal();
	if (helpers){
		int helper_padding = (w.ws_col - strlenIgnoreAnsi(helpers)) / 2;
		printf("\033[%d;%dH\033[1m%s\033[0m\n", w.ws_row / 2 - 2, helper_padding, helpers);
	}
	DrawnextCatFrame();
	int padding = (w.ws_col - strlenIgnoreAnsi(mainStr)) / 2;
	printf("\033[%d;%dH\033[1m%s\033[0m\n", w.ws_row / 2, padding, mainStr);

	//print session at the bottom left
	if (IS_FLAG_SET(config, FLAG_SHOW_SESSION))
		printf("\033[%d;%dH\033[1mSession: %d\033[0m\n", w.ws_row - 1, 0, session);
}

void holdTime(int delay, enum state state){
	time_t start_time;
    time_t current_time;
	char time_str[20];
	char *helper = getRandomHelper(state);
    /* Obtain current time as start time */
    start_time = time(NULL);
    do {
        current_time = time(NULL);
        int remaining_time = (start_time + delay) - current_time;

        int hours = remaining_time / 3600;
        int minutes = (remaining_time % 3600) / 60;
        int seconds = remaining_time % 60;

        sprintf(time_str, "%02d:%02d:%02d\n", hours, minutes, seconds);
		displayInfo(time_str, helper);
        sleep(1);
		if (seconds == 0)
			helper = getRandomHelper(state);
    } while ((start_time + delay) > current_time);
}