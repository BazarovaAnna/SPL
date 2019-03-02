#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int is_prime(unsigned long);

int main(int argc, char** argv){
	
	printf("Enter a number and I'll check if it is prime\n");
	
	char string[128];
	scanf("%s",string);
	//printf("%ld",strlen(string));
	long i=0;
	int flag=1;
	for (; i < strlen(string); i++){
		if (string[i] < '0' || string[i] > '9'){
			//printf("check");
			flag=0;
		}

    }
	
	if(flag){
		unsigned long x;
		x=atol(string);
		//scanf("%lu",&x);
		if(is_prime(x)){
			printf("Yes\n");
		}else{
			printf("No\n");
		}
		//printf("%d\n",is_prime(x));
	}else{
		printf("Entered line contains invalid symbols\n");
	}
	
	i=3;
	i=i++ + ++i;
	printf("%l",&i);
	
	return 0;
}

int is_prime(unsigned long x){
	if(x<2){
		return 0;
	}
	int i=2;
	for( ; i<x; i=i+1){
		if(x%i==0){
			return 0;
		}
	}
	return 1;
}
