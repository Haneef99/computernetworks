#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<poll.h>

struct pollfd fds[4];

int timeout = 5000;

int main(){

	FILE* fp[4];
	
	fp[0] = popen("./p2","r");
	fp[1] = popen("./p3","r");
	fp[2] = popen("./p4","r");
	fp[3] = popen("./p5","r");
	
	char buff[100];
	
	int pfd[2];
	
	pipe(pfd);
	
	int c = fork();
	
	if( c > 0){
	
		dup2(pfd[1],1);
	
		while(1){
		
			for(int i=0;i<4;i++){
				fds[i].fd = fileno(fp[i]);
				fds[i].events = POLLIN;
			}
			
			int x = poll(fds,4,timeout);
			
			int flag = 1;
			
			for(int i=0;i<4;i++){
				
				if(fds[i].revents & POLLIN){
					int s = read(fds[i].fd,buff,100);
					flag = 0;
					printf("%s\n",buff);
				}
			
			}
			
			if(flag){
			
				printf("output of process 1\n");
				
				break;
			
			}
			
		}
		
//		printf("hi");
	
	}
	else{
		dup2(pfd[0],0);
		
		char* args[] = {"./p6",NULL};
		
		execvp(args[0],args);
		
	}
	
	return 0;

}
