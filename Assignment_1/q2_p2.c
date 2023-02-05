#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

int main(int argc,char* argv[]){
	int in = atoi(argv[1]),out=atoi(argv[2]);
    int fd0=dup(0);
    int fd1=dup(1);
    dup2(in,0);
    dup2(out,1);
    printf("%s\n",argv[1]);
    printf("%s\n",argv[2]);
    while(1){
        char cmsg[100],buffer[100];
        read(fd0,buffer,100);
        printf("C : P sent %s",buffer);
        printf("C : ");
        fgets(cmsg,100,stdin);
        if(strlen(cmsg)==1) exit(1);
        write(fd1,cmsg,strlen(cmsg)+1);
    }
}