#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "circbuf.h"

int main() {
	
	char * memoire;
	//char memoire_partagee[8192];
	char buffer[128], precedent[128];
	int idshm;
	key_t cle;
	circbuf bufferCirculaire;
	
	cle = ftok("/etc/passwd", 1);
	idshm = shmget(cle, 0, 0);
	memoire = shmat(idshm, NULL, SHM_RND);
	
	//strcpy(memoire_partagee, memoire);
	//printf("%d\n", (int)sizeof(*memoire_partagee));
	
	bufferCirculaire = circbuf_init(memoire, sizeof(char[8192]));
	
	
	for(;;) {
		strcpy(precedent, buffer);
		printf("> ");
		fgets(buffer, sizeof(buffer), stdin);
		
		if(!strcmp(buffer, "quitter\n"))
			break;
		else if(!strcmp(buffer, "copier\n"))
		{
			circbuf_append(bufferCirculaire, precedent);
			
		}
		else if(strstr(buffer, "coller") != NULL)
		{
			if(!strcmp(buffer, "coller\n")){
				strcpy(buffer, circbuf_get(bufferCirculaire, 0));
			}
			else{
				char * numero = strrchr(buffer, 'r') + 1;
				strncpy(numero, numero, strlen(numero) - 2);
				strcpy(buffer, circbuf_get(bufferCirculaire, atoi(numero)));
			}
			
			printf("%s", buffer);
		}
		else if(!strcmp(buffer, "liste\n"))
		{
			for(int i = 0; i < circbuf_count(bufferCirculaire); i++){
				printf("%d\t%s", i, circbuf_get(bufferCirculaire, i));
			}
		}
		else
			printf("%s", buffer);
	}
	
	
	circbuf_shut(bufferCirculaire);
	shmdt(memoire);
	return 0;
}
