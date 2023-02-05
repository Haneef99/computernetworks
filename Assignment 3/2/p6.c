#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){

	char buff[100];

	while(1){

		while(read(0,buff,100) != -1){
			
			printf("%s\n",buff);}
	}
	return 0;

}
