#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<string.h>


int main(){

	int c;
	
	int input_fd = open("./in.txt",O_RDONLY);
	int output_fd = open("./out.txt",O_RDWR);
	
	c = fork();
	
	if(c > 0){
		wait(NULL);
		// check if output and provided output are same
		
		//printf("Child execution completed\n");
		
		int same = 1;
		
		char buffer1[2];
		char buffer2[2];
		FILE* out = fopen("./out.txt","r");
		FILE* r_out = fopen("./outp.txt","r");
		while(fgets(buffer1,2,out) != NULL && fgets(buffer2,2,r_out) != NULL){
			if(strcmp(buffer1,buffer2) != 0) same = 0;
		}
		
		if(same) printf("Test cases passed\n");
		else printf("Test cases failed\n");
		
		
		
	}
	else{
		//input from in.txt and output to out.txt
		
		dup2(input_fd,0);
		dup2(output_fd,1);
		
		// executing the client program
		system("gcc cp.c");
		
		char* args[] = {"./a.out",NULL};
		
		execvp(args[0],args);
		
	}

	
	return 0;
}

