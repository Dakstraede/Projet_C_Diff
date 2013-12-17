#include "Header.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Options fonctions of the diff command
void help_option()
{
	printf("Usage: diff [OPTION]... FICHIERS\n");
	printf("Comparer les fichiers ligne par ligne.\n");
	printf("Les param%ctres requis pour les options longues sont %cgalement requis pour les options courtes.\n", 138, 130);
	printf("--normal                  Produire un %c diff %c en format normal (par d%cfaut)\n", 174, 175, 130);
	printf("-q, --brief                   Indiquer seulement si les fichiers diff%crent\n", 130);
	printf("-s, --report-identical-files  Indiquer si les deux fichiers sont identiques\n");
	printf("-t, --expand-tabs             %ctaler les tabulateurs en espaces dans la sortie\n", 144);
	printf("-T, --initial-tab            Aligner les tabulateurs en pr�fixant un tabulateur\n");
	printf("-l, --paginate                Pagine la sortie du diff, une page est par d�faut � 66 lignes dont 56 lignes de textes\n");
	printf("-N, --new-file                Traiter les fichiers absents comme des fichiers vides");
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
		printf("Impossible de lire le fichier\n");
		return 0;
	}

}

int strcomp1(const char* s1, const char* s2)
{
	while ((*s1++ == *s2++) && (*s1 != '\0'));
	return (*((unsigned char *)--s1) < *((unsigned char *)--s2)) ? -1 : (*(unsigned char *)s1 != *(unsigned char *)s2);
}

unsigned long linesOfTab(char *tab, unsigned long* length)
{
	unsigned long newLineCount = 1;

	if (tab != NULL)
	{/*count the newline characters*/

		unsigned long j = 0;
		unsigned long* taille = &j;

		while (tab[j] != '\0')
		{
			if (tab[j] == '\n')
			{
				newLineCount++;
			}
			j++;
		}
		*length = j;
	}
	else
	{
		return 0;
	}
	return newLineCount;
}

//Return the number of characters in a file
unsigned long CaractersOfFile(char* file)
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
		printf("Impossible de lire le fichier\n");
		return 0;
	}
}
//Return a tab fulled by all the characters of file
//char per char
char* fileToTabs(char* file, int optionSpe, int optionN)
{
	char* buffer;
	FILE* fileToTab = NULL;
	unsigned long fileSize = 0;

	fileToTab = OpenAFile(file, optionSpe);
	if (fileToTab == NULL && optionN == 0)
	{
		printf("Can not open input file : %s\n", file);
		exit(EXIT_FAILURE);

	}
	else if (fileToTab == NULL && optionN == 1)
	{
		buffer = calloc(1, fileSize + 1);
		buffer[0] = '\0';
	}
	else{
		fileSize = GetFileSize(file, optionSpe);
		//calloc = allocation d'un tableau de 1 �l�ment de taille fileSize+1
		buffer = calloc(1, fileSize + 1);
		if (buffer == NULL)
		{
			exit(EXIT_FAILURE);
		}

		//Copy of file to the buffer tab
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
	}
	return buffer;
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
/*Function ignoreCase
Return 1 if c1!=c2 even ignoring case
Return 0 if c1==c2 ignoring case
+32 et -32 = Table ACSII maj to min or min to maj*/
int ignoreCase(char c1, char c2)
{
	if (c1 == c2)
	{
		return 0;
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
void briefing(int differ, const char *firstFileName, const char *secondFileName)
{
	if (differ == 1)
	{
		printf("Files %s and %s differ\n", firstFileName, secondFileName);
		exit(EXIT_SUCCESS);

	}
	else{//Sinon fermeture direct
		exit(EXIT_SUCCESS);
	}
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
		printf("\nContent of files %s and %s are the same\n", firstFile, secondFile);
	}
	//Rien si different
}

//Return the file size in bytes (octet)
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
		printf("Impossible de lire le fichier\n");
		return 0;
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
		fputs("entire read fails\n", stderr);
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
void diff(char* oldFile, char* newFile, int lengthOldFile, int lengthNewFile, int nbCommonLines, int displayOption,
	const char* firstFileName, const char* secondFileName, const char *chaineArgs, int ldisplayOption, int optionCase,
	int nbLinesOld, int nbLinesNew)
{
	//D�claration des indices de parcours des tableaux
	//et une variable pour g�rer la diff�rence
	int i = 0, j = 0, k, temp1 = 0, temp2 = 0;;
	int isave = 0, jsave = 0;
	int beginLine = 1, lastCompLine = 1;
	int diff = 0;
	unsigned long nbDisplayLines = 0;
	time_t now = time(NULL);
	struct tm * tm = localtime(&now);
	int nbPages = 2;

	//D�claration d'un tableau contenant les tailles des lignes de chaque tableau:
	// lengths[0] = old ; lengths[1] = new
	int* lengths = malloc(2 * sizeof(int));
	lengths[0] = 0; lengths[1] = 0;

	/*
	paginate option prints a header of five lines : 2 empty lines the header(time, arguments, page) and 2 empty lines
	it prints 56 lines of text
	and a footer of five empty lines
	*/
	if (ldisplayOption == 1)
	{
		printf("\n\n%u-%u-%u %u:%u %s Page 1\n\n\n", (1900 + tm->tm_year), (1 + tm->tm_mon), tm->tm_mday, tm->tm_hour, tm->tm_min, chaineArgs);
	}
	while ((i < lengthOldFile) && (j < lengthNewFile))
	{
		lastCompLine = beginLine;
		length_line_from_idx(oldFile, newFile, i, j, lengths);

		//test simple sur la longueur des lignes, si la taille est diff�rente, les lignes sont diff�rentes
		//si elles sont de taille �gale, on appelle la fonction de comparaison des lignes
		if (lengths[0] != lengths[1]){
			diff = 1;
		}
		else{
			diff = compare_line(lengths, i, j, oldFile, newFile, optionCase);
		}


		if (diff == 1){
			if (displayOption == 2)
			{
				briefing(1, firstFileName, secondFileName);
			}
			//passage au 1er indice de la ligne suivante pour les deux fichiers
			//et recalcul de la taille de la ligne suivante
			//les variables save correspondent � l'indice de d�part pour l'affichage
			isave = i;
			jsave = j;
			i += lengths[0] + 1;
			j += lengths[1] + 1;
			length_line_from_idx(oldFile, newFile, i, j, lengths);

			while (compare_line(lengths, i, j, oldFile, newFile, optionCase) == 1 && lastCompLine < nbCommonLines && i < lengthOldFile && j < lengthNewFile){
				lastCompLine++;
				i += lengths[0] + 1;
				j += lengths[1] + 1;
				length_line_from_idx(oldFile, newFile, i, j, lengths);
			}

			//AFFICHAGE : cas o� il y a plus d'une ligne diff�rente
			if (beginLine != lastCompLine){
				//affichage de la ligne avec le code de changement
				printf("%i,%ic%i,%i \n", beginLine, lastCompLine, beginLine, lastCompLine);
				if (ldisplayOption == 1)
				{
					nbDisplayLines++;
					if (nbDisplayLines % 56 == 0)
					{
						printf("\n\n\n\n\n\n\n%u-%u-%u %u:%u %s Page %d\n\n\n", (1900 + tm->tm_year), (1 + tm->tm_mon), tm->tm_mday, tm->tm_hour, tm->tm_min, chaineArgs, nbPages);
						nbPages++;
						nbDisplayLines = 0;
					}
				}
				//gestion du cas o� la premi�re ligne est diff�rente (on affiche la premi�re ligne avant le traitement)
				printf("< ");
				if (displayOption == 1)
				{
					printf("\t");
				}

				while (oldFile[isave] != '\n'){
					printf("%c", oldFile[isave]);
					isave++;
				}
				printf("\n");
				//affichage des lignes du 1er fichier avec <"
				for (k = isave; k < i; k++){
					if (oldFile[k - 1] == '\n'){
						if (ldisplayOption == 1)
						{
							nbDisplayLines++;
							if (nbDisplayLines % 56 == 0)
							{
								printf("\n\n\n\n\n\n\n%u-%u-%u %u:%u %s Page %d\n\n\n", (1900 + tm->tm_year), (1 + tm->tm_mon), tm->tm_mday, tm->tm_hour, tm->tm_min, chaineArgs, nbPages);
								nbPages++;
								nbDisplayLines = 0;
							}
						}
						printf("< ");
						if (displayOption == 1)
						{
							printf("\t");
						}

					}

					printf("%c", oldFile[k]);
				}

				//s�paration
				if (ldisplayOption == 1)
				{
					nbDisplayLines++;
					if (nbDisplayLines % 56 == 0)
					{
						printf("\n\n\n\n\n\n\n%u-%u-%u %u:%u %s Page %d\n\n\n", (1900 + tm->tm_year), (1 + tm->tm_mon), tm->tm_mday, tm->tm_hour, tm->tm_min, chaineArgs, nbPages);
						nbPages++;
						nbDisplayLines = 0;
					}
				}
				printf("\n - - - \n");

				//affichage 1�re ligne 2�me fichier
				if (ldisplayOption == 1)
				{
					nbDisplayLines++;
					if (nbDisplayLines % 56 == 0)
					{
						printf("\n\n\n\n\n\n\n%u-%u-%u %u:%u %s Page %d\n\n\n", (1900 + tm->tm_year), (1 + tm->tm_mon), tm->tm_mday, tm->tm_hour, tm->tm_min, chaineArgs, nbPages);
						nbPages++;
						nbDisplayLines = 0;
					}
				}
				printf("> ");
				if (ldisplayOption == 1)
				{
					printf("\t");
				}

				while (newFile[jsave] != '\n'){
					printf("%c", newFile[jsave]);
					jsave++;
				}

				//affichage des lignes du nouveau fichier avec ">"
				for (k = jsave; k < j; k++){
					if (newFile[k - 1] == '\n'){
						if (ldisplayOption == 1)
						{
							nbDisplayLines++;

							if (nbDisplayLines % 56 == 0)
							{
								printf("\n\n\n\n\n\n\n%u-%u-%u %u:%u %s Page %d\n\n\n", (1900 + tm->tm_year), (1 + tm->tm_mon), tm->tm_mday, tm->tm_hour, tm->tm_min, chaineArgs, nbPages);
								nbPages++;
								nbDisplayLines = 0;
							}
						}
						printf("> ");
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
				if (ldisplayOption == 1)
				{
					nbDisplayLines++;
					if (nbDisplayLines % 56 == 0)
					{
						printf("\n\n\n\n\n\n\n%u-%u-%u %u:%u %s Page %d\n\n\n", (1900 + tm->tm_year), (1 + tm->tm_mon), tm->tm_mday, tm->tm_hour, tm->tm_min, chaineArgs, nbPages);
						nbPages++;
						nbDisplayLines = 0;
					}
				}
				printf("< ");
				if (displayOption == 1)
				{
					printf("\t");
				}

				for (k = isave; k < i; k++)
					printf("%c", oldFile[k]);

				if (ldisplayOption == 1)
				{
					nbDisplayLines++;
					if (nbDisplayLines % 56 == 0)
					{
						printf("\n\n\n\n\n\n\n%u-%u-%u %u:%u %s Page %d\n\n\n", (1900 + tm->tm_year), (1 + tm->tm_mon), tm->tm_mday, tm->tm_hour, tm->tm_min, chaineArgs, nbPages);
						nbPages++;
						nbDisplayLines = 0;
					}
				}
				printf("\n - - - \n");
				if (ldisplayOption == 1)
				{
					nbDisplayLines++;
					if (nbDisplayLines % 56 == 0)
					{
						printf("\n\n\n\n\n\n\n%u-%u-%u %u:%u %s Page %d\n\n\n", (1900 + tm->tm_year), (1 + tm->tm_mon), tm->tm_mday, tm->tm_hour, tm->tm_min, chaineArgs, nbPages);
						nbPages++;
						nbDisplayLines = 0;
					}
				}

				printf("> ");
				if (displayOption == 1)
				{
					printf("\t");
				}

				for (k = jsave; k < j; k++)
					printf("%c", newFile[k]);
			}
		}
		temp1 = i;
		temp2 = j;
		i += lengths[0] + 1;
		j += lengths[1] + 1;
		beginLine++;
	}
	//GESTION ADD ET DELETE

	//cas o� le nouveau fichier contient plus de lignes -> add
	if (nbLinesNew > nbLinesOld){
		if (nbLinesNew == nbLinesOld + 1){
			if (ldisplayOption == 1)
			{
				nbDisplayLines++;
				if (nbDisplayLines % 56 == 0)
				{
					printf("\n\n\n\n\n\n\n%u-%u-%u %u:%u %s Page %d\n\n\n", (1900 + tm->tm_year), (1 + tm->tm_mon), tm->tm_mday, tm->tm_hour, tm->tm_min, chaineArgs, nbPages);
					nbPages++;
					nbDisplayLines = 0;
				}
			}
			printf("%da%d\n", (nbCommonLines + 1), (nbCommonLines + 1));
			printf("> ");
			if (displayOption == 1)
			{
				printf("\t");
			}
			while (newFile[temp2] != '\0'){
				printf("%c", newFile[temp2]);
				temp2++;
			}
		}
		else{

			if (ldisplayOption == 1)
			{
				nbDisplayLines++;
				if (nbDisplayLines % 56 == 0)
				{
					printf("\n\n\n\n\n\n\n%u-%u-%u %u:%u %s Page %d\n\n\n", (1900 + tm->tm_year), (1 + tm->tm_mon), tm->tm_mday, tm->tm_hour, tm->tm_min, chaineArgs, nbPages);
					nbPages++;
					nbDisplayLines = 0;
				}
			}
			printf("%da%d,%d\n", (nbCommonLines + 1), (nbCommonLines + 1), nbLinesNew);
			printf("> ");
			if (displayOption == 1)
			{
				printf("\t");
			}
			while (newFile[temp2] != '\n'){
				printf("%c", newFile[temp2]);
				temp2++;
			}
			printf("%c", newFile[temp2]);
			temp2++;

			for (k = temp2; newFile[k] != '\0'; k++){
				if (newFile[k - 1] == '\n'){
					if (ldisplayOption == 1)
					{
						nbDisplayLines++;
						if (nbDisplayLines % 56 == 0)
						{
							printf("\n\n\n\n\n\n\n%u-%u-%u %u:%u %s Page %d\n\n\n", (1900 + tm->tm_year), (1 + tm->tm_mon), tm->tm_mday, tm->tm_hour, tm->tm_min, chaineArgs, nbPages);
							nbPages++;
							nbDisplayLines = 0;
						}
					}
					printf("> ");
					if (displayOption == 1)
					{
						printf("\t");
					}
				}
				printf("%c", newFile[k]);
			}
		}
	}

	//cas o� le nouveau fichier contient moins de lignes -> delete
	else if (nbLinesOld > nbLinesNew){

		if (nbLinesNew == nbLinesOld - 1){
			if (ldisplayOption == 1)
			{
				nbDisplayLines++;
				if (nbDisplayLines % 56 == 0)
				{
					printf("\n\n\n\n\n\n\n%u-%u-%u %u:%u %s Page %d\n\n\n", (1900 + tm->tm_year), (1 + tm->tm_mon), tm->tm_mday, tm->tm_hour, tm->tm_min, chaineArgs, nbPages);
					nbPages++;
					nbDisplayLines = 0;
				}
			}
			printf("%dd%d\n", (nbCommonLines + 1), (nbCommonLines));
			printf("< ");
			if (displayOption == 1)
			{
				printf("\t");
			}
			while (oldFile[temp1] != '\0'){
				printf("%c", oldFile[temp1]);
				temp1++;
			}
		}
		else{

			if (ldisplayOption == 1)
			{
				nbDisplayLines++;
				if (nbDisplayLines % 56 == 0)
				{
					printf("\n\n\n\n\n\n\n%u-%u-%u %u:%u %s Page %d\n\n\n", (1900 + tm->tm_year), (1 + tm->tm_mon), tm->tm_mday, tm->tm_hour, tm->tm_min, chaineArgs, nbPages);
					nbPages++;
					nbDisplayLines = 0;
				}
			}
			printf("%d,%dd%d\n", (nbCommonLines + 1), (nbLinesOld), nbCommonLines);
			printf("< ");
			if (displayOption == 1)
			{
				printf("\t");
			}
			while (oldFile[temp1] != '\n'){
				printf("%c", oldFile[temp1]);
				temp1++;
			}
			printf("%c", oldFile[temp1]);
			temp1++;

			for (k = temp1; oldFile[k] != '\0'; k++){
				if (oldFile[k - 1] == '\n'){
					if (ldisplayOption == 1)
					{
						nbDisplayLines++;
						if (nbDisplayLines % 56 == 0)
						{
							printf("\n\n\n\n\n\n\n%u-%u-%u %u:%u %s Page %d\n\n\n", (1900 + tm->tm_year), (1 + tm->tm_mon), tm->tm_mday, tm->tm_hour, tm->tm_min, chaineArgs, nbPages);
							nbPages++;
							nbDisplayLines = 0;
						}
					}
					printf("< ");
					if (displayOption == 1)
					{
						printf("\t");
					}
				}
				printf("%c", oldFile[k]);
			}
		}
	}

	if (ldisplayOption == 1)
	{
		while (nbDisplayLines % 60 != 0)
		{
			printf("\n");
			nbDisplayLines++;
		}
	}

	free(lengths);
}

//Renvoie un tableau de dimension 2 contenant les tailles respectives de la prochaine ligne des tableaux pass�s en param�tres
//� partir de l'indice pass� (!!!!! g�rer le EOF !!!!!)
void length_line_from_idx(const char* tab1, const  char* tab2, int id1, int id2, int* lengths)
{
	lengths[0] = 0;
	while (tab1[id1] != '\n' && tab1[id1] != '\0'){
		lengths[0]++;
		id1++;
	}

	lengths[1] = 0;
	while (tab2[id2] != '\n' && tab2[id2] != '\0'){
		lengths[1]++;
		id2++;
	}
}
//fonction de comparaison de 2 lignes, utile seulement dans le cas o� les lignes sont de taille �gale
int compare_line(int* lengths, int idOld, int idNew, char* oldFile, char* newFile, int optionCase){
	int diff = 0;
	int imax = idOld + lengths[0], jmax = idNew + lengths[1], diffCase = 0;
	while ((diff == 0) && (idOld < imax) && (idNew < jmax)){
		if (oldFile[idOld] != newFile[idNew]){
			if (optionCase == 1)
			{
				if ((diffCase = ignoreCase(oldFile[idOld], newFile[idNew]) == 1))
				{
					diff = 1;
				}
			}
			else
			{
				diff = 1;
			}
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
	//Verification de l'ouverture du fichier dans les autres fonctions cause option N

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
