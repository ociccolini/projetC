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
	FILE* f;

	void traitementArret(int signal){

		printf("\nDésallocation de la mémoire partagée.\n");
		shmctl(idshm, IPC_RMID, NULL);
		exit(0);
	}

	if(!fopen(monFichier, "a")){
		perror("\nImpossible de créer le fichier.\n");
	}

	cle = ftok(monFichier, 1);

	// Créer
	idshm = shmget(cle, 8192, IPC_CREAT | IPC_EXCL|0770);

	// Désalouer la mémoire partagée sur ctrl -c
	signal(SIGINT, traitementArret);

	if(idshm == -1){
		perror("\nL'allocation de la mémoire partagée a échouée.\n");
	}

	// Attacher
	p=shmat(idshm, NULL, SHM_RND);
	
	printf("\nUn espace mémoire est partagé,\n");
	printf("\nAppuyez sur ctrl-c pour quitter.\n");

	strcpy(p, "Premier test ... ! \n sur plusieurs lignes.");
	strcpy(p, "deuxième test ... ! \n sur plusieurs lignes.");

	// Attendre le signal ctrl -c
	pause();
	
	return 0;
}

