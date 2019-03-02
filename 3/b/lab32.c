#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float scal_pr(int, float*, float*);

int main(int argc, char** argv){
	
	printf("Enter the length of your vectors\n");
	
	char string[128];
	scanf("%s",string);
	//printf("%ld",strlen(string));
	long i;
	int flag=1;
	for (i=0; i < strlen(string); i++){
		if (string[i] < '0' || string[i] > '9'){
			//printf("check");
			flag=0;
		}

    }
	
	if(flag){
		long len;
		len=atol(string);
		
		float a[len];
		float b[len];
		printf("Enter elements for 1 vector:\n");
		for(i=0;i<len;i++){
			scanf("%f",&a[i]);	
		}
		
		printf("Enter elements for 2 vector:\n");
		for(i=0;i<len;i++){
			scanf("%f",&b[i]);	
		}
		
		//printf("ok");
		float rez=scal_pr(len,a,b);
		printf("Your rezult is: %f\n",rez);
		
	}else{
		printf("Entered line contains invalid symbols\n");
	}
	
	
	return 0;
}


float scal_pr(int len, float* a,float* b){
	int i;
	int sum=0;
	for(i=0; i < len; i++){
		sum+=a[i]*b[i];
	}
	return sum;
}


