#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "pomodoro.h"

// state of the timer
enum state {
	WORK,
	REST
};

int			session = 1;
enum state	current_state = WORK;
extern _pomodoroConf config;
void clear_terminal() {
    printf("\033[H\033[J");
}

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


void DrawnextCatFrame(){
	static int frame = 0;

    const char* cat_frames[] = {
		"     |\\__/,|  (`\\",
		"  _.|o o  |_   ) )",
		"-(((---(((--------",
		"     |\\__/,|   (`\\",
		"  _.|o o  |_   ) )",
		"-(((---(((--------",
		"     |\\__/,|    (`\\",
		"  _.|o o  |_   ) )",
		"-(((---(((--------",
		"     |\\__/,|    /`)",
		"  _.|o o  |_   ) )",
		"-(((---(((--------",
    };

	// each tree lines are a cat frame, draw under the timer
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	int padding = (w.ws_col - 18) / 2;

	// draw frame under the timer w.ws_row / 2 + 1
	for (int i = 0; i < 3; i++){
			printf("\033[%d;%dH\033[1m%s%s\033[0m", w.ws_row / 2 + 2 + i, padding, config.catColor, cat_frames[frame * 3 + i]);
		}
	frame = (frame + 1) % 4;
}

void print_centered(char *mainStr, char *helpers) {
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	clear_terminal();
	if (helpers){
		int helper_padding = (w.ws_col - strlenIgnoreAnsi(helpers)) / 2;
		printf("\033[%d;%dH\033[1m%s\033[0m\n", w.ws_row / 2 - 2, helper_padding, helpers);
	}
	DrawnextCatFrame();
	int padding = (w.ws_col - strlenIgnoreAnsi(mainStr)) / 2;
	printf("\033[%d;%dH\033[1m%s\033[0m\n", w.ws_row / 2, padding, mainStr);

	//print session at the bottom left
	printf("\033[%d;%dH\033[1mSession: %d\033[0m\n", w.ws_row - 1, 0, session);
}

char *getRandomHelper(enum state state){
	return state == WORK ? encouragments[rand() % 13] : rests[rand() % 13];
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
		print_centered(time_str, helper);
        sleep(1);
		if (seconds == 0)
			helper = getRandomHelper(state);
    } while ((start_time + delay) > current_time);
}

void notifyStateChange(enum state next){
	char *next_state = next == WORK ? "\033[34mWork\033[0m" : "\033[32mRest\033[0m";
	char str[40];

	clear_terminal();
	sprintf(str, "Time is up! Time to %s", next_state);
	print_centered(str, NULL);
	if (IS_FLAG_SET(config, FLAG_USE_SOUND))
		system(strcat("aplay -q ", config.soundPath));
	sleep(2);
}

void sigintHandler(int sig_num)
{
	(void)sig_num;
    /* Reset handler to catch SIGINT next time. */
    signal(SIGINT, sigintHandler);
    printf("\nProccess terminated by user\n");
    write(1, "\033[?25h", 6); // show cursor
    exit(0);
}

int	getRestTime(int restTime, int session){
	if (session != 1 && IS_FLAG_SET(config, FLAG_LONG_REST) && (session % 3 == 0))
		return restTime * 3;
	return restTime;
}

void	updateViaArgs(int ac, char **av){
	if (ac >= 3)
		config.RestTime = atoi(av[2]);
	if (ac >= 2)
		config.workTime = atoi(av[1]);
}

int	main(int ac, char **av){

	#ifdef CONF_FILE
		parseConf(CONF_FILE);
	#else
		parseConf("pomodoro.conf");
	#endif

	updateViaArgs(ac, av);

	write(1, "\033[?25l", 6); // hide cursor
	signal(SIGINT, sigintHandler);
	while (1)
	{
		holdTime(config.workTime, WORK);
		notifyStateChange(REST);
		holdTime(getRestTime(config.RestTime, session), REST);
		notifyStateChange(WORK);
		session++;
	}
	write(1, "\033[?25h", 6); // show cursor
	return 0;
}