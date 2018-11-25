#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main()
{
	int fd[2];
	if(pipe(fd) < 0){
		printf("Failed pipe open\n");
		exit(0);
	}

	time_t ttime;

	pid_t pid = fork();
	if(pid == 0){	//CHILD - source
		printf("CHILD +\n");

		close(fd[0]);
		time_t data = time(0);
		write(fd[1], &data, sizeof(time_t));
		close(fd[1]);

		printf("CHILD -\n");
		exit(0);
	}else{		//PARENT - target
		printf("PARENT +\n");

		close(fd[1]);
		time_t buf;
		read(fd[0], &buf, sizeof(time_t));
		printf("PARENT -\n%s\n", ctime(&buf));
	}
	return 0;
}
