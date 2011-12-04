#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

int main()
{
	char *p;
	char prev[1024]="", buf[1024];
	int idshm;
	key_t k;
	
	k=ftok("/etc/passwd", 1);
	idshm=shmget(k, 0, 0);
	if(idshm<0)
	{
		perror("Récupération de la mémoire partagée");
		return errno;
	}
	p=(char*)shmat(idshm, NULL, SHM_RND);
	for(;;)
	{
		printf("> ");
		fgets(buf, sizeof(buf), stdin);
		
		if(!strcmp(buf, "quit\n"))
			break;
		else if(!strcmp(buf, "copy\n"))
		{
			strcpy(p, prev);
			strcpy(buf, prev);
		}
		else if(!strcmp(buf, "paste\n"))
			strcpy(buf, p);
		strcpy(prev, buf);
		printf("%s", buf);
	}
	shmdt(p);
	return 0;
}
