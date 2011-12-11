#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	
	char * memoire_partagee;
	char buffer[1024], precedent[1024];
	int idshm;
	key_t cle;
	
	cle = ftok("/etc/passwd", 1);
	idshm = shmget(cle, 0, 0);
	memoire_partagee = shmat(idshm, NULL, SHM_RND);
	
	for(;;) {
		strcpy(precedent, buffer);
		printf("> ");
		fgets(buffer, sizeof(buffer), stdin);
		
		if(!strcmp(buffer, "quitter\n"))
			break;
		else if(!strcmp(buffer, "copier\n"))
		{
			strcpy(memoire_partagee, precedent);
		}
		else if(!strcmp(buffer, "coller\n"))
		{
			strcpy(buffer, memoire_partagee);
			printf("%s", buffer);
		}
		else
			printf("%s", buffer);
	}
	
	
	shmdt(memoire_partagee);
	return 0;
}
