#pragma once

#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>

enum pomodoroFlags
{
	FLAG_LONG_REST,
	FLAG_USE_SOUND,
	FLAG_SHOW_SESSION,
};

enum state {
	WORK,
	REST
};

typedef	struct tokenValue
{
	char *token;
	char *value;
} _tokenValue;


typedef struct pomodoroConf
{
	int		workTime;
	int		RestTime;
	char	*catColor;
	char	*soundPath;
	int		flags;
} _pomodoroConf;

# define SET_FLAG(conf, flag) (conf.flags |= (1 << flag))
# define RESET_FLAG(conf, flag) (conf.flags &= ~(1 << flag))
# define IS_FLAG_SET(conf, flag) (conf.flags & (1 << flag))

// parse conf:
int	parseConf(char *configPath);

//notify:
void notifyStateChange(enum state next);

// companion:
void DrawnextCatFrame();

// display:
void holdTime(int delay, enum state state);
void displayInfo(char *mainStr, char *helpers);

// general utils:
void	clearTerminal();
size_t	strlenIgnoreAnsi(char *s);
char	*trimSpaces(char *s);
void	turnLowerCase(char *s);