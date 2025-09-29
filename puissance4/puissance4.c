#include <stdio.h>
#include <stdlib.h>

#define ROWS 6
#define COLS 7

// Couleurs ANSI
#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define GREEN   "\033[1;32m"

// Initialise la grille vide
void initBoard(char board[ROWS][COLS]) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            board[r][c] = ' ';
        }
    }
}

// Affiche la grille
void printBoard(char board[ROWS][COLS]) {
    system("clear"); // Nettoie la console sous Linux
    printf(BLUE "=== PUISSANCE 4 ===\n\n" RESET);
    for (int r = 0; r < ROWS; r++) {
        printf("|");
        for (int c = 0; c < COLS; c++) {
            char cell = board[r][c];
            if (cell == 'X') printf(RED " X " RESET "|");
            else if (cell == 'O') printf(YELLOW " O " RESET "|");
            else printf("   |");
        }
        printf("\n");
    }
    // Ligne du bas
    for (int c = 0; c < COLS; c++) printf("----");
    printf("-\n");
    // Numéros de colonnes
    for (int c = 0; c < COLS; c++) printf(" %d  ", c+1);
    printf("\n");
}

// Ajoute un pion dans une colonne
int dropPiece(char board[ROWS][COLS], int col, char piece) {
    if (col < 0 || col >= COLS) return 0; // invalide
    for (int r = ROWS-1; r >= 0; r--) {
        if (board[r][col] == ' ') {
            board[r][col] = piece;
            return 1;
        }
    }
    return 0; // colonne pleine
}

// Vérifie s’il y a 4 alignés
int checkWin(char board[ROWS][COLS], char piece) {
    // Horizontale
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS-3; c++) {
            if (board[r][c] == piece && board[r][c+1] == piece &&
                board[r][c+2] == piece && board[r][c+3] == piece) return 1;
        }
    }
    // Verticale
    for (int c = 0; c < COLS; c++) {
        for (int r = 0; r < ROWS-3; r++) {
            if (board[r][c] == piece && board[r+1][c] == piece &&
                board[r+2][c] == piece && board[r+3][c] == piece) return 1;
        }
    }
    // Diagonale descendante
    for (int r = 0; r < ROWS-3; r++) {
        for (int c = 0; c < COLS-3; c++) {
            if (board[r][c] == piece && board[r+1][c+1] == piece &&
                board[r+2][c+2] == piece && board[r+3][c+3] == piece) return 1;
        }
    }
    // Diagonale montante
    for (int r = 3; r < ROWS; r++) {
        for (int c = 0; c < COLS-3; c++) {
            if (board[r][c] == piece && board[r-1][c+1] == piece &&
                board[r-2][c+2] == piece && board[r-3][c+3] == piece) return 1;
        }
    }
    return 0;
}

// Vérifie si la grille est pleine
int isFull(char board[ROWS][COLS]) {
    for (int c = 0; c < COLS; c++) {
        if (board[0][c] == ' ') return 0;
    }
    return 1;
}

int main() {
    char board[ROWS][COLS];
    initBoard(board);

    char players[2] = {'X', 'O'};
    int currentPlayer = 0;

    while (1) {
        printBoard(board);
        printf("Joueur %d (%s%c%s), choisissez une colonne (1-%d) : ",
               currentPlayer+1,
               (players[currentPlayer]=='X'?RED:YELLOW),
               players[currentPlayer],
               RESET, COLS);

        int col;
        if (scanf("%d", &col) != 1) {
            printf(RED "Entree invalide.\n" RESET);
            return 1;
        }
        col--; // index 0-based

        if (!dropPiece(board, col, players[currentPlayer])) {
            printf(RED "Colonne invalide ou pleine ! Reessayez.\n" RESET);
            getchar(); // consomme le retour à la ligne
            continue;
        }

        if (checkWin(board, players[currentPlayer])) {
            printBoard(board);
            printf(GREEN ">>> Joueur %d gagne ! Felicitations !\n" RESET, currentPlayer+1);
            break;
        }

        if (isFull(board)) {
            printBoard(board);
            printf(YELLOW ">>> Match nul !\n" RESET);
            break;
        }

        currentPlayer = 1 - currentPlayer; // change de joueur
    }

    return 0;
}

