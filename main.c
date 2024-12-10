#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DEMANDES 100
#define MAX_EVENEMENTS 100
#define MAX_RESOURCES 100

struct demande {
    int id;
    char name[50];
    char adresse[100];
    int priorite;
    char statue[20];
};

struct evenement {
    int id;
    char type[50];
    char statue[20];
    int priorite;
    char localisation[100];
};

struct Resource {
    int id;
    char type[50];
    char statue[20];
    char location[100];
    struct Resource *next;
};

int nbDemandes = 0;
int nbEvenements = 0;
int nextId = 1;
struct demande demandes[MAX_DEMANDES];
struct evenement evenements[MAX_EVENEMENTS];
struct Resource *resourceList = NULL;

void declare() {
    if (nbDemandes >= MAX_DEMANDES) {
        printf("Limite maximale de demandes atteinte.\n");
        return;
    }

    struct demande newDemande;
    newDemande.id = nbDemandes + 1;
    printf("Entrer le nom de la personne : ");
    scanf("%s", newDemande.name);

    printf("Entrer l'adresse : ");
    scanf("%s", newDemande.adresse);

    printf("Entrer la priorité (1: faible, 2: moyenne, 3: élevée) : ");
    scanf("%d", &newDemande.priorite);

    printf("Entrer le statut de la demande : ");
    scanf("%s", newDemande.statue);

    demandes[nbDemandes] = newDemande;
    nbDemandes++;
    printf("Demande d'urgence enregistrée avec succès !\n");
}

void suivre() {
    int idDemande;
    printf("Entrer l'ID de la demande à suivre : ");
    scanf("%d", &idDemande);

    if (idDemande <= 0 || idDemande > nbDemandes) {
        printf("Demande non trouvée avec cet ID.\n");
        return;
    }

    struct demande current = demandes[idDemande - 1];
    printf("\nDemande trouvée :\n");
    printf("ID: %d\n", current.id);
    printf("Nom: %s\n", current.name);
    printf("Adresse: %s\n", current.adresse);
    printf("Priorite: %d\n", current.priorite);
    printf("Statut: %s\n", current.statue);
}

void ajouter_resource() {
    struct Resource *newResource = (struct Resource *)malloc(sizeof(struct Resource));

    printf("Entrez le type de ressource : ");
    scanf("%s", newResource->type);

    printf("Entrez le statut (Disponible/Occupé/Maintenance) : ");
    scanf("%s", newResource->statue);

    printf("Entrez la localisation : ");
    scanf("%s", newResource->location);

    newResource->id = nextId++;
    newResource->next = resourceList;
    resourceList = newResource;

    printf("Ressource ajoutée avec succès !\n");
}

void modifier_resource() {
    int id;
    printf("Entrez l'ID de la ressource à modifier : ");
    scanf("%d", &id);

    struct Resource *current = resourceList;
    while (current) {
        if (current->id == id) {
            printf("Entrez le nouveau type de ressource : ");
            scanf("%s", current->type);

            printf("Entrez le nouveau statut (Disponible/Occupé/Maintenance) : ");
            scanf("%s", current->statue);

            printf("Entrez la nouvelle localisation : ");
            scanf("%s", current->location);

            printf("Ressource modifiée avec succès !\n");
            return;
        }
        current = current->next;
    }
    printf("Ressource avec ID %d non trouvée.\n", id);
}

void listResources() {
    struct Resource *current = resourceList;
    if (!current) {
        printf("Aucune ressource disponible.\n");
        return;
    }

    printf("\nListe des ressources :\n");
    while (current) {
        printf("ID: %d | Type: %s | Statue: %s | Localisation: %s\n",
               current->id, current->type, current->statue, current->location);
        current = current->next;
    }
}

void enregistrer_resources() {
    FILE *file = fopen("resources.txt", "w");
    if (file == NULL) {
        printf("Erreur d'ouverture du fichier.\n");
        return;
    }

    struct Resource *current = resourceList;
    while (current != NULL) {
        fprintf(file, "%d %s %s %s\n", current->id, current->type, current->statue, current->location);
        current = current->next;
    }

    fclose(file);
    printf("Ressources sauvegardées dans le fichier.\n");
}

void charger_resources() {
    FILE *file = fopen("resources.txt", "r");
    if (file == NULL) {
        printf("Aucun fichier trouvé. Pas de ressources chargées.\n");
        return;
    }

    while (1) {
        struct Resource *newResource = (struct Resource *)malloc(sizeof(struct Resource));
        if (fscanf(file, "%d %s %s %s", &newResource->id, newResource->type, newResource->statue, newResource->location) != 4) {
            free(newResource);
            break;
        }
        newResource->next = resourceList;
        resourceList = newResource;
    }

    fclose(file);
    printf("Ressources chargées depuis le fichier.\n");
}

void menu(){
    int choix;
    while(1) {
        printf("SOYEZ LE BIENVENUE\n");
        printf("***************************\n");
        printf("1)AJOUTER RESOURCES\n");
        printf("2)MODIFIER RESSOURCES\n");
        printf("3)DECLARE UN CAS D'URGENCES\n");
        printf("4)SUIVRE LA DEMANDE\n");
        printf("5)SORTIR\n");
        printf("***************************\n");
        printf("entrer votre choix\n");
        scanf("%d",&choix);
        switch (choix){
          case 1: ajouter_resource(); break;
          case 2: modifier_resource(); break;
          case 3: declare(); break;
          case 4: suivre(); break;
          case 5: return;
          default: printf("Choix invalide.\n");
        }
    }
}

int main() {
    charger_resources();
    menu();
    enregistrer_resources();
    return 0;
}
