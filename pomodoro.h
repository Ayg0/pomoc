#pragma once

#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

enum pomodoroFlags
{
	FLAG_LONG_REST,
	FLAG_USE_SOUND,
	FLAG_SHOW_SESSION,
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