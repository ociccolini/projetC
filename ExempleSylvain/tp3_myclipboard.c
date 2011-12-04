#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

volatile int idshm;	// volatile mot clé utile pour les variables utilisées dans les handler d'exception

void detruire_shm(int signum) 
{
	shmctl(idshm, IPC_RMID, NULL);
	exit(0);
}

int main()
{
	char *p;
	key_t k;
	
	k=ftok("/etc/passwd", 1);
	idshm=shmget(k, 1024, IPC_CREAT|IPC_EXCL|0700);
	if(idshm<0)
	{
		perror("Création de la mémoire partagée");
		return errno;
	}
	p=(char*)shmat(idshm, NULL, SHM_RND);
	strcpy(p, "");
	shmdt(p);
	signal(SIGINT, detruire_shm);
	pause();
	return 0;
}
