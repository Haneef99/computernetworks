#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

int main(int argc,char* argv[]){

	int in = atoi(argv[1]);
	int out = atoi(argv[2]);
	
	int fd0 = dup(0);
	int fd1 = dup(1);
	
	dup2(in,0);
	dup2(out,1);
	
	printf("This is process 3\n");
	
	while(1){
	
		char send[100], recv[100];
		
		read(fd0,recv,100);
		if(strlen(recv) == 1) break;
		printf("Message from p1 to p3 : %s\n",recv);
		
		printf("p3 : ");
		fgets(send,100,stdin);
		if(strlen(send) == 1) break;
		
		write(fd1,send,strlen(send)+1);
	
	}
	
	return 0;

}
