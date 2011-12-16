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

void afficher_aide()
{
	printf("Utilisation : ./mycp fichier\n");
	printf("         ou : ./mycp [OPTION]\n\n");
	printf("Voici les options utilisables :\n");
	printf("-a\t\tColler tous les fichiers précédemment copiés\n");
	printf("-l\t\tLister tous les fichiers précédemment copiés\n");
	printf("-h\t\tAfficher le manuel\n");
	printf("-[n]\t\tColler le fichier numéro 'n' précédemment copiés\n");
}

int main(int argc, char *argv[])
{
	
	key_t cle;
	int idshm;
	char * memoire;
	circbuf bufferCirculaire;
	
	
	cle = ftok("/etc/passwd", 1);
	idshm = shmget(cle, 0, 0);
	memoire = shmat(idshm, NULL, SHM_RND);
	

	bufferCirculaire = circbuf_init(memoire, sizeof(char[8192]));
	
	if(argc > 1) {
		if((argv[1])[0] == '-')
		{
			if((argv[1])[1] == 'a')
			{
				for(int i = 0; i < circbuf_count(bufferCirculaire); i++)
					coller(circbuf_get(bufferCirculaire, i));
			}
			else if((argv[1])[1] == 'l')
			{
				for(int i = 0; i < circbuf_count(bufferCirculaire); i++)
					printf("%d\t%s\n", i, strrchr(circbuf_get(bufferCirculaire, i), '/') + 1);
			}
			else if((argv[1])[1] == 'h')
			{
				afficher_aide();
			}
			else
			{		
				coller(circbuf_get(bufferCirculaire, atoi(&(argv[1])[1])));
			}
		}
		else{
			if(fopen(argv[1], "r")) {
				char c = (argv[1])[0];
				if(c != '/') {
					char path[100];
					getcwd(path, sizeof(path));
					argv[1] = strcat(strcat(path, "/"), argv[1]);
				}
				printf("%s\n", argv[1]);
				circbuf_append(bufferCirculaire, argv[1]);
			}
			else {
				printf("Le fichier '%s' n'existe pas\n", argv[1]);
			}
		}
			
	}
	else {
		printf("Veuillez entrez un paramètre ou un nom de fichier!\n");
		printf("Pour plus d'informations, utilisez l'aide en saisissant 'mycp -h'\n");
	}
	
	
	circbuf_shut(bufferCirculaire);
	shmdt(memoire);
	
	return 0;
}
