#include<stdio.h>
#include<stdlib.h>

int main(){
	
	int* arr;
	
	int n;
	scanf("%d",&n);
	
	arr = (int*) malloc(sizeof(int) * n);
	
	for(int i=0;i<n;i++) scanf("%d",&arr[i]);
	
	int sum = 1;
	
	for(int i=0;i<n;i++){
		sum *= arr[i];
	}
	
	printf("%d",sum);
	
	return 0;
}
