#ifndef PROC_H
#define PROC_H

#ifndef DBUG
	#define DBUG
#endif

typedef enum process_state{running, notrunning, error} _PROC_STATE;

_PROC_STATE check_instance(pid_t pid, char *test_line);

#endif