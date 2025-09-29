// Fichier : Calculatrice.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Définition des couleurs
#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"

// Fonction pour calculer la factorielle
long long factorielle(int n) {
    if (n < 0) return -1; // erreur
    long long resultat = 1;
    for (int i = 1; i <= n; i++) {
        resultat *= i;
    }
    return resultat;
}

int main() {
    int choix;
    double a, b;

    do {
        printf("\n" BLUE "=== CALCULATRICE SCIENTIFIQUE ===" RESET "\n");
        printf(CYAN "1." RESET " Addition\n");
        printf(CYAN "2." RESET " Soustraction\n");
        printf(CYAN "3." RESET " Multiplication\n");
        printf(CYAN "4." RESET " Division\n");
        printf(CYAN "5." RESET " Puissance\n");
        printf(CYAN "6." RESET " Racine carrée\n");
        printf(CYAN "7." RESET " Modulo (reste de division entière)\n");
        printf(CYAN "8." RESET " Factorielle\n");
        printf(CYAN "9." RESET " Sinus\n");
        printf(CYAN "10." RESET " Cosinus\n");
        printf(CYAN "11." RESET " Tangente\n");
        printf(CYAN "12." RESET " Logarithme naturel (ln)\n");
        printf(CYAN "13." RESET " Logarithme base 10\n");
        printf(CYAN "14." RESET " Valeur absolue\n");
        printf(CYAN "0." RESET " Quitter\n");
        printf(YELLOW "Votre choix : " RESET);
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                printf("Entrez deux nombres : ");
                scanf("%lf %lf", &a, &b);
                printf(GREEN "Résultat = %.2lf\n" RESET, a + b);
                break;

            case 2:
                printf("Entrez deux nombres : ");
                scanf("%lf %lf", &a, &b);
                printf(GREEN "Résultat = %.2lf\n" RESET, a - b);
                break;

            case 3:
                printf("Entrez deux nombres : ");
                scanf("%lf %lf", &a, &b);
                printf(GREEN "Résultat = %.2lf\n" RESET, a * b);
                break;

            case 4:
                printf("Entrez deux nombres : ");
                scanf("%lf %lf", &a, &b);
                if (b == 0) printf(RED "Erreur : division par zéro.\n" RESET);
                else printf(GREEN "Résultat = %.2lf\n" RESET, a / b);
                break;

            case 5:
                printf("Entrez la base et l’exposant : ");
                scanf("%lf %lf", &a, &b);
                printf(GREEN "Résultat = %.2lf\n" RESET, pow(a, b));
                break;

            case 6:
                printf("Entrez un nombre : ");
                scanf("%lf", &a);
                if (a < 0) printf(RED "Erreur : racine négative.\n" RESET);
                else printf(GREEN "Résultat = %.2lf\n" RESET, sqrt(a));
                break;

            case 7: {
                int x, y;
                printf("Entrez deux entiers : ");
                scanf("%d %d", &x, &y);
                if (y == 0) printf(RED "Erreur : division par zéro.\n" RESET);
                else printf(GREEN "Résultat = %d\n" RESET, x % y);
            } break;

            case 8: {
                int n;
                printf("Entrez un entier : ");
                scanf("%d", &n);
                long long res = factorielle(n);
                if (res == -1) printf(RED "Erreur : factorielle négative.\n" RESET);
                else printf(GREEN "Résultat = %lld\n" RESET, res);
            } break;

            case 9:
                printf("Entrez un angle en radians : ");
                scanf("%lf", &a);
                printf(GREEN "Résultat = %.5lf\n" RESET, sin(a));
                break;

            case 10:
                printf("Entrez un angle en radians : ");
                scanf("%lf", &a);
                printf(GREEN "Résultat = %.5lf\n" RESET, cos(a));
                break;

            case 11:
                printf("Entrez un angle en radians : ");
                scanf("%lf", &a);
                printf(GREEN "Résultat = %.5lf\n" RESET, tan(a));
                break;

            case 12:
                printf("Entrez un nombre : ");
                scanf("%lf", &a);
                if (a <= 0) printf(RED "Erreur : ln(x) défini seulement pour x>0.\n" RESET);
                else printf(GREEN "Résultat = %.5lf\n" RESET, log(a));
                break;

            case 13:
                printf("Entrez un nombre : ");
                scanf("%lf", &a);
                if (a <= 0) printf(RED "Erreur : log10(x) défini seulement pour x>0.\n" RESET);
                else printf(GREEN "Résultat = %.5lf\n" RESET, log10(a));
                break;

            case 14:
                printf("Entrez un nombre : ");
                scanf("%lf", &a);
                printf(GREEN "Résultat = %.2lf\n" RESET, fabs(a));
                break;

            case 0:
                printf(BLUE "Au revoir ! 👋\n" RESET);
                break;

            default:
                printf(RED "Choix invalide.\n" RESET);
        }
    } while (choix != 0);

    return 0;
}

