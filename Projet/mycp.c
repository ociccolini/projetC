#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>
#include "circbuf.h"

void coller(const char * nomFichierSource)
{
	FILE* fichierSource = NULL;
	FILE* fichierCible = NULL;
	char chaine[1000] = "";
		
	char * nomFichierCible = strrchr(nomFichierSource, '/') + 1;
	char reponse[10];
	
	
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
	
	
	fichierSource = fopen(nomFichierSource, "r");
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
//	char memoirePartagee[8192];
	char * memoire;
	circbuf bufferCirculaire;
	
	
	cle = ftok("/etc/passwd", 1);
	idshm = shmget(cle, 0, 0);
	memoire = shmat(idshm, NULL, SHM_RND);
	
	//c = circbuf_init(mem, sizeof(char[1024]));
	
//		strcpy(memoirePartagee, memoire);
	
	bufferCirculaire = circbuf_init(memoire, sizeof(char[8192]));
	printf("%d\n", circbuf_count(bufferCirculaire));
	
	if(argc > 1) {
		
		if(!strcmp(argv[1], "-a"))
		{
			printf("%d\n", circbuf_count(bufferCirculaire));
			for(int i = 0; i < circbuf_count(bufferCirculaire); i++)
				coller(circbuf_get(bufferCirculaire, i));
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
				//strcpy(memoirePartagee, argv[1]);
				circbuf_append(bufferCirculaire, argv[1]);
				printf("%d\n", circbuf_count(bufferCirculaire));
			}
			else {
				printf("Le fichier '%s' n'existe pas\n", argv[1]);
			}
		}
			
	}
	else {
		printf("Veuillez entrer un nom de fichier!\n");
	}
	
	
	circbuf_shut(bufferCirculaire);
	shmdt(memoire);
	
	return 0;
}
