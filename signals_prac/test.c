#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<signal.h>

void hfptc(){
	printf("signal from parent to child\n");
}

void hfctp(){
	printf("signal from child to parent\n");
}

int main(){

	signal(SIGUSR1,hfptc);
	signal(SIGUSR2,hfctp);
	
	int c = fork();
	
	if(c > 0){
		kill(c,SIGUSR1);
		wait();
	}
	else{
		kill(getppid(),SIGUSR2);
	}
	
	return 0;

}
