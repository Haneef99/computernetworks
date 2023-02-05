#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

int main()
{

	int pfd1[2];
	int pfd2[2];
	
	pipe(pfd1);
	pipe(pfd2);
	
	int c = fork();
	
	if(c > 0){
	
		close(pfd1[0]);
		close(pfd2[1]);
	
		while(1){
		
			char send[100],recv[100];
			printf("p1 : ");
			fgets(send,100,stdin);
			if(strlen(send) == 1) break;
			write(pfd1[1],send,strlen(send)+1);
			
			read(pfd2[0],recv,100);
			printf("msg from p3 : %s\n",recv);
		}
	
	}
	else{
		
		close(pfd1[1]);
		close(pfd2[0]);
		
		int temp = dup(0);
		char in[10];
		sprintf(in,"%d",temp);
		temp  = dup(1);
		char out[10];
		sprintf(out,"%d",temp);
		
		dup2(pfd1[0],0);
		dup2(pfd2[1],1);
		
		
		system("gcc q3_p2.c -o q3_p2");
		char* args[] = {"./q3_p2",in,out,NULL};
		execvp(args[0],args);
		
		
	}

	return 0;

}
