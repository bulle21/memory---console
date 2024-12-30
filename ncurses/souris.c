#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Structure pour les coordonnees
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
static int handleMouseClick(GameState* game);
static void showCard(GameState* game, int cell);
static void hideCard(GameState* game, int cell);
static void cleanup(GameState* game);

// Dessiner les bordures du tableau
static void printTop(WINDOW* win, int columns, int startY, int startX) {
    mvwaddch(win, startY, startX, ACS_ULCORNER);
    for (int i = 0; i < columns; i++) {
        for (int j = 0; j < 7; j++) {
            waddch(win, ACS_HLINE);
        }
        if (i < columns - 1) {
            waddch(win, ACS_TTEE);
        }
    }
    waddch(win, ACS_URCORNER);
}

static void printSeparator(WINDOW* win, int columns, int startY, int startX) {
    mvwaddch(win, startY, startX, ACS_LTEE);
    for (int i = 0; i < columns; i++) {
        for (int j = 0; j < 7; j++) {
            waddch(win, ACS_HLINE);
        }
        if (i < columns - 1) {
            waddch(win, ACS_PLUS);
        }
    }
    waddch(win, ACS_RTEE);
}

static void printBottom(WINDOW* win, int columns, int startY, int startX) {
    mvwaddch(win, startY, startX, ACS_LLCORNER);
    for (int i = 0; i < columns; i++) {
        for (int j = 0; j < 7; j++) {
            waddch(win, ACS_HLINE);
        }
        if (i < columns - 1) {
            waddch(win, ACS_BTEE);
        }
    }
    waddch(win, ACS_LRCORNER);
}

static void initGame(GameState* game) {
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS, NULL);  // Activer les evenements de souris
    curs_set(0);
    use_default_colors();

    init_pair(1, COLOR_BLUE, -1);
    init_pair(2, COLOR_GREEN, -1);
    init_pair(3, COLOR_RED, -1);
    init_pair(4, COLOR_CYAN, -1);

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    game->statusWin = newwin(5, maxX - 2, 1, 1);
    game->gameWin = newwin(maxY - 7, maxX - 2, 6, 1);

    game->points1 = 0;
    game->points2 = 0;
    game->turn = 1;

    game->gameString = malloc(game->rows * game->columns);
    game->discoveredCells = calloc(game->rows * game->columns, sizeof(int));

    createGameString(game);

    refresh();
    wrefresh(game->statusWin);
    wrefresh(game->gameWin);
}

static void createGameString(GameState* game) {
    static const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#$%^&*()_+";
    int size = game->rows * game->columns;
    char* temp = malloc(size);

    for (int i = 0; i < size; i += 2) {
        char c;
        do {
            c = alphanum[rand() % (sizeof(alphanum) - 1)];
        } while (strchr(temp, c) != NULL);
        temp[i] = temp[i + 1] = c;
    }

    for (int i = 0; i < size; i++) {
        int j = rand() % size;
        char tmp = temp[i];
        temp[i] = temp[j];
        temp[j] = tmp;
    }

    memcpy(game->gameString, temp, size);
    free(temp);
}

static void drawBoard(GameState* game) {
    wclear(game->gameWin);

    int startY = 1;
    int startX = 2;

    printTop(game->gameWin, game->columns, startY, startX);
    startY++;

    for (int i = 0; i < game->rows; i++) {
        mvwaddch(game->gameWin, startY, startX, ACS_VLINE);
        for (int j = 0; j < game->columns; j++) {
            int cellIndex = i * game->columns + j;
            if (game->discoveredCells[cellIndex]) {
                wprintw(game->gameWin, "   %c   ", game->gameString[cellIndex]);
            } else {
                wprintw(game->gameWin, " %2d ", cellIndex);
            }
            mvwaddch(game->gameWin, startY, startX + (j + 1) * 8, ACS_VLINE);
        }
        startY++;

        if (i < game->rows - 1) {
            printSeparator(game->gameWin, game->columns, startY, startX);
            startY++;
        }
    }

    printBottom(game->gameWin, game->columns, startY, startX);
    wrefresh(game->gameWin);
}

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

static int handleMouseClick(GameState* game) {
    MEVENT event;
    int result = getmouse(&event);
    if (result == OK && event.bstate & BUTTON1_CLICKED) {
        int colWidth = 8;
        int rowHeight = 2;
        int cellX = (event.x - 2) / colWidth;
        int cellY = (event.y - 2) / rowHeight;

        if (cellX >= 0 && cellX < game->columns && cellY >= 0 && cellY < game->rows) {
            int cellIndex = cellY * game->columns + cellX;
            showCard(game, cellIndex);
            return cellIndex;
        }
    }
    return -1;
}

static void showCard(GameState* game, int cell) {
    if (!game->discoveredCells[cell]) {
        int row = cell / game->columns;
        int col = cell % game->columns;
        int startY = 2 + row * 2;
        int startX = 2 + col * 8;

        wattron(game->gameWin, COLOR_PAIR(4));
        mvwprintw(game->gameWin, startY, startX + 1, " %c ", game->gameString[cell]);
        wattroff(game->gameWin, COLOR_PAIR(4));
        wrefresh(game->gameWin);
    }
}

static void cleanup(GameState* game) {
    free(game->gameString);
    free(game->discoveredCells);
    delwin(game->gameWin);
    delwin(game->statusWin);
    endwin();
}

int main() {
    GameState game = {0};

    printf("Entrez le nombre de lignes (max 10): ");
    scanf("%d", &game.rows);
    printf("Entrez le nombre de colonnes (max 10): ");
    scanf("%d", &game.columns);

    if (game.rows <= 0 || game.columns <= 0 || game.rows > 10 || game.columns > 10 || (game.rows * game.columns) % 2 != 0) {
        printf("Dimensions invalides!\n");
        return 1;
    }

    printf("Nom du joueur 1: ");
    scanf("%s", game.name1);
    printf("Nom du joueur 2: ");
    scanf("%s", game.name2);

    srand(time(NULL));
    initGame(&game);

    // Dessiner initialement le tableau et l'état
    drawBoard(&game);
    drawStatus(&game);

    while (1) {
        int ch = wgetch(game.gameWin); // Attendre une entrée utilisateur

        if (ch == KEY_MOUSE) {
            // Gérer les clics de souris
            int clickedCell = handleMouseClick(&game);
            if (clickedCell != -1) {
                // Mettre à jour uniquement après un clic valide
                drawBoard(&game);
                drawStatus(&game);
            }
        } else if (ch == 'q' || ch == 'Q') {
            // Quitter si l'utilisateur appuie sur 'q'
            break;
        }
        // Ajoutez ici d'autres conditions pour gérer les entrées clavier si nécessaire
    }

    cleanup(&game);
    return 0;
}

