#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

int main(){

	int c;
	
	//pfd1 write - parent read - child
	//pfd2 write - child read - parent
	
	int pfd1[2],pfd2[2];
	
	pipe(pfd1);
	pipe(pfd2);
	
	c = fork();
	
	if(c>0){
		close(pfd1[0]);
		close(pfd2[1]);
		
		char buffer[100];
		
		while(1){
		
		
			printf("Enter message to be sent by parent :\n");
			fgets(buffer,100,stdin);
			write(pfd1[1],buffer,100);
			
			//wait();
			
			read(pfd2[0],&buffer,100);
			if(strlen(buffer) == 1) break;
			printf("message sent by child to parent : ");
			printf("%s\n",buffer);
		
		}
	}
	else{
	
		close(pfd1[1]);
		close(pfd2[0]);
		
		char buffer[100];
		
		while(1){
			read(pfd1[0],&buffer,100);
			
			
			if(strlen(buffer) == 1) break;
			
			printf("Message sent by parent to child : ");
			printf("%s\n",buffer);
			
						
			printf("Enter message to be sent to parent :\n");
			fgets(buffer,100,stdin);
			
			
			
			write(pfd2[1],buffer,100);
		
		}
		
		
	
	}

	
	return 0;

}
