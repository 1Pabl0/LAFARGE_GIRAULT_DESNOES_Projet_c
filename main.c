#include "Partie_1.h"
#include "Partie_2.h"
#include "matrix.h" // <-- AJOUTÉ
#include <string.h>
#include <limits.h>

int main() {
    int choix;
    liste_adjacence G;       // structure du graphe
    int graphe_charge = 0;   // drapeau pour savoir si un graphe est déjà chargé
    char chemin[256];        // pour stocker le chemin du fichier

    // Pour stocker les résultats des Parties 2 et 3
    t_partition partition;
    int partition_calculee = 0;

    t_matrix M;              // Matrice de transition du graphe
    int matrice_chargee = 0;

    do {
        printf("\n==================================\n");
        printf("     PROJET GRAPHES DE MARKOV\n");
        printf("==================================\n");
        printf("--- Partie 1 ---\n");
        printf("1. Charger un graphe depuis un fichier\n");
        printf("2. Afficher la liste d'adjacence\n");
        printf("3. Vérifier si le graphe est de Markov\n");
        printf("\n--- Partie 2 ---\n");
        printf("4. Calculer les classes (Tarjan)\n");
        printf("\n--- Partie 3 ---\n");
        printf("5. Calculs de distributions (Matriciel)\n");
        printf("-----------------------------\n");
        printf("0. Quitter\n");
        printf("-----------------------------\n");
        printf("Votre choix : ");

        // --- GESTION DE LA SAISIE CORRIGÉE ---
        if (scanf("%d", &choix) != 1) {
            // Saisie invalide (ex: une lettre)
            printf("\n❌ Entrée invalide. Veuillez entrer un numéro.\n");
            choix = -1; // Valeur pour forcer le 'default'

            // Vide le buffer d'entrée
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        } else {
            // Saisie valide, consomme le \n restant
            getchar();
        }
        // --- FIN GESTION DE LA SAISIE ---

        switch (choix) {
            case 1: {
                printf("\nEntrez le chemin du fichier à charger (ex: ../data/meteo.txt) : ");
                fgets(chemin, sizeof(chemin), stdin);
                chemin[strcspn(chemin, "\n")] = '\0';

                // Libération des anciennes structures
                if (graphe_charge) {
                    // Libération de la liste d'adjacence G (si vous l'aviez fait, sinon il y a une fuite)
                    // NOTE: readGraph n'a pas de fonction de libération correspondante ici.
                }
                if (partition_calculee) {
                    liberer_partition(&partition);
                    partition_calculee = 0;
                }
                if (matrice_chargee) {
                    freeMatrix(&M);
                    matrice_chargee = 0;
                }

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

            case 4:
                if (!graphe_charge) {
                    printf("\n⚠️  Aucun graphe n'est chargé. Utilisez l'option 1 d'abord.\n");
                } else {
                    if (partition_calculee) {
                        liberer_partition(&partition);
                    }
                    printf("\n⚙️  Calcul des composantes fortement connexes (Tarjan)...\n");
                    partition = algorithme_tarjan(G);
                    partition_calculee = 1;
                    afficher_partition(partition);
                }
                break;

                // --- NOUVEAU CASE 5 (Partie 3) ---
            case 5: {
                if (!graphe_charge) {
                    printf("\n⚠️  Aucun graphe n'est chargé. Utilisez l'option 1 d'abord.\n");
                    break;
                }

                if (!matrice_chargee) {
                    M = createTransitionMatrix(G);
                    matrice_chargee = 1;
                }

                printf("\n=== Matrice de Transition M ===\n");
                displayMatrix(M);

                // 2. Calcul de M^3 (Exemple de validation)
                printf("\n=== Calcul de M^3 (puissance pour validation) ===\n");
                t_matrix M_power_n = createTransitionMatrix(G); // M^1 = M
                t_matrix M_temp = createEmptyMatrix(G.taille);

                // M^2 = M^1 * M
                multiplyMatrices(M_power_n, M, M_temp);
                copyMatrix(M_power_n, M_temp);

                // M^3 = M^2 * M
                multiplyMatrices(M_power_n, M, M_temp);
                copyMatrix(M_power_n, M_temp);

                printf("M^3:\n");
                displayMatrix(M_power_n);

                freeMatrix(&M_power_n);
                freeMatrix(&M_temp);


                // 3. Calcul de M^n pour la convergence
                printf("\n=== Calcul de la convergence (|M^n - M^(n-1)| < 0.01) ===\n");

                // M_curr stockera M^n, M_prev stockera M^(n-1)
                t_matrix M_curr = createTransitionMatrix(G); // Commence à M^1
                t_matrix M_prev = createEmptyMatrix(G.taille);
                t_matrix M_next = createEmptyMatrix(G.taille); // Pour stocker M^(n+1)

                float epsilon = 0.01f;
                float diff = 100.0f; // Initialisation > epsilon
                int n = 1;

                while (n < 50 && diff >= epsilon) { // Limite de 50 itérations

                    // M_prev = M^n (pour comparaison)
                    copyMatrix(M_prev, M_curr);

                    // Calcul M^(n+1) = M^n * M
                    multiplyMatrices(M_curr, M, M_next);

                    // M_curr devient M^(n+1)
                    copyMatrix(M_curr, M_next);

                    // Calcule la différence |M^(n+1) - M^n|
                    diff = diffMatrix(M_curr, M_prev);

                    n++;
                }
                n--; // Rétablir l'indice à la dernière matrice calculée

                if (diff < epsilon) {
                    printf("\n✅ Convergence atteinte à n = %d.\n", n);
                    printf("Différence |M^n - M^(n-1)| = %.6f\n", diff);
                    printf("\n=== M^%d (Matrice stationnaire/limite) ===\n", n);
                    displayMatrix(M_curr);
                } else {
                    printf("\n⚠️  La convergence n'a pas été atteinte après %d itérations. Dernière différence: %.6f\n", n, diff);
                    printf("Dernière matrice M^n:\n");
                    displayMatrix(M_curr);
                }

                // Libération des matrices de convergence
                freeMatrix(&M_curr);
                freeMatrix(&M_prev);
                freeMatrix(&M_next);

                break;
            }

            case 0:
                if (partition_calculee) {
                    liberer_partition(&partition);
                }
                if (matrice_chargee) {
                    freeMatrix(&M);
                }
                printf("\n👋 Fin du programme. Au revoir !\n");
                break;

            default:
                if (choix != -1) { // Évite de réafficher si l'erreur de saisie a déjà affiché un message
                    printf("\n❌ Choix invalide. Réessayez.\n");
                }
        }
    } while (choix != 0);

    return 0;
}