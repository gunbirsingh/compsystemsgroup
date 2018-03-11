#include <stdio.h>


int main()
{
    FILE* data;
	FILE* output;
	data=fopen("data.txt","r");
	output=fopen("Output PartA.txt", "w");

	/*for printing process id */
    int pid;
    pid = getpid();
    if ((pid = getpid()) < 0)
    {
        perror("Cannot get pid\n");
    }
    else
    {
        fprintf(output, "The process id is %d\n", pid);
    }
    
    /* for printing parent process id */
    int ppid;
    ppid = getppid();
    if (ppid < 0) { 
        printf("Error");
    } 
   else 
   {
   fprintf(output,"The parent process id is %d\n", ppid);
   }
   
    int i;
	int x;

	int min=100000; //arbitrary large number so that the first number smaller than this becomes the new min
	int max=0;
	int sum=0;

	if (data==NULL){
		printf("Error opening file \n");
		return 0;
	}
	else{
		while (fscanf(data, "%d\n", &x)!=EOF){
			if (x < min)
             min = x;
          	if (x > max)
             max = x;
         	for (int y=0; y<sizeof(data)/5; y++){
         		sum=x+sum;
         	}
		}
	}
	
	fclose(data);
	fprintf(output, "Max= %d\n", max);
	fprintf(output, "Min= %d\n", min);
	fprintf(output, "Sum= %d\n", sum);
	printf("Max=%d\n",max);
	printf("Min=%d\n",min);
	printf("Sum=%d\n",sum);

}
