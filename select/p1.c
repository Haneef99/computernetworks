#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<sys/select.h>
#include<unistd.h>
#include<fcntl.h>


int main(){

	struct timeval tv;
	int fd1 = fileno(popen("./p2","r"));	
	int fd2 = fileno(popen("./p3","r"));
	
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	
	fd_set rfds;
	FD_ZERO(&rfds);
	
	int max = fd1 > fd2? fd1: fd2;
	while(1){
		FD_SET(fd1,&rfds);
		FD_SET(fd2,&rfds);
		int ret = select(max + 1,&rfds,NULL,NULL,&tv);
		char buff[100];
		
		if(ret < 0){
			printf("select failed\n");
			return -1;
		}
		else if(ret == 0) {printf("timeout occured\n"); break;}
		else{
		
			int flag = 1;
		
			if(FD_ISSET(fd1,&rfds)){
				flag = 0;
				int size = read(fd1,buff,100);
				if(size == 0) {printf("wrw");FD_CLR(fd1,&rfds);}
				else printf("%s\n",buff);
			}
			if(FD_ISSET(fd2,&rfds)){
				flag = 0;
				int size = read(fd2,buff,100);
				if(size == 0) FD_CLR(fd2,&rfds);
				else printf("%s\n",buff);
			}
			
			if(flag) break;
		}
		
		
		
	}
	close(fd1);
	close(fd2);
	
	return 0;
		
}
