// Fichier : MonCarnet.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING 100

typedef struct {
    char nom[MAX_STRING];
    char prenom[MAX_STRING];
    char telephone[MAX_STRING];
    char email[MAX_STRING];
} Contact;

Contact *contacts = NULL;
int nbContacts = 0;

// Charger les contacts depuis un fichier CSV
void chargerContacts() {
    FILE *f = fopen("contacts.csv", "r");
    if (f == NULL) return;

    char ligne[500];
    while (fgets(ligne, sizeof(ligne), f)) {
        Contact c;
        if (sscanf(ligne, "%99[^,],%99[^,],%99[^,],%99[^\n]",
                   c.nom, c.prenom, c.telephone, c.email) == 4) {
            contacts = realloc(contacts, (nbContacts + 1) * sizeof(Contact));
            contacts[nbContacts++] = c;
        }
    }
    fclose(f);
}

// Sauvegarder les contacts dans un fichier CSV
void sauvegarderContacts() {
    FILE *f = fopen("contacts.csv", "w");
    if (f == NULL) {
        printf("Erreur d’ouverture fichier.\n");
        return;
    }
    for (int i = 0; i < nbContacts; i++) {
        fprintf(f, "%s,%s,%s,%s\n",
                contacts[i].nom,
                contacts[i].prenom,
                contacts[i].telephone,
                contacts[i].email);
    }
    fclose(f);
}

// Ajouter un contact
void ajouterContact() {
    Contact c;
    printf("Nom : "); scanf("%99s", c.nom);
    printf("Prénom : "); scanf("%99s", c.prenom);
    printf("Téléphone : "); scanf("%99s", c.telephone);
    printf("Email : "); scanf("%99s", c.email);

    contacts = realloc(contacts, (nbContacts + 1) * sizeof(Contact));
    contacts[nbContacts++] = c;
    printf("Contact ajouté !\n");
}

// Afficher tous les contacts
void afficherContacts() {
    if (nbContacts == 0) {
        printf("Aucun contact.\n");
        return;
    }
    for (int i = 0; i < nbContacts; i++) {
        printf("%d. %s %s | %s | %s\n",
               i + 1,
               contacts[i].prenom,
               contacts[i].nom,
               contacts[i].telephone,
               contacts[i].email);
    }
}

// Rechercher un contact par sous-chaîne
void rechercherContact() {
    char recherche[MAX_STRING];
    printf("Nom ou partie de nom à rechercher : ");
    scanf("%99s", recherche);

    int trouve = 0;
    for (int i = 0; i < nbContacts; i++) {
        if (strstr(contacts[i].nom, recherche) != NULL) {
            printf("Trouvé : %s %s | %s | %s\n",
                   contacts[i].prenom,
                   contacts[i].nom,
                   contacts[i].telephone,
                   contacts[i].email);
            trouve = 1;
        }
    }
    if (!trouve) printf("Aucun contact trouvé.\n");
}

// Supprimer un contact
void supprimerContact() {
    int index;
    printf("Numéro du contact à supprimer : ");
    scanf("%d", &index);

    if (index < 1 || index > nbContacts) {
        printf("Numéro invalide.\n");
        return;
    }
    for (int i = index - 1; i < nbContacts - 1; i++) {
        contacts[i] = contacts[i + 1];
    }
    nbContacts--;
    contacts = realloc(contacts, nbContacts * sizeof(Contact));
    printf("Contact supprimé.\n");
}

// Modifier un contact
void modifierContact() {
    int index;
    printf("Numéro du contact à modifier : ");
    scanf("%d", &index);

    if (index < 1 || index > nbContacts) {
        printf("Numéro invalide.\n");
        return;
    }

    Contact *c = &contacts[index - 1];
    printf("Nouveau nom (%s) : ", c->nom); scanf("%99s", c->nom);
    printf("Nouveau prénom (%s) : ", c->prenom); scanf("%99s", c->prenom);
    printf("Nouveau téléphone (%s) : ", c->telephone); scanf("%99s", c->telephone);
    printf("Nouveau email (%s) : ", c->email); scanf("%99s", c->email);

    printf("Contact modifié.\n");
}

// Trier les contacts par nom (bubble sort)
void trierContacts() {
    for (int i = 0; i < nbContacts - 1; i++) {
        for (int j = 0; j < nbContacts - i - 1; j++) {
            if (strcmp(contacts[j].nom, contacts[j + 1].nom) > 0) {
                Contact tmp = contacts[j];
                contacts[j] = contacts[j + 1];
                contacts[j + 1] = tmp;
            }
        }
    }
    printf("Contacts triés par nom.\n");
}

// Menu principal
int main() {
    int choix;
    chargerContacts();

    do {
        printf("\n=== MonCarnet (Carnet de contacts) ===\n");
        printf("1. Ajouter un contact\n");
        printf("2. Afficher tous les contacts\n");
        printf("3. Rechercher un contact\n");
        printf("4. Supprimer un contact\n");
        printf("5. Modifier un contact\n");
        printf("6. Trier par nom\n");
        printf("7. Sauvegarder et quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1: ajouterContact(); break;
            case 2: afficherContacts(); break;
            case 3: rechercherContact(); break;
            case 4: supprimerContact(); break;
            case 5: modifierContact(); break;
            case 6: trierContacts(); break;
            case 7: sauvegarderContacts(); printf("Au revoir !\n"); break;
            default: printf("Choix invalide.\n");
        }
    } while (choix != 7);

	free(contacts);
	return 0;
}

