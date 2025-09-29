#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define NUM_COLORS 4
#define NUM_VALUES 10
#define HAND_SIZE 7
#define DECK_SIZE (NUM_COLORS * NUM_VALUES)

typedef struct {
    char color[10];
    int value;
} Card;

const char *colors[NUM_COLORS] = {"Rouge", "Jaune", "Vert", "Bleu"};

// Mélange le deck
void shuffle(Card *deck, int size) {
    for (int i = 0; i < size; i++) {
        int j = rand() % size;
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

// Affiche une carte
void printCard(Card c) {
    if (strcmp(c.color, "Rouge") == 0)
        printf("\033[1;31m[%s %d]\033[0m", c.color, c.value);
    else if (strcmp(c.color, "Jaune") == 0)
        printf("\033[1;33m[%s %d]\033[0m", c.color, c.value);
    else if (strcmp(c.color, "Vert") == 0)
        printf("\033[1;32m[%s %d]\033[0m", c.color, c.value);
    else if (strcmp(c.color, "Bleu") == 0)
        printf("\033[1;34m[%s %d]\033[0m", c.color, c.value);
}

// Vérifie si une carte peut être jouée
int canPlay(Card c, Card top) {
    return (c.value == top.value || strcmp(c.color, top.color) == 0);
}

// Affiche la main d’un joueur
void printHand(Card *hand, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d:", i+1);
        printCard(hand[i]);
        printf("  ");
    }
    printf("\n");
}

int main() {
    srand(time(NULL));

    // Créer le deck
    Card deck[DECK_SIZE];
    int index = 0;
    for (int c = 0; c < NUM_COLORS; c++) {
        for (int v = 0; v < NUM_VALUES; v++) {
            strcpy(deck[index].color, colors[c]);
            deck[index].value = v;
            index++;
        }
    }

    shuffle(deck, DECK_SIZE);

    // Mains
    Card player[50]; int playerCount = 0;
    Card bot[50]; int botCount = 0;
    int deckPos = 0;

    // Distribuer 7 cartes chacun
    for (int i = 0; i < HAND_SIZE; i++) {
        player[playerCount++] = deck[deckPos++];
        bot[botCount++] = deck[deckPos++];
    }

    // Première carte de la pile
    Card top = deck[deckPos++];

    printf("=== UNO Simplifie ===\n");

    while (1) {
        printf("\nCarte sur la pile : ");
        printCard(top);
        printf("\n");

        // Tour du joueur
        printf("Vos cartes :\n");
        printHand(player, playerCount);

        int choice;
        printf("Choisissez une carte (1-%d) ou 0 pour piocher : ", playerCount);
        scanf("%d", &choice);

        if (choice == 0) { // pioche
            if (deckPos < DECK_SIZE) {
                player[playerCount++] = deck[deckPos++];
                printf("Vous piochez une carte.\n");
            }
        } else if (choice > 0 && choice <= playerCount) {
            if (canPlay(player[choice-1], top)) {
                top = player[choice-1];
                printf("Vous jouez ");
                printCard(top);
                printf("\n");
                for (int i = choice-1; i < playerCount-1; i++)
                    player[i] = player[i+1];
                playerCount--;
            } else {
                printf("Carte invalide, vous perdez votre tour.\n");
            }
        }

        if (playerCount == 0) {
            printf("\n>>> Vous avez gagne ! Bravo !\n");
            break;
        }

        // Tour du bot
        printf("\nTour du robot...\n");
        int played = 0;
        for (int i = 0; i < botCount; i++) {
            if (canPlay(bot[i], top)) {
                top = bot[i];
                printf("Le robot joue ");
                printCard(top);
                printf("\n");
                for (int j = i; j < botCount-1; j++)
                    bot[j] = bot[j+1];
                botCount--;
                played = 1;
                break;
            }
        }
        if (!played) {
            if (deckPos < DECK_SIZE) {
                bot[botCount++] = deck[deckPos++];
                printf("Le robot pioche une carte.\n");
            }
        }

        if (botCount == 0) {
            printf("\n>>> Le robot a gagne ! Dommage...\n");
            break;
        }
    }

    return 0;
}

