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
	
	printf("This is process 2\n");
	
	/*while(1){
		char send[100];
		char recv[100];
		
		read(fd0,recv,100);
		if(strlen(recv) == 1) break;
		printf("Parent sent  : %s\n",recv);
		
		printf("Child  : ");
		fgets(send,100,stdin);
		write(fd1,send,strlen(send)+1);
		
	}*/
	
	int pfd1[2];
	int pfd2[2];
	
	pipe(pfd1);
	pipe(pfd2);
	
	int c = fork();
	
	if(c > 0){
		
		close(pfd1[0]);
		close(pfd2[1]);
		

		while(1){
			char recv1[100];
			read(fd0,recv1,100);
			if(strlen(recv1) == 1) break;
			write(pfd1[1],recv1,strlen(recv1)+1);
			
			char recv2[100];
			read(pfd1[0],recv2,100);
			if(strlen(recv2) == 1) break;
			write(fd1,recv2,strlen(recv2)+1);	
			
		}

		
	}
	else{
	
		close(pfd1[1]);
		close(pfd2[0]);
		
		char in[10];
		int temp = dup(0);
		sprintf(in,"%d",temp);
		
		temp = dup(1);
		char out[10];
		sprintf(out,"%d",temp);
		
		dup2(pfd1[0],0);
		dup2(pfd2[1],1);
		
		system("gcc q3_p3.c -o q3_p3");
		char* args[] = {"./q3_p3",in,out,NULL};
		
		execvp(args[0],args);
	
	}
	
	return 0;

}
