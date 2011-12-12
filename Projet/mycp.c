#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>

char * memoirePartagee;

void coller()
{
	FILE* fichierSource = NULL;
	FILE* fichierCible = NULL;
	char chaine[1000] = "";
		
	char * nomFichierCible = strrchr(memoirePartagee, '/') + 1;
	char * reponse;
	
	
	if(fopen(nomFichierCible, "r"))
	{
		printf("Le fichier %s existe déjà\n", nomFichierCible);
		printf("Souhaitez vous l'écraser? (y/n) : ");
		fgets(reponse, sizeof(reponse), stdin);
		if(strcmp(reponse, "y\n"))
		{
			printf("Abandon!\n");
			exit(0);
		}
		
		printf("Fichier %s écrasé!\n", nomFichierCible);
	}
	
	
	fichierSource = fopen(memoirePartagee, "r");
	fichierCible = fopen(nomFichierCible, "w+");
	
	if(fichierSource != NULL && fichierCible != NULL)
	{
		
		while (fgets(chaine, 1000, fichierSource) != NULL)
		{
			fputs(chaine, fichierCible);
		}
		
		fclose(fichierSource);
		fclose(fichierCible);
	}
}

int main(int argc, char *argv[])
{
	
	key_t cle;
	int idshm;
	char * argumentCommande[3];
	
	cle = ftok("/etc/passwd", 1);
	idshm = shmget(cle, 0, 0);
	memoirePartagee = shmat(idshm, NULL, SHM_RND);
	
	
	if(argc > 1) {
		
		if(!strcmp(argv[1], "-a"))
		{
			coller();
		}
		else {
			if(fopen(argv[1], "r")) {
				char c = (argv[1])[0];
				if(c != '/') {
					char path[100];
					getcwd(path, sizeof(path));
					argv[1] = strcat(strcat(path, "/"), argv[1]);
				}
				printf("%s\n", argv[1]);
				strcpy(memoirePartagee, argv[1]);
			}
			else {
				printf("Le fichier '%s' n'existe pas\n", argv[1]);
			}
		}
			
	}
	else {
		printf("Veuillez entrer un nom de fichier!\n");
	}
	
	
	
	shmdt(memoirePartagee);
	
	return 0;
}
