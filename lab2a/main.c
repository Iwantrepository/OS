#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include <stddef.h>
#include <stdlib.h>
#include <sys/stat.h>

#define SOURCE_FILE "source.txt"
#define TARGET_FILE "target.txt"

#define FILE_SIZE 100000000 //100000000 ~ 100mb
#define BUFFER_SIZE 10000000

int generate_source_file()
{
	int fd;
	char * buff = (char*)calloc(BUFFER_SIZE, sizeof(char));
	
	for (int i=0; i<BUFFER_SIZE; i++)	//generating buffer
		buff[i] = '0'+i%10;

	if((fd=open(SOURCE_FILE, O_RDWR | O_TRUNC | O_CREAT)) < 0){
		return -1;
	}else{
		printf("Starting generating file %s\n", SOURCE_FILE);
		for(int i=0; i< FILE_SIZE/BUFFER_SIZE; i++){
			write(fd, buff, BUFFER_SIZE);
			printf("%d/10 of file generated\n", i+1);
		}	
	}
	printf("File %s generated\n", SOURCE_FILE);
	close(fd);
	return 0;
}

int create_target_file()
{
	int fd;
	if((fd=open(TARGET_FILE, O_RDWR | O_TRUNC | O_CREAT)) < 0)
		return -1;
	close(fd);
	printf("File %s created\n", TARGET_FILE);
	return 0;
}

int source_to_target_copy()
{
	fd_set rfds;
	fd_set wfds;

	FD_ZERO( &rfds );
	FD_ZERO( &wfds );


	int rfd = open(SOURCE_FILE, O_RDWR);
	int wfd = open(TARGET_FILE, O_RDWR);

	if( rfd<0 || wfd<0 ){
		printf("Can't open files");
		return -1;
	}

	char * buff = (char*)calloc(BUFFER_SIZE, sizeof(char));
	

	int i=0;
	ssize_t buf_amount;

	buf_amount = read(rfd, buff, BUFFER_SIZE);
	do{
		i++;
		write(wfd, buff, buf_amount);
		buf_amount = read(rfd, buff, BUFFER_SIZE);
		printf("%d/10 of file copied\n", i);
	}while(buf_amount);
	


	close(rfd);
	close(wfd);
	printf("File %s copied\n", SOURCE_FILE);
	return(0);
}

int main(void)
{
	if(create_target_file() == -1){
		printf("Can't create %s", TARGET_FILE);
		exit(0);
	}
	
	pid_t pid = fork();
	if(pid == 0){	//CHILD
		sleep(1);
		if(source_to_target_copy() == -1){
			printf("Copying failed");
			exit(0);
		}
	}
	else{	//PARENT
		if(generate_source_file() == -1){
			printf("Can't open %s", SOURCE_FILE);
			exit(0);
		}
	}
	return 0;
}
