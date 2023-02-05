#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include<sys/types.h>
#include<string.h>
#include<fcntl.h>

int main(){
	
	sem_t* s23 = sem_open("s23",O_CREAT,0666,0);
	sem_t* s34 = sem_open("s34",O_CREAT,0666,0);
	
	while(1){
	
		printf("P3 : waiting for s23\n");;
		
		sem_wait(s23);
		
		printf("P3 : s23 signalled by p1\n");
		
		printf("P3 : Enter any character to signal s34\n");
		char ch;
		scanf("%c",&ch);
		
		printf("P3 : signalling s34\n");
		
		sem_post(s34);
	
	}
	
	sem_close(s23);
	sem_close(s34);
	
	sem_unlink("s23");
	sem_unlink("s34");

	return 0;

}
