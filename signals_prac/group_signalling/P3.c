#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>

void hdfn(){

	write(1,"This is handler function of P3\n",sizeof "This is handler function of P3\n"-1);
	
	return ;
}

int main(){
	
	signal(SIGUSR1,hdfn);
	sleep(5);
	return 0;
}
