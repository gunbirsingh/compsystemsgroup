#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>



typedef struct data {
	char name[10];
	double age;
} data;

int alrmflag=0;
int lock=0;

void sig_func(int sig){
	write(1, "Caught signal no = %d\n", sig);
	printf("First sig func caught signal no = %d\n",sig);
	signal(sig,sig_func);
}

void sig_func2(int sig){
	write(1, "Caught signal no = %d\n", sig);
	printf("Sig func2 caught signal no= %d\n", sig);
	alrmflag=1;
}

int test_and_set(int *memory)
{
	if(*memory){
	return 1;
	}	
	else{			
	  *memory = 1;
	  return 0;
	}
}

void func(data *p){
	int x;
	snprintf(p->name,10,"%d\n",(int) pthread_self());
	p->age= clock( )/ CLOCKS_PER_SEC;
	do {
	  if (lock ==0){
	    __sync_lock_test_and_set(&lock, 1);
	    continue;
	  }
	  x=(int)(((int)pthread_self()+getpid())/((getpid()*__sync_lock_test_and_set(&lock,1))));
	}while(!lock);
	sleep(50); //sleep to catch signals
}

int main (){
	pthread_t tid1, tid2, tid3;
	//pthread_attr_t attr = NULL;
	data d;
	data *ptr=&d;
	ptr = (data *)malloc(10*sizeof(char) + sizeof(double));
	int pid;
	signal (SIGINT,SIG_IGN);
	pthread_create(&tid1,NULL,(void*)func,ptr);
	signal(SIGSEGV,sig_func);
	signal(SIGSTOP,sig_func);
	pthread_create(&tid2,NULL,(void*)func,ptr);
	signal(SIGFPE,sig_func);
	signal(SIGALRM,sig_func2);
	signal(SIGINT,sig_func2);
	pthread_create(&tid3,NULL,(void*)func,ptr);

	pid=getpid();
	sleep(10);
	printf("Line A\n");
	pthread_kill(tid1,SIGSEGV); 	//Line A 
	sleep(5);
	printf("Line B\n");
	pthread_kill(tid2,SIGSTOP); 	//Line B
	printf("Line C1\n");
	alarm(3);			//Line C(1)
	printf("Line C2\n");
	while(!alrmflag) pause(); 	//Line C(2)
	printf("Line D\n");
	pthread_kill(tid1,SIGINT);	//Line D
	printf("Line E\n");
	pthread_kill(tid3,SIGINT);	//Line E
	sleep(40);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	pthread_join(tid3,NULL);

}
