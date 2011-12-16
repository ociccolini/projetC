#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "circbuf.h"

volatile int idshm;

void detruire_shm(int signum){
	printf("Fermeture de la memoire partagée.\n");
	shmctl(idshm, IPC_RMID, NULL);
	exit(0);
}

int main() {
	
	key_t cle;
	char * memoirePartagee;
	
	cle = ftok("/etc/passwd", 1);
	idshm = shmget(cle, 8192, IPC_CREAT | IPC_EXCL | 0760);
	memoirePartagee = shmat(idshm, 0, SHM_RND);
	
	circbuf_format(memoirePartagee, sizeof(memoirePartagee));
	
	
	shmdt(memoirePartagee);
	
	
	signal(SIGINT, detruire_shm);
	printf("\nUn espace mémoire est partagé,\n");
	printf("\nAppuyez sur ctrl-c pour quitter.\n");
	pause();
	return 0;
}
