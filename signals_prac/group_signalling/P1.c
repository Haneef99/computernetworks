#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<signal.h>

void hdfn(){

	write(1,"This is handler function of P1\n",sizeof "This is handler function of P1\n"-1);

	return ;

}

int main(){

	signal(SIGUSR1,hdfn);

	setpgid(0,0);
	
	int gid = getpgid(0);

	int c = getpid();
	
	int c1 = fork();
	
	if(c1 > 0){
		int c2 = fork();
		
		if(c2 > 0){
			
			sleep(5);

			killpg(gid,SIGUSR1);
		
		}
		else{
		
			setpgid(c2,c);
		
			char* args[] = {"./P2",NULL};
			execvp(args[0],args);
		}
	
	}
	else{
	
		setpgid(c1,c);
	
		char* args[] = {"./P3",NULL};
		execvp(args[0],args);
	}

	return 0;

}
