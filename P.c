#include<stdio.h>
#include<pthread.h>
#include <stdlib.h>
int number[20]={1,1,1,1,1,1,1,1,1,1,11,12,13,14,15,16,17,18,19,20};
void *summing(void *arg)
{
	int index=*(int *)arg;
	int sum=0;
	int size = sizeof(number) / sizeof(int);
	size=size/2;
	for(int j=0;j<size;j++){
		sum+=number[index+j];
	}
	printf("Local Sum: %d\n",sum);
	*(int *)arg=sum;
	return arg;
}
int main(int argc,char *argv[]){
	pthread_t th[2];
	int i;
	for(i=0;i<2;i++){
		int* a=malloc(sizeof(int));
		*a=i*10;
		pthread_create(&th[i],NULL,&summing,a);
	}
	int globalSum=0;
	for (i=0;i<2;i++){
		int *r;
		pthread_join(th[i],(void **)&r);
		globalSum+= *r;
		free(r);
	}
	printf("Final Sum: %d\n",globalSum);
	return 0;
}