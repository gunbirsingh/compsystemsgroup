#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <signal.h>
#include "CycleTimer.h"

typedef struct data {
	char name[10];
	double age;
} data;

int alrmflag=0;

void sig_func(int sig){
	write(1, "Caught signal no = %d\n", sig);
	signal(sig,sig_func);
}

void sig_fun2(int sig){
	write(1, "Caught signal no = %d\n", sig);
	alrmflag=1;
}

void func(data *p){
	int x;
	snprintf(p->"name,10,%d",(int) pthread_self() );	
	p->age=CycleTimer::currentSeconds();			//file for CycleTimer.h ?? doesnt matter
	x=(int)(((int)pthread_self()+getpid())/((getpid()*(int)test_and_set(&lock)));
	sleep(50); //sleep to catch signals
}

int main (){
	pthread_t tid1, tid2, tid3;
	pthread_attr_t attr;
	data d;
	data *ptr=&d;
	int pid, lock=0;
	signal (SIGINT,SIG_IGN);
	pthread_create(&tid1,&attr,(void*)func,ptr);
	signal(SIGSEGV,sig_func);
	signal(SIGSTOP,sif_func);
	pthread_create(&tid2,&attr,(void*)func,ptr);
	signal(SIGFPE,sig_func);
	signal(SIGALRM,sig_func2);
	signal(SIGINT,sig_func2);
	pthread_create(&tid3,&attr,(void*)func,ptr);

	pid=getpid();
	sleep(10);
	pthread_kill(tid1,SIGSEGV); //Line A 
	sleep(5);
	pthread_kill(tid2,SIGSTOP); //Line B
	alarm(3);					//Line C(1)
	while(!alrmflag pause); 	//Line C(2)		//formatting ???
	pthread_kill(tid1,SIGINT);	//Line D 
	pthread_kill(tid3,SIGINT);	//Line E
	sleep(40);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	pthread_join(tid3,NULL);


}