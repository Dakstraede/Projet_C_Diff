#include "Header.h"
#include <stdio.h>
#include <stdlib.h>

//Options fonctions of the diff command

int strcomp1(const char* s1, const char* s2)
{
	while ((*s1++ == *s2++) && (*s1 != '\0'));
	return (*((unsigned char *)--s1) < *((unsigned char *)--s2)) ? -1 : (*(unsigned char *)s1 != *(unsigned char *)s2);
}

void help_option()
{
	printf("Usage: diff [OPTION]... FICHIERS\n");
	printf("Comparer les fichiers ligne par ligne.\n");
	printf("Les param%ctres requis pour les options longues sont %cgalement requis pour les options courtes.\n", 138, 130);
	printf("--normal                  Produire un %c diff %c en format normal (par d%cfaut)\n", 174, 175, 130);
	printf("-q, --brief                   Indiquer seulement si les fichiers diff%crent\n", 130);
	printf("-s, --report-identical-files  Indiquer si les deux fichiers sont identiques\n");
	printf("-y, --side-by-side            Affichage sur deux colonnes\n");
	printf("-t, --expand-tabs             %ctaler les tabulateurs en espaces dans la sortie\n", 144);
	printf("-i, --ignore-case            Ignorer les diff%crences de casses dans le contenu des fichiers\n", 130);
	printf("-a, --text                    Traiter tous les fichiers comme des textes\n");
	printf("-h, --help                    Afficher cette aide et terminer\n");
	printf("-v, --version                 Afficher le nom et la version du logiciel et terminer\n");
	printf("FICHIERS sont %c FICHIER1 FICHIER2 %c ou %c R%cP1 R%cP2 %c ou %c R%cP FICHIER... %c ou %c FICHIER... R�P %c.\n", 174, 175, 174, 144, 144, 175, 174, 144, 175, 174, 175);
	printf("\n");

	//suite des d�tails d'options
}

/*Count lines
There is no need to define the max length  of a line
*/
unsigned long linesOfFile(char* file)
{
	FILE* fileParam = NULL;
	int  c;
	unsigned long newline_count = 1;
	fileParam = fopen(file, "r");

	if (fileParam != NULL)
	{
		/* count the newline characters */
		while ((c = fgetc(fileParam)) != EOF) {
			if (c == '\n')
				newline_count++;
		}
		fclose(fileParam);

		return newline_count;


	}
	else
	{
		printf("Impossible de lire le fichier");
		return -1;
	}

}

unsigned long linesOfTab(char *tab)
{
	unsigned long newLineCount = 1;
	if (tab != NULL)
	{/*count the newline characters*/
		unsigned long j = 0;

		while (tab[j] != '\0')
		{
			//printf("%i\n", j);
			if (tab[j] == '\n')
			{
				newLineCount++;
			}
			j++;
		}

	}
	else
	{
		return 0;
	}
	return newLineCount;
}

//Renvoie le nombre de caract�res dans un fichier
int CaractersOfFile(char* file)
{
	FILE* fileParam = NULL;
	int  c = 0;
	int cars = 0;
	fileParam = fopen(file, "r");

	if (fileParam != NULL)
	{
		/* count the numbers of characters */
		while ((c = fgetc(fileParam)) != EOF) {
			cars++;
		}
		fclose(fileParam);

		return cars;
	}
	else
	{
		printf("Impossible de lire le fichier");
		return -1;
	}
}

//Renvoie un tableau rempli de tous les caract�res du fichier
//Caract�re par caract�re
char* fileToTabs(char* file, int optionSpe)
{
	unsigned long i = 0;
	char* buffer;
	FILE* fileToTab = NULL;

	fileToTab = OpenAFile(file, optionSpe);
	unsigned long fileSize = GetFileSize(file, optionSpe);
	//calloc = allocation d'un tableau de 1 �l�ment de taille fileSize+1
	buffer = calloc(1, fileSize + 1);
	if (buffer == NULL)
	{
		exit(EXIT_FAILURE);
	}

	//Copie du fichier dans le buffer
	if (1 != fread(buffer, fileSize, 1, fileToTab))
	{
		fclose(fileToTab);
		free(buffer);
		fputs("entire read fails", stderr);
		exit(EXIT_FAILURE);
	}
	//fileSize = la taille du fichier mais indique aussi le dernier caract�re
	buffer[fileSize] = '\0';
	fclose(fileToTab);
	return buffer;
}

// Modifie le tableau existant et ajoute les caract�res d'une ligne dedans
void ajouterATableauLigneFichier(char *nomFichier, char *tabLigne, int numLigne, int tailleTableau)
{
	FILE *fichier = NULL;
	int caractereActuel = 0, i = 0;
	long position = 0;
	fichier = fopen(nomFichier, "r");
	position = retourLigneCurseur(nomFichier, numLigne);
	fseek(fichier, position, SEEK_SET);
	if (fichier != NULL)
	{
		for (i = 0; i<tailleTableau; i++)
		{
			caractereActuel = fgetc(fichier);
			tabLigne[i] = caractereActuel;
		}
	}
	fclose(fichier);
}

// Retourne la position du curseur au d�but de la ligne voulue
int retourLigneCurseur(char *nomFichier, int numLigne)
{
	FILE *fichier = NULL;
	int caractereActuel = 0, nb = 0, nombreBackSlashN = 0;
	fichier = fopen(nomFichier, "r");
	if (fichier != NULL)
	{
		while (nombreBackSlashN != numLigne)
		{
			caractereActuel = fgetc(fichier);
			if (caractereActuel == '\n' || caractereActuel == EOF)
			{
				nombreBackSlashN++;
			}
		}
		nb = ftell(fichier);
	}
	else
	{
		nb = 0;
	}
	fclose(fichier);
	return nb;
}

// Renvoie le nombre de caract�res pour une ligne
int nombreCaractereLigne(char *nomFichier, int numLigne)
{
	FILE *fichier = NULL;
	int caractereActuel = 0, nb = 0;
	long position = 0;
	fichier = fopen(nomFichier, "r");
	position = retourLigneCurseur(nomFichier, numLigne);
	fseek(fichier, position, SEEK_SET);
	if (fichier != NULL)
	{
		while (caractereActuel != '\n')
		{
			if (caractereActuel == '\n' || caractereActuel == EOF)
			{
				break;
			}
			caractereActuel = fgetc(fichier);
			nb++;
		}
	}
	else
	{
		nb = 1;
	}
	fclose(fichier);
	return nb - 1;
}

// Affichage de la ligne voulue
void afficheLigne(char *nomFichier, int numLigne)
{
	FILE *fichier = NULL;
	int caractereActuel = 0, i = 0;
	long position = 0;
	fichier = fopen(nomFichier, "r");
	position = retourLigneCurseur(nomFichier, numLigne);
	fseek(fichier, position, SEEK_SET);
	if (fichier != NULL)
	{
		while (caractereActuel != '\n' || caractereActuel == EOF)
		{
			if (caractereActuel == EOF)
			{
				break;
			}
			caractereActuel = fgetc(fichier);
			// On affiche tous les caract�res sauf les '\n' (retour � la ligne)
			if (caractereActuel != '\n')
			{
				printf("%c", caractereActuel);
			}
		}
	}
	fclose(fichier);
}

/*
Fonction -t  : Retourne le tableau en transformant les tabulations par un espace
CODE ASCII : TAB = 09
SP = 32
*/
char* tabToSpace(char *tab, int tailleTab)
{
	int i;
	if (tab != NULL)
	{
		for (i = 0; i < tailleTab; i++)
		{
			if (tab[i] == '\t')
			{
				tab[i] = ' ';
			}
		}
	}
	return tab;
}

/*Fonction ignoreCase
Renvoie 1 si c1!=c2 m�me en ignorant la casse
Renvoie 0 si c1==c2 en ignorant la casse
+32 et -32 = Table ACSII maj vers min ou min vers maj

A int�grer dans le diff principal si option -i pour chaque caract�re*/
int ignoreCase(char c1, char c2)
{
	if (c1 == c2)
	{
		return 1;
	}

	if (((c1 >= 65 && c1 <= 90) || (c1 >= 97 && c1 <= 122)) && ((c2 >= 65 && c2 <= 90) || (c2 >= 97 && c2 <= 122)))
	{
		if ((c1 == c2 + 32) || (c1 == c2 - 32))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 1;
	}
}

/*
Appel dans le diff apr�s les options de comportement
Dans le diff principal d�s qu'un caract�re diff�re on envoie 1

Sinon � la fin du diff principal si toujours aucun caract�re ne diff�re on envoie 0
*/
void briefing(int differ, const char *firstFile, const char *secondFile)
{
	if (differ == 1)
	{
		printf("Files %s and %s", firstFile, secondFile);
	}
	//Rien si 0
}

/*
-s N'empeche pas l'affichage du diff principale si les fichiers diff�rent
Appel � la fin du diff princpal apr�s toutes les options de comportement s'il y en a
Re�oie 1 si diff�rent
Rz�oie 0 si contenu identique
*/
void identicalFiles(int same, const char *firstFile, const char *secondFile)
{
	if (same == 0)
	{
		printf("\nContent of files %s and %s are the same", firstFile, secondFile);
	}
	//Rien si different
}

//Renvoie la taille du fichier en octet
unsigned long GetFileSize(char *file, int optionSpe)
{
	FILE* fileSize = NULL;
	fileSize = OpenAFile(file, optionSpe);

	if (fileSize != NULL)
	{
		unsigned long t = 0;
		fseek(fileSize, 0, SEEK_END);
		t = ftell(fileSize);
		fclose(fileSize);
		return t;
	}
	else
	{
		printf("Impossible de lire le fichier");
		return -1;
	}
}

//Renvoie un tableau rempli de tous les caract�res du fichier les tabulations ---> 8 espaces
//Donc ajout de 7 caract�res
//Caract�re par caract�re
char* fileToTabsOptionT(char* file, int optionSpe)
{
	unsigned long i = 0, nbEsc = 0;
	char* buffer;
	char* tabEsc;
	FILE* fileToTab = NULL;

	fileToTab = OpenAFile(file, optionSpe);
	unsigned long fileSize = GetFileSize(file, optionSpe);

	//calloc = allocation d'un tableau de 1 �l�ment de taille fileSize+1
	buffer = calloc(1, fileSize + 1);
	if (buffer == NULL)
	{
		exit(EXIT_FAILURE);

	}
	//Copie du fichier dans le buffer
	if (1 != fread(buffer, fileSize, 1, fileToTab))
	{
		fclose(fileToTab);
		free(buffer);
		fputs("entire read fails", stderr);
		exit(EXIT_FAILURE);
	}
	//fileSize = la taille du fichier mais indique aussi le dernier caract�re
	buffer[fileSize] = '\0';

	fclose(fileToTab);
	//Si caract�re tabulation \t nbEsc +7 : caract�re tab en moins + 8 espaces
	i = 0;
	while (buffer[i] != '\0')
	{
		if (buffer[i] == '\t')
		{
			nbEsc += 7;
		}
		i++;
	}

	fileSize += nbEsc;
	tabEsc = calloc(1, fileSize + 1);

	if (tabEsc == NULL)
	{
		exit(EXIT_FAILURE);

	}
	i = 0;
	int k = 0;
	unsigned long j = 0;

	while (buffer[i] != '\0')
	{
		if (buffer[i] == '\t')
		{
			k = 0;
			//si tab trouv� tabEsc est rempli de 8 espaces
			for (k = 0; k < 8; j++, k++)
			{
				//Code ASCII de espace = 32
				tabEsc[j] = 32;
			}
			//decrementation � cause de la boucle for qui va quand m�me incrementer jusqu'� 8
			j--;
		}
		else
		{
			tabEsc[j] = buffer[i];
		}
		j++;
		i++;
	}
	//j = derniere case du tabEsc
	tabEsc[j] = '\0';
	free(buffer);
	return tabEsc;
}

//Fonction diff principale (impl�mentation des options ult�rieure) (!!!!!! il faut une variable de nb de lignes de fichier !!!!!!)
void diff(char* oldFile, char* newFile, int lengthOldFile, int lengthNewFile, int nbCommonLines, int displayOption)
{
	//D�claration des indices de parcours des tableaux
	//et une variable pour g�rer la diff�rence
	int i = 0, j = 0, k;
	int isave = 0, jsave = 0;
	int beginLine = 1, lastCompLine = 1;
	int diff = 0;

	//D�claration d'un tableau contenant les tailles des lignes de chaque tableau:
	// lengths[0] = old ; lengths[1] = new
	int* lengths = malloc(2 * sizeof(int));
	lengths[0] = 0; lengths[1] = 0;

	length_line_from_idx(oldFile, newFile, i, j, lengths);

	while (i < lengthOldFile && j < lengthNewFile)
	{
		lastCompLine = beginLine;

		//test simple sur la longueur des lignes, si la taille est diff�rente, les lignes sont diff�rentes
		//si elles sont de taille �gale, on appelle la fonction de comparaison des lignes
		if (lengths[0] != lengths[1]){
			diff = 1;
		}
		else{
			diff = compare_line(lengths, i, j, oldFile, newFile);
		}


		if (diff == 1){
			//passage au 1er indice de la ligne suivante pour les deux fichiers
			//et recalcul de la taille de la ligne suivante
			//les variables save correspondent � l'indice de d�part pour l'affichage
			isave = i;
			jsave = j;
			i += lengths[0];
			j += lengths[1];
			length_line_from_idx(oldFile, newFile, i, j, lengths);

			while (compare_line(lengths, i, j, oldFile, newFile) == 1 && lastCompLine <= nbCommonLines){
				lastCompLine++;
				i += lengths[0];
				j += lengths[1];
				length_line_from_idx(oldFile, newFile, i, j, lengths);
			}

			//AFFICHAGE : cas o� il y a plus d'une ligne diff�rente
			if (beginLine != lastCompLine){
				//affichage de la ligne avec le code de changement
				printf("%i,%ic%i,%i \n", beginLine, lastCompLine, beginLine, lastCompLine);

				//gestion du cas o� la premi�re ligne est diff�rente (on affiche la premi�re ligne avant le traitement)
				printf("<");
				if (displayOption == 1)
				{
					printf("\t");
				}
				while (oldFile[isave] != '\n'){
					printf("%c", oldFile[isave]);
					isave++;
				}
				//affichage des lignes du 1er fichier avec <"
				printf("%c", oldFile[isave]);
				for (k = isave; k < i; k++){
					if (oldFile[k - 1] == '\n'){
						printf("<");
						if (displayOption == 1)
						{
							printf("\t");
						}
					}

					printf("%c", oldFile[k]);
				}

				//s�paration
				printf("\n - - - \n");

				//affichage 1�re ligne 2�me fichier
				printf("<");
				if (displayOption == 1)
				{
					printf("\t");
				}
				while (newFile[jsave] != '\n'){
					printf("%c", newFile[jsave]);
					jsave++;
				}
				printf("%c", newFile[jsave]);

				//affichage des lignes du nouveau fichier avec ">"
				for (k = jsave; k < j; k++){
					if (newFile[k - 1] == '\n'){
						printf(">");
						if (displayOption == 1)
						{
							printf("\t");
						}
					}
					printf("%c", newFile[k]);
				}
				beginLine = lastCompLine;
			}

			//AFFICHAGE : cas o� une seule ligne diff�re
			else{
				printf("%ic%i", beginLine, beginLine);
				printf("<");
				if (displayOption == 1)
				{
					printf("\t");
				}
				for (k = isave; k < i; k++)
					printf("%c", oldFile[k]);

				printf("\n - - - \n");

				printf(">");
				if (displayOption == 1)
				{
					printf("\t");
				}
				for (k = jsave; k < j; k++)
					printf("%c", newFile[k]);
			}
		}
		isave = i;
		jsave = j;
		beginLine++;
	}


	free(lengths);
}

//Renvoie un tableau de dimension 2 contenant les tailles respectives de la prochaine ligne des tableaux pass�s en param�tres
//� partir de l'indice pass� (!!!!! g�rer le EOF !!!!!)
void length_line_from_idx(const char* tab1, const  char* tab2, int id1, int id2, int* lengths)
{
	while (tab1[id1] != '\n' && tab1[id1] != '\0'){
		lengths[0]++;
		id1++;
	}
	id1++;

	while (tab2[id2] != '\n' && tab2[id2] != '\0'){
		lengths[1]++;
		id2++;
	}
	id2++;
}

//fonction de comparaison de 2 lignes, utile seulement dans le cas o� les lignes sont de taille �gale
int compare_line(int* lengths, int idOld, int idNew, char* oldFile, char* newFile){
	int diff = 0;
	int imax = idOld + lengths[0], jmax = idNew + lengths[1];
	while ((diff == 0) && (idOld < imax) && (idNew < jmax)){
		if (oldFile[idOld] != newFile[idNew]){
			diff = 1;
		}
		idOld++;
		idNew++;
	}
	return diff;
}

//Fonction d'ouverture de fichier renvoi un pointeur de type FILE
//Cr�ation de cette fonction pour simplification, gestion des options et fichiers *lourds
FILE* OpenAFile(char* nomFichier, int optionOpen)
{
	FILE* file = NULL;
	if (optionOpen == 1)
	{
		//Forcer l'ouverture du fichier en mode tetxe m�me si le fichier est un fichier binaire
		//Peut conduire � des donn�es certes valides mais illisibles
		file = fopen(nomFichier, "r");
	}
	else
	{
		//Par d�faut ouverture en mode binaire pour la fonction fread()
		file = fopen(nomFichier, "rb");
	}
	//Verification de l'ouverture du fichier
	if (file == NULL)
	{
		printf("Can not open input file : %s\n", nomFichier);
		exit(EXIT_FAILURE);
	}
	return file;
}

/*Pointeur sur fichier, caract�re par caract�re
Renvoie un pointeur � la position du curseur
Fonction utilis�e pour les fichiers > 100 000 000 octets
A utiliser dans le diff principal si fichier > 100 000 000 octets
incompatible pur l'instant avec le soptions de transformation telle que :
--expand-tabs
*/
char* pointFile(FILE* file)
{
	//Prise du caract�re � la position du curseur et cast en char*
	char* actualChar = (char*)fgetc(file);
	return actualChar;
}
