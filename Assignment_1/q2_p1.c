#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

int main(){
    int fd1[2],fd2[2];
    pipe(fd1);
    pipe(fd2);
    pid_t p=fork();
    if(p>0){
        close(fd1[0]);
        close(fd2[1]);
        while(1){
            char pmsg[100],buffer[100];
            printf("P : ");
            fgets(pmsg,100,stdin);
            if(strlen(pmsg)==1) exit(1);
            write(fd1[1],pmsg,strlen(pmsg)+1);
            read(fd2[0],buffer,100);
            printf("P : C sent %s",buffer);
        }
    }
    else{
        close(fd1[1]);
        close(fd2[0]);
        int temp=dup(0);
	char in[10];
	sprintf(in,"%d",temp);
        temp=dup(1);
	char out[10];
	sprintf(out,"%d",temp);
        dup2(fd1[0],0);
        dup2(fd2[1],1);
        close(fd1[0]);
        close(fd2[1]);
        system("gcc -o q2_p2 q2_p2.c");
        char* args[]={"q2_p2",in,out,NULL};
        execv(args[0],args);
    }
}
