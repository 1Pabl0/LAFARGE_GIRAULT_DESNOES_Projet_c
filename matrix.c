//
// Created by Paul Girault on 13/11/2025.
//

#include "matrix.h"
#include "Partie_2.h"
#include <math.h> // Pour fabsf()


// IMPLÉMENTATION - ÉTAPE 1 (MATRICIEL)


// Allocation et initialisation à zéro
t_matrix createEmptyMatrix(int n) {
    t_matrix m;
    m.rows = n;

    // Allocation des lignes
    m.data = (t_mat_elem**)malloc(n * sizeof(t_mat_elem*));
    if (m.data == NULL) {
        perror("Erreur d'allocation pour les lignes de la matrice");
        exit(EXIT_FAILURE);
    }

    // Allocation des colonnes et initialisation à 0.0f
    for (int i = 0; i < n; i++) {
        m.data[i] = (t_mat_elem*)calloc(n, sizeof(t_mat_elem)); // calloc met à zéro
        if (m.data[i] == NULL) {
            perror("Erreur d'allocation pour les colonnes de la matrice");
            // Libérer les lignes déjà allouées avant de quitter
            for (int j = 0; j < i; j++) free(m.data[j]);
            free(m.data);
            exit(EXIT_FAILURE);
        }
    }

    return m;
}

// Libération de la mémoire
void freeMatrix(t_matrix *m) {
    if (m != NULL && m->data != NULL) {
        for (int i = 0; i < m->rows; i++) {
            free(m->data[i]);
        }
        free(m->data);
        m->data = NULL;
        m->rows = 0;
    }
}

// Création de la matrice de transition M à partir de la liste d'adjacence G
t_matrix createTransitionMatrix(liste_adjacence G) {
    int n = G.taille;
    t_matrix M = createEmptyMatrix(n);

    for (int i = 0; i < n; i++) {
        // i est l'indice 0-indexé (sommet i+1)
        cellule *tmp = G.tab[i].head;
        while (tmp != NULL) {
            // j est l'indice 0-indexé (sommet tmp->sommet_arrivee)
            int j = tmp->sommet_arrivee - 1;
            if (j >= 0 && j < n) {
                M.data[i][j] = tmp->proba;
            }
            tmp = tmp->suiv;
        }
    }
    return M;
}

// Recopie des valeurs d'une matrice
void copyMatrix(t_matrix dest, t_matrix src) {
    if (dest.rows != src.rows) {
        fprintf(stderr, "Erreur: Les matrices n'ont pas la même taille pour la copie.\n");
        return;
    }
    int n = src.rows;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dest.data[i][j] = src.data[i][j];
        }
    }
}

// Multiplication de deux matrices: C = A * B
void multiplyMatrices(t_matrix A, t_matrix B, t_matrix C) {
    int n = A.rows;
    if (n != B.rows || n != C.rows) {
        fprintf(stderr, "Erreur: Les matrices ne sont pas de taille compatible pour la multiplication.\n");
        return;
    }

    // Utilisation d'une matrice temporaire pour éviter d'écraser A ou B si C=A ou C=B
    t_matrix temp = createEmptyMatrix(n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            t_mat_elem sum = 0.0f;
            for (int k = 0; k < n; k++) {
                sum += A.data[i][k] * B.data[k][j];
            }
            temp.data[i][j] = sum;
        }
    }

    // Recopie du résultat dans la matrice C
    copyMatrix(C, temp);

    // Libération de la matrice temporaire
    freeMatrix(&temp);
}

// Calcul de la différence |M - N|
t_mat_elem diffMatrix(t_matrix M, t_matrix N) {
    if (M.rows != N.rows) {
        fprintf(stderr, "Erreur: Les matrices n'ont pas la même taille pour le calcul de différence.\n");
        return -1.0f; // Valeur d'erreur
    }

    int n = M.rows;
    t_mat_elem diff_sum = 0.0f;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // fabsf pour la valeur absolue d'un float
            diff_sum += fabsf(M.data[i][j] - N.data[i][j]);
        }
    }
    return diff_sum;
}

// Affichage d'une matrice
void displayMatrix(t_matrix m) {
    printf("Matrice (%d x %d):\n", m.rows, m.rows);
    for (int i = 0; i < m.rows; i++) {
        printf("  |");
        for (int j = 0; j < m.rows; j++) {
            printf(" %.4f", m.data[i][j]);
        }
        printf(" |\n");
    }
}


// IMPLÉMENTATION ÉTAPE 2 (SOUS-MATRICE)

t_matrix subMatrix(t_matrix matrix, struct t_partition *part, int compo_index) {
    if (compo_index < 0 || compo_index >= part->nb_classes) {
        fprintf(stderr, "Erreur: Indice de classe invalide.\n");
        // Retourne une matrice vide (taille 0)
        t_matrix m;
        m.rows = 0;
        m.data = NULL;
        return m;
    }

    t_classe compo = part->classes[compo_index];
    int n_sub = compo.nb_sommets;
    t_matrix sub_m = createEmptyMatrix(n_sub);

    // Extraction de la sous-matrice
    // Pour chaque sommet i de la classe (ligne)
    for (int i = 0; i < n_sub; i++) {
        int sommet_i = compo.sommets[i]; // ID du sommet (1-indexé)
        int idx_i = sommet_i - 1;        // Indice dans la matrice originale (0-indexé)

        // Pour chaque sommet j de la classe (colonne)
        for (int j = 0; j < n_sub; j++) {
            int sommet_j = compo.sommets[j]; // ID du sommet (1-indexé)
            int idx_j = sommet_j - 1;        // Indice dans la matrice originale (0-indexé)

            // Copie de la valeur de la matrice originale
            if (idx_i >= 0 && idx_i < matrix.rows && idx_j >= 0 && idx_j < matrix.rows) {
                sub_m.data[i][j] = matrix.data[idx_i][idx_j];
            }
        }
    }

    return sub_m;
}


//Etape 3 :
int gcd(int *vals, int nbvals) {
    if (nbvals == 0) return 0;

    // Initialise le résultat avec la première valeur.
    int result = vals[0];

    // Parcourt le reste du tableau et applique le PGCD au résultat cumulé.
    for (int i = 1; i < nbvals; i++) {
        int a = result;
        int b = vals[i];

        // Algorithme d'Euclide : PGCD(a, b) = PGCD(b, a mod b)
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }

        // Le PGCD(result, vals[i]) est stocké dans 'a'
        result = a;
    }
    return result;
}

// [DÉFI 1 - Commentaire et Explication]
// Calcule la période d'une classe en cherchant les longueurs 'n' des chemins
// qui reviennent à un état (M_sub^n a une diagonale non nulle).
int getPeriod(t_matrix sub_matrix)
{
    int n = sub_matrix.rows;
    if (n == 0) return 0;

    // On alloue un tableau assez grand pour les périodes.
    // L'analyse montre que le nombre de sommets 'n' suffit comme taille max.
    int *periods = (int*)malloc(n * sizeof(int));
    if (!periods) {
        perror("Erreur d'allocation périodes");
        exit(EXIT_FAILURE);
    }
    int period_count = 0;

    // M_sub^cpt. Commence à M_sub^1.
    t_matrix power_matrix = createEmptyMatrix(n);
    copyMatrix(power_matrix, sub_matrix);

    // Matrice temporaire pour le calcul de la puissance suivante
    t_matrix result_matrix = createEmptyMatrix(n);

    // On itère de cpt=1 à n*n (sécurité) pour trouver toutes les longueurs de cycles.
    for (int cpt = 1; cpt <= n * n; cpt++)
    {
        int diag_nonzero = 0;
        for (int i = 0; i < n; i++)
        {
            // Un coefficient diagonal > 0.0f indique un chemin de retour i -> i en 'cpt' étapes.
            if (power_matrix.data[i][i] > 0.0f)
            {
                diag_nonzero = 1;
                break;
            }
        }

        if (diag_nonzero) {
            // Correction du bug : vérifier l'espace avant d'ajouter
            if (period_count < n) {
                periods[period_count] = cpt;
                period_count++;
            }
        }

        // S'il ne reste plus de place pour stocker, on break.
        if (period_count >= n) break;

        // Calcul de la puissance suivante : M_sub^(cpt+1) = M_sub^cpt * M_sub
        multiplyMatrices(power_matrix, sub_matrix, result_matrix);
        copyMatrix(power_matrix, result_matrix);
    }

    freeMatrix(&power_matrix);
    freeMatrix(&result_matrix);

    // La période est le PGCD de toutes les longueurs de cycles trouvées.
    int period = gcd(periods, period_count);
    free(periods);

    return period;
}