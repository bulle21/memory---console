#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DEBUG 1



// GLOBAL VARIABLES
int 	col = 0, lig = 0,
	tour = 1,
	points1 = 0,
	points2 = 0,
	*CaseTrouve = NULL;

char 	nom1[20] = "NomJoueur1", nom2[20] = "NomJoueur2";

// Prototypages
static void createGameString(char *,int,int);
static void printGame(int,char *,char *,int,int);
static void printTable(char *,char *,char *,int,int);
static void printTop(int);
static void printBottom(int);
static void markAsDiscovered(int,int);
static void resetInputs(int,char *, char *);
static void printFinalScore(char *,char *,int,int);
static int checkTurn(char *,int,int,int);
static int inputPlayer(int,int,char *, char *);
static int valideSaisie(int,int,int,int);
static int saisieNumero(const char*) ;
static char rndCar(void);
static void printDebugTable(char *,int,int);




// Les fonctions
static void printDebugTable(char *Gametab,int lig,int col)
{
int i;

printf("\n[DEBUG] Game Symbols Table:\n");
for(i = 0;i < lig * col;i ++) {
    printf("%c ", Gametab[i]);
    if((i + 1) % col == 0) 
	    printf("\n");
    }
printf("\n");
}

static void printTable(char *Gametab,char *saisieJoueur1,char *saisieJoueur2,int lig,int col)
{
int i,j;
int iCase;

printTop(col);
for(i = 0;i < lig;i ++) {
    printf("│");
    for(j = 0;j < col;j ++) {
        iCase = i * col + j;
        if(CaseTrouve[iCase]) 
            printf("       │");
        else if((int) saisieJoueur1[iCase] || (int) saisieJoueur2[iCase]) 
            printf("   %c   │", Gametab[iCase]);
        else 
            if(iCase + 1 < 10) 
                printf("   %d   │", iCase + 1);
            else 
                if(iCase == 99)
                    printf("   %d │", iCase + 1);
                else
                    printf("   %d  │", iCase + 1);
        }

    printf("\n");
    if(i != lig - 1) {
        printf("├");
        for(j = 0; j < col; j++) 
            if(j == col - 1) 
                printf("───────┤\n");
            else
                printf("───────┼");
        }
    }
printBottom(col);

#ifdef DEBUG
// Call debug table print function
printDebugTable(Gametab, lig, col);
#endif
}

static char rndCar()
{
static const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#$%^&*()_+{}|:<>?-=[];',./";
return(alphanum[rand() % (sizeof(alphanum) - 1)]);
}


static int saisieNumero(const char* prompt) 
{
char 	*newline,input[100];
int 	i,valid = 0,number,c;
    
do {
    printf("%s", prompt);
    if(fgets(input, sizeof(input), stdin)) {
        // Remove newline if present
        newline = strchr(input, '\n');
        if(newline) 
		*newline = 0;
        
        // Check if input is numeric
        valid = 1;
        for(i = 0;input[i] != '\0'; i++) 
            if(input[i] < '0' || input[i] > '9') {
                valid = 0;
                break;
                }
            
        if(valid && sscanf(input, "%d", &number) == 1) 
            return(number);
        }
        
    printf("\x1b[31mEntrée invalide. Veuillez entrer un nombre.\x1b[0m\n");
    
    // Clear any remaining input
    while((c = getchar()) != '\n' && c != EOF);

    } while (!valid);
    
return(0);
}



// saisie d'un numero de case
static int valideSaisie(int input,int lig,int col,int prevSaisie)
{ 
int 	i,inputOk = 0,
    	NBcaseAchercher;
    
// Count available cells
for(NBcaseAchercher = 0,i = 0;i < lig * col;i ++) 
    if(!CaseTrouve[i]) 
        NBcaseAchercher ++;
    
do	{
	if(scanf("%d",&input) != 1) {
        	// Clear input buffer
        	while(getchar() != '\n');
        	printf("\x1b[31mEntrée invalide. Veuillez entrer un nombre.\x1b[0m\n");
    		} 
    	else if (input > lig * col || input < 1 || CaseTrouve[input - 1]) {
        	if(NBcaseAchercher > 0) 
            		printf("\x1b[31mVeuillez entrer un nombre valide entre 1 et %d (case non découverte)\x1b[0m\n", 
					lig * col);
        	else 
            		// end the game
            		return(-1);
    		}
	
    	// Add check for same input as previous selection
    	else if(prevSaisie != 0 && input == prevSaisie) 
        	printf("\x1b[31mVous ne pouvez pas sélectionner la même case deux fois!\x1b[0m\n");
    	else 
        	inputOk = 1;
        
    	if(!inputOk) 
        	printf("Entrez un numéro de case valide : ");

	} while(!inputOk);

return(input);
}

static void createGameString(char *Gametab,int lig,int col) 
{
int 	i,rndPosition;
char 	rndCarCreated,lastChar = '\0',
	backUp[lig * col];

for(i = 0;i < lig * col;i += 2) {
    rndCarCreated = rndCar();
    while(rndCarCreated == lastChar) 
        rndCarCreated = rndCar();

    lastChar = rndCarCreated;
    backUp[i] = backUp[i + 1] = lastChar;
    }

for(i = 0;i <= lig * col;i ++) 
    Gametab[i] = '\0';

for(i = 0;i < lig * col;i ++) {
    rndPosition = rand() % (lig * col);
    while(Gametab[rndPosition] != '\0') 
        rndPosition = rand() % (lig * col);
    Gametab[rndPosition] = backUp[i];
    }
}

static int checkTurn(char * Gametab, int input1, int input2, int tour)
{
if(Gametab[input1 - 1] == Gametab[input2 - 1]) {
    if (tour == 1) {
        points1 = points1 + 1;
        } 
    else {
            points2 = points2 + 1;
        }
    markAsDiscovered(input1, input2);
    return 1;
    }
return(0);
}

static void markAsDiscovered(int input1, int input2) 
{
CaseTrouve[input1 - 1] = 1;
CaseTrouve[input2 - 1] = 1;
}


static void printTop(int col)
{
for(int i = 0; i < col; i++) {
    if(i == 0) 
        printf("┌");
    if(i == col - 1) 
        printf("───────┐\n");
    else 
        printf("───────┬");
    }
}

static void printBottom(int col) 
{
int i;

for(i = 0; i < col; i++) {
    if(i == 0) 
        printf("└");
    	
    if(i == col - 1) 
        printf("───────┘\n");
    else 
        printf("───────┴");
    }
}

static int inputPlayer(int tour,int input,char *saisieJoueur1,char *saisieJoueur2)
{
if(tour == 1)
    	saisieJoueur1[input - 1] = 1;
else 
	saisieJoueur2[input - 1] = 1;
return(0);
}

static void resetInputs(int tour,char *saisieJoueur1,char *saisieJoueur2)
{
if(tour == 1) 
    	memset(saisieJoueur1, 0,lig * col);
else 
    	memset(saisieJoueur2, 0,lig * col);
}

static void printGame(int tour,char *nom1,char *nom2,int points1,int points2)
{
printf("Joueur 1 : %s\tJoueur 2 : %s", nom1, nom2);
tour == 1 ? printf("\t\tTour de : %s\n", nom1) : printf("\t\t\x1b[36mTour de : %s\x1b[0m\n", nom2);
printf("Points : %d\t\tPoints : %d\n\n", points1, points2);
}

static void printFinalScore(char *nom1,char *nom2,int points1,int points2) 
{
printf("\n\x1b[33m=== Résultat Final ===\x1b[0m\n\n");
printf("%s : %d points\n", nom1, points1);
printf("%s : %d points\n\n", nom2, points2);
    
if(points1 > points2) 
    	printf("\x1b[32mFélicitations %s, vous avez gagné !\x1b[0m\n", nom1);
else if(points2 > points1) 
    	printf("\x1b[32mFélicitations %s, vous avez gagné !\x1b[0m\n", nom2);
else 
    	printf("\x1b[33mFélicitations, c'est une égalité !\x1b[0m\n");
}

int main()
{
char *Gametab,*saisieJoueur1,*saisieJoueur2;
int input1,input2;

srand(time(NULL));
printf("\x1b[32mBienvenue\x1b[0m\n\nVeuillez entrer le nombre de lignes et de colonnes que vous souhaitez\n\n");

do {
	lig = saisieNumero("Entrez le nombre de lignes : ");
	col = saisieNumero("Entrez le nombre de colonnes : ");

	if(lig * col > 100) 
    		printf("\x1b[31mLe nombre de lignes multiplié par le nombre de colonnes ne peut pas dépasser 100\x1b[0m\n");
	else if(lig * col < 4) 
    		printf("\x1b[31mLe nombre de lignes multiplié par le nombre de colonnes ne peut pas être inférieur à 4\x1b[0m\n");
	if (lig * col % 2 != 0) 
    		printf("\x1b[31mLe nombre de lignes multiplié par le nombre de colonnes doit être pair\x1b[0m\n");

} while (lig * col > 100 || lig * col < 4 || lig * col % 2 != 0);

system("clear");
printf("\x1b[32mExcellent ! Maintenant, entrez les noms des joueurs\x1b[0m\n\n");
system("sleep 1.8 && clear");
printf("Veuillez entrer votre nom, joueur 1 : "); scanf("%s", nom1);
printf("Veuillez entrer votre nom, joueur 2 : "); scanf("%s", nom2);
system("clear");
    
//Gametab[lig * col];
Gametab = (char *) malloc (lig * col * sizeof(char));
createGameString(Gametab, lig, col);

//saisieJoueur1[lig*col];
saisieJoueur1 = (char *) malloc(lig * col * sizeof(char));

//saisieJoueur2[lig*col];
saisieJoueur2 = (char *) malloc(lig * col * sizeof(char));

CaseTrouve = calloc(lig * col, sizeof(int));
//memset(saisieJoueur1, 0, sizeof(saisieJoueur1));
//memset(saisieJoueur2, 0, sizeof(saisieJoueur2));
    
while(points1 + points2 < lig * col / 2) {
    printGame(tour, nom1, nom2, points1, points2);
    printTable(Gametab, saisieJoueur1, saisieJoueur2, lig, col);
    input1 = input2 = 0;
    printf("\n%s,\n\n", tour == 1 ? nom1 : nom2);
    printf("Choisissez la première case : ");
    input1 = valideSaisie(input1, lig, col, 0); // 0 car pas de sélection précédente
    if(input1 == -1) 
	    break;
    inputPlayer(tour,input1,saisieJoueur1,saisieJoueur2);
    system("clear");
    printGame(tour,nom1,nom2,points1,points2);
    printTable(Gametab,saisieJoueur1,saisieJoueur2,lig,col);
    printf("\n%s,\n\n", tour == 1 ? nom1 : nom2);

    printf("Choisissez la deuxième case : ");
    input2 = valideSaisie(input2, lig, col, input1); // On passe input1 comme sélection précédente
    if (input2 == -1) 
	    break;
    inputPlayer(tour,input2,saisieJoueur1,saisieJoueur2);
    system("clear");
    printGame(tour,nom1,nom2,points1,points2);
    printTable(Gametab,saisieJoueur1,saisieJoueur2,lig,col);
    if(checkTurn(Gametab, input1, input2, tour) == 1) {
        printf("\x1b[32mBien joué, vous avez trouvé une paire !\x1b[0m\n");
        system("sleep 1.2");
        } 
    else {
        printf("\x1b[31mRaté, mais ne vous découragez pas !\x1b[0m\n");
        system("sleep 1.2");
        tour = (tour == 1) ? 2 : 1;
        }
    resetInputs(1,saisieJoueur1,saisieJoueur2);
    resetInputs(2,saisieJoueur2,saisieJoueur2);
    system("clear");
    }
    
printTable(Gametab,saisieJoueur1,saisieJoueur2,lig,col);
printFinalScore(nom1,nom2,points1,points2);

free(CaseTrouve);
free(Gametab);
free(saisieJoueur1);
free(saisieJoueur2);
return(0);
}
