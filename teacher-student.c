#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>

// Variables used 

int table_used=1;
int generated=0;
int item_gen[2]={0};

char *item[3]={"question paper","sheet","pen"}; 

// q --> " Question paper "
// s --> " Sheet "
// p --> " Pen "

sem_t t;

// Teachers function

void *teacher()
{
	int i=0,j=0,k=0;
	do
	{
		sem_wait(&t);
		if(table_used==1)
		{
			i=k;
			j=i+1;
			if(j==3)
				j=0;
			k=j;
			item_gen[0]=i;
			item_gen[1]=j;
			printf("teacher puts %s and %s\n",item[i],item[j]);
			generated=1;
			table_used=0;
		}
		sem_post(&t);
	}
	while(1);
}

// Student's function

void *student(void *i)
{
	sem_wait(&t);
	int *val=(int *)i;
	if(table_used==0)
	{
		if(generated && item_gen[0]!=*val && item_gen[1]!=*val)
		{
			printf("student %d has done with his exam \n",*val);
			table_used=1;
			generated=0;
		}
	}
	sem_post(&t);
}

int main()
{
	pthread_t s1,s2,s0,teachr;
	sem_init(&t,0,1);
	int m=0,j=1,k=2;
	
	printf("Student 0 has question paper \n");
	printf("Student 1 has sheet \n");
	printf("Student 2 has pen \n");
	pthread_create(&teachr,NULL,teacher,NULL);
	
	while(1)
	{
		pthread_create(&s0,NULL,student,(void*)&m);
		pthread_create(&s1,NULL,student,(void*)&j);
		pthread_create(&s2,NULL,student,(void*)&k);
	
		pthread_join(s0,NULL);
		pthread_join(s1,NULL);
		pthread_join(s2,NULL);
		sleep(1);
	}
	
	pthread_join(teachr,NULL);
	
	return 0;
}

// note this program will not terminate. press control + c to stop the execution 
