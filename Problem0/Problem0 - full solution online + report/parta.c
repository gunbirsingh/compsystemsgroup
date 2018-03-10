#include <stdio.h>
#include <time.h>

int main() 
{
    
    int num;
    int min = 999; 
    int max = 0;
    int sum= 0;

    FILE *ifp;
    FILE *ofp;

    ifp = fopen("file.txt", "r");
    ofp = fopen("Output partA.txt", "w");
     while(fscanf(ifp,"%d",&num) == 1)
     {
          if (num < min)
             min = num;
          if (num > max)
             max = num;
     }
	sum= (min+ max+ num);
    
fclose(ifp);

    fprintf(ofp, "Minimum output is = %d \n", min);
    fprintf(ofp, "Maximum output is =  %d \n", max);
    fprintf(ofp, "Sum is = %d \n", sum);

    
   

fclose(ofp);
    return 0; 
}

