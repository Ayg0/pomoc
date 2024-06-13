#include "pomodoro.h"


int			session = 1;
enum state	current_state = WORK;
extern _pomodoroConf config;

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

void	pomodoroStart(){
	while (1)
	{
		holdTime(config.workTime, WORK);
		notifyStateChange(REST);
		holdTime(getRestTime(config.RestTime, session), REST);
		notifyStateChange(WORK);
		session++;
	}
}

int	main(int ac, char **av){

	#ifdef CONF_FILE
		parseConf(CONF_FILE);
	#else
		parseConf("pomodoro.conf");
	#endif
	// update config via args if any provided
	updateViaArgs(ac, av);

	write(1, "\033[?25l", 6); // hide cursor
	// handle ctrl+c
	signal(SIGINT, sigintHandler);

	pomodoroStart();

	write(1, "\033[?25h", 6); // show cursor
	return 0;
}