#include <stdio.h>

int main()
{
    /*for printing process id */
    int pid;
    pid = getpid();
    if ((pid = getpid()) < 0)
    {
        perror("Cannot get pid\n");
    }
    else
    {
        printf("The process id is %d\n", pid);
    }
    /* for printing parent process id */
    int ppid;
    ppid = getppid();
    if (ppid < 0) { 
        printf("Error");
    } 
   else 
   {
   printf("The parent process id is %d\n", ppid);
   }
   
   
    int i;
int x[15];
FILE* data;
data=fopen("data.txt","r");
	if (data==NULL){
		printf("Error opening file \n");
		return 0;
	}
	else{
		for (i=0; i<15; i++){
			fscanf(data, "%d \n" , &x[i]);
		}
	}
	
	fclose(data);

int max = x[0];
    
    for(int k =0; k<15; k++){
        if(x[k]>max){
            max= x[k];
        }
        else max = max;
    }
	printf("Max=%d", max);
printf("\n");
int min = x[0];
    
    for(int m =0; m<15; m++){
        if(x[m]<min){
            min=x[m];
        }
        else min = min;
    }
	printf("Min=%d", min);
printf("\n");
int sum = 0;
	for (int z=0; z<15; z++){
	    sum=sum+x[z];
	}
	printf("Sum=%d", sum);
	return 0;
}
