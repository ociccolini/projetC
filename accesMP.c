#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h> 
#include <signal.h>

int main(){
	char * monFichier = "/tmp/monPressePapier.conf";
	key_t cle;
	int idshm;
	char *p;
	int i = 0;

	if(!fopen(monFichier, "r")){
		perror("\nImpossible de mapper le fichier.\n");
	}
	cle = ftok(monFichier, 1);

	idshm = shmget(cle, 8192, 0);
	if(idshm == -1){
		perror("\nLa mémoire partagée demandée n'existe pas.\n");
	}

	// Attacher
	p=shmat(idshm, NULL, SHM_RND);
	
	// Afficher le contenu de la mémoire partagée
	while(p[i] != '\0'){
		printf("%c", p[i]);
		i++;
	}
	printf("\n");
	
	return 0;
}

