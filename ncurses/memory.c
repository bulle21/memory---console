#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Structure pour les coordonnées
typedef struct {
    int y;
    int x;
} Position;

// Structure pour le jeu
typedef struct {
    int rows;
    int columns;
    char* gameString;
    int* discoveredCells;
    char name1[20];
    char name2[20];
    int points1;
    int points2;
    int turn;
    WINDOW* gameWin;
    WINDOW* statusWin;
} GameState;

// Prototypes
static void initGame(GameState* game);
static void createGameString(GameState* game);
static void drawBoard(GameState* game);
static void drawStatus(GameState* game);
static Position getCellPosition(int cell, int columns);
static int handleInput(GameState* game);
static void showCard(GameState* game, int cell);
static void hideCard(GameState* game, int cell);
static void cleanup(GameState* game);


// Dessiner le haut du tableau
static void printTop(WINDOW* win, int columns, int startY, int startX) {
    mvwaddch(win, startY, startX, ACS_ULCORNER);
    for(int i = 0; i < columns; i++) {
        for(int j = 0; j < 7; j++) {
            waddch(win, ACS_HLINE);
        }
        if(i < columns - 1) {
            waddch(win, ACS_TTEE);
        }
    }
    waddch(win, ACS_URCORNER);
}

// Dessiner le bas du tableau
static void printBottom(WINDOW* win, int columns, int startY, int startX) {
    mvwaddch(win, startY, startX, ACS_LLCORNER);
    for(int i = 0; i < columns; i++) {
        for(int j = 0; j < 7; j++) {
            waddch(win, ACS_HLINE);
        }
        if(i < columns - 1) {
            waddch(win, ACS_BTEE);
        }
    }
    waddch(win, ACS_LRCORNER);
}

// Dessiner une ligne de séparation
static void printSeparator(WINDOW* win, int columns, int startY, int startX) {
    mvwaddch(win, startY, startX, ACS_LTEE);
    for(int i = 0; i < columns; i++) {
        for(int j = 0; j < 7; j++) {
            waddch(win, ACS_HLINE);
        }
        if(i < columns - 1) {
            waddch(win, ACS_PLUS);
        }
    }
    waddch(win, ACS_RTEE);
}

// Initialisation du jeu
static void initGame(GameState* game) {
    // Initialiser ncurses
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    
use_default_colors(); // utilise les couleurs de fond par defaut
			      //
    // Définir les paires de couleurs
    init_pair(1, COLOR_BLUE, -1);    // Status normal
    init_pair(2, COLOR_GREEN, -1);   // Succès
    init_pair(3, COLOR_RED, -1);     // Erreur
    init_pair(4, COLOR_CYAN, -1);    // Carte sélectionnée
    
    // Obtenir les dimensions de l'écran
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    
    // Créer la fenêtre de statut
    game->statusWin = newwin(5, maxX-2, 1, 1);
    
    // Créer la fenêtre de jeu
    game->gameWin = newwin(maxY-7, maxX-2, 6, 1);
    
    // Initialiser les scores
    game->points1 = 0;
    game->points2 = 0;
    game->turn = 1;
    
    // Allouer la mémoire
    game->gameString = malloc(game->rows * game->columns);
    game->discoveredCells = calloc(game->rows * game->columns, sizeof(int));
    
    // Créer le tableau de jeu
    createGameString(game);
    
    // Rafraîchir les fenêtres
    refresh();
    wrefresh(game->statusWin);
    wrefresh(game->gameWin);
}

// Création du tableau de jeu
static void createGameString(GameState* game) {
    static const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#$%^&*()_+";
    int size = game->rows * game->columns;
    char* temp = malloc(size);
    
    // Créer les paires de symboles
    for(int i = 0; i < size; i += 2) {
        char c;
        do {
            c = alphanum[rand() % (sizeof(alphanum) - 1)];
        } while(strchr(temp, c) != NULL);
        temp[i] = temp[i+1] = c;
    }
    
    // Mélanger les symboles
    for(int i = 0; i < size; i++) {
        int j = rand() % size;
        char tmp = temp[i];
        temp[i] = temp[j];
        temp[j] = tmp;
    }
    
    memcpy(game->gameString, temp, size);
    free(temp);
}


// Afficher le statut
static void drawStatus(GameState* game) {
    wclear(game->statusWin);
    box(game->statusWin, 0, 0);
    
    wattron(game->statusWin, COLOR_PAIR(1));
    mvwprintw(game->statusWin, 1, 2, "%s: %d points    %s: %d points", 
              game->name1, game->points1, game->name2, game->points2);
    mvwprintw(game->statusWin, 2, 2, "Tour de: %s", 
              game->turn == 1 ? game->name1 : game->name2);
    wattroff(game->statusWin, COLOR_PAIR(1));
    
    wrefresh(game->statusWin);
}

// Obtenir la position d'une cellule
static Position getCellPosition(int cell, int columns) {
    Position pos;
    pos.y = 2 + (cell / columns) * 2;
    pos.x = 2 + (cell % columns) * 4;
    return pos;
}


// Nettoyer
static void cleanup(GameState* game) {
    free(game->gameString);
    free(game->discoveredCells);
    delwin(game->gameWin);
    delwin(game->statusWin);
    endwin();
}

static int getValidInput(GameState* game) {
    int input = 0;
    int ch;
    while (1) {
        ch = wgetch(game->gameWin);
        if(ch >= '0' && ch <= '9') {
            input = input * 10 + (ch - '0');
        } else if(ch == '\n') {
            if(input >= 0 && input < game->rows * game->columns && !game->discoveredCells[input]) {
                return input;
            }
            // Entrée invalide, réinitialiser
            input = 0;
        }
    }
}

static void drawBoard(GameState* game) {
    wclear(game->gameWin);
    
    int startY = 1;
    int startX = 2;
    int cellHeight = 1;
    
    printTop(game->gameWin, game->columns, startY, startX);
    startY++;
    
    for(int i = 0; i < game->rows; i++) {
        mvwaddch(game->gameWin, startY, startX, ACS_VLINE);
        for(int j = 0; j < game->columns; j++) {
            int cellIndex = i * game->columns + j;
            if(game->discoveredCells[cellIndex]) {
                wprintw(game->gameWin, "   %c   ", game->gameString[cellIndex]);
            } else {
                wprintw(game->gameWin, " %2d ", cellIndex);
            }
            mvwaddch(game->gameWin, startY, startX + (j + 1) * 8, ACS_VLINE);
        }
        startY++;
        
        if(i < game->rows - 1) {
            printSeparator(game->gameWin, game->columns, startY, startX);
            startY++;
        }
    }
    
    printBottom(game->gameWin, game->columns, startY, startX);
    
    wrefresh(game->gameWin);
}

static void showCard(GameState* game, int cell) {
    int row = cell / game->columns;
    int col = cell % game->columns;
    int startY = 2 + row * 2;
    int startX = 2 + col * 8;
    
    mvwaddch(game->gameWin, startY, startX, ACS_VLINE);
    if(game->discoveredCells[cell]) {
        wattron(game->gameWin, COLOR_PAIR(2));
    } else {
        wattron(game->gameWin, COLOR_PAIR(4));
    }
    wprintw(game->gameWin, "   %c   ", game->gameString[cell]);
    wattroff(game->gameWin, COLOR_PAIR(2));
    wattroff(game->gameWin, COLOR_PAIR(4));
    mvwaddch(game->gameWin, startY, startX + 8, ACS_VLINE);
    
    wrefresh(game->gameWin);
}

static void hideCard(GameState* game, int cell) {
    if(!game->discoveredCells[cell]) {
        int row = cell / game->columns;
        int col = cell % game->columns;
        int startY = 2 + row * 2;
        int startX = 2 + col * 8;
        
        mvwaddch(game->gameWin, startY, startX, ACS_VLINE);
        wprintw(game->gameWin, " %2d ", cell);
        mvwaddch(game->gameWin, startY, startX + 8, ACS_VLINE);
        
        wrefresh(game->gameWin);
    }
}

static int handleInput(GameState* game) {
    int input1 = getValidInput(game);
    showCard(game, input1);

    int input2;
    do {
        input2 = getValidInput(game);
    } while(input2 == input1);
    showCard(game, input2);

    // Le reste de la fonction reste inchangé
    if(game->gameString[input1] == game->gameString[input2]) {
        if(game->turn == 1) game->points1++; else game->points2++;
        game->discoveredCells[input1] = game->discoveredCells[input2] = 1;
        wattron(game->statusWin, COLOR_PAIR(2));
        mvwprintw(game->statusWin, 3, 2, "Bien joué !");
        wattroff(game->statusWin, COLOR_PAIR(2));
    } else {
        wattron(game->statusWin, COLOR_PAIR(3));
        mvwprintw(game->statusWin, 3, 2, "Raté !");
        wattroff(game->statusWin, COLOR_PAIR(3));
        game->turn = (game->turn == 1) ? 2 : 1;
        napms(1000);
        hideCard(game, input1);
        hideCard(game, input2);
    }
    
    wrefresh(game->statusWin);
    return (game->points1 + game->points2) * 2 == game->rows * game->columns;
}






int main() 
{
    GameState game = {0};
    
    // Configuration initiale
    printf("Entrez le nombre de lignes (max 10): ");
    scanf("%d", &game.rows);
    printf("Entrez le nombre de colonnes (max 10): ");
    scanf("%d", &game.columns);
    
    // Vérification des dimensions
    if(game.rows <= 0 || game.columns <= 0 || 
       game.rows > 10 || game.columns > 10 || 
       (game.rows * game.columns) % 2 != 0) {
        printf("Dimensions invalides!\n");
        return 1;
    }

    printf("Nom du joueur 1: ");
    scanf("%s", game.name1);
    printf("Nom du joueur 2: ");
    scanf("%s", game.name2);
    
    // Initialiser le générateur aléatoire
    srand(time(NULL));
    
    // Initialiser le jeu
    initGame(&game);
    
    // Boucle principale
    int gameOver = 0;
    while(!gameOver) {
        drawBoard(&game);
        drawStatus(&game);
        gameOver = handleInput(&game);
    }
    
    // Afficher le résultat final
    mvwprintw(game.statusWin, 3, 2, "Partie terminée! ");
    if(game.points1 > game.points2)
        wprintw(game.statusWin, "%s gagne!", game.name1);
    else if(game.points2 > game.points1)
        wprintw(game.statusWin, "%s gagne!", game.name2);
    else
        wprintw(game.statusWin, "Égalité!");
    wrefresh(game.statusWin);
    wgetch(game.gameWin);
    
    // Nettoyage
    cleanup(&game);
    
    return 0;
}

