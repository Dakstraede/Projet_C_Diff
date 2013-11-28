//Prototypes des fonctions

int strcomp1(const char* s1, const char* s2);

int myStrlen(char *s);
int linesOfFile(char* file);
int CaractecrsOfFile(char* file);
char* fileToTabs(char* file, int lines);


void help_option();


int compteLigneFichier(char *nomFichier);
void ajouterATableauLigneFichier(char *nomFichier, char *tabLigne, int numLigne, int tailleTableau);
int retourLigneCurseur(char *nomFichier, int numLigne);
int nombreCaractereLigne(char *nomFichier, int numLigne);
void afficheLigne(char *nomFichier, int numLigne);
void compare(char *nomDuFichier1,char *nomDuFichier2);

int ignoreCase(char c1, char c2);

void briefing(int differ, char firstFile, char secondFile);

void identicalFiles(int same, char firstFile, char secondFile);

unsigned long GetFileSize(char *file);