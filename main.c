#include "Partie_1.h"
#include <string.h>
int main() {
    int choix;
    liste_adjacence G;       // structure du graphe
    int graphe_charge = 0;   // drapeau pour savoir si un graphe est déjà chargé
    char chemin[256];        // pour stocker le chemin du fichier

    do {
        printf("\n=============================\n");
        printf("     PROJET GRAPHES DE MARKOV\n");
        printf("=============================\n");
        printf("1. Charger un graphe depuis un fichier\n");
        printf("2. Afficher la liste d'adjacence\n");
        printf("3. Vérifier si le graphe est de Markov\n");
        printf("0. Quitter\n");
        printf("-----------------------------\n");
        printf("Votre choix : ");
        scanf("%d", &choix);
        getchar(); // consomme le \n

        switch (choix) {
            case 1: {
                printf("\nEntrez le chemin du fichier à charger (ex : ../data/exemple1.txt) : ");
                fgets(chemin, sizeof(chemin), stdin);
                // Supprimer le retour à la ligne (\n)
                chemin[strcspn(chemin, "\n")] = '\0';

                G = readGraph(chemin);
                graphe_charge = 1;
                printf("\n✅ Graphe chargé avec succès depuis '%s'.\n", chemin);
                break;
            }

            case 2:
                if (!graphe_charge) {
                    printf("\n⚠️  Aucun graphe n'est chargé. Utilisez l'option 1 d'abord.\n");
                } else {
                    printf("\n=== Liste d'adjacence du graphe ===\n");
                    for (int i = 0; i < G.taille; i++) {
                        printf("Sommet %d : ", i + 1);
                        afficher_liste(G.tab[i]);
                    }
                }
                break;

            case 3:
                if (!graphe_charge) {
                    printf("\n⚠️  Aucun graphe n'est chargé. Utilisez l'option 1 d'abord.\n");
                } else {
                    verifier_markov(G);
                }
                break;

            case 0:
                printf("\n👋 Fin du programme. Au revoir !\n");
                break;

            default:
                printf("\n❌ Choix invalide. Réessayez.\n");
        }
    } while (choix != 0);

    return 0;
}