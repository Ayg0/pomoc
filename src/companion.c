#include "pomodoro.h"

extern _pomodoroConf config;

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
	for (int i = 0; i < 3; i++)
		printf("\033[%d;%dH\033[1m%s%s\033[0m", w.ws_row / 2 + 2 + i, padding, config.catColor, cat_frames[frame * 3 + i]);
	frame = (frame + 1) % 4;
}