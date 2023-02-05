#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

int main(int argc,char* argv[]){

	printf("This is process 3\n");
	
	int n = argc;
	
	if( n == 1) return 0;
	
	const char* exec = "./";
	
	char* main = (char*)malloc(strlen(exec)+1);
	strcpy(main,exec);
	
	for(int i = 1;i<n;i++){
		strcat(main,argv[i]);
             if(i!=n-1) strcat(main," ");
	}
	
	//printf("%s\n",main);
	
	FILE* fp = popen(main,"r");
	int fd = fileno(fp);
	dup2(fd,0);
	
	char recv[100];
	
	while(fgets(recv,100,stdin) != NULL){
		printf("%s",recv);
	}
	
	return 0;

}
