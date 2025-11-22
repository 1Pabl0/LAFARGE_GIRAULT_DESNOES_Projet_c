//
// Created by Paul Girault on 13/11/2025.
//

#ifndef LAFARGE_GIRAULT_DESNOES_PROJET_C_MATRIX_H
#define LAFARGE_GIRAULT_DESNOES_PROJET_C_MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include "Partie_1.h" // Pour liste_adjacence

// Déclarations forward pour éviter dépendances circulaires
struct t_partition;
struct t_classe;

// Définition d'un type pour les éléments de la matrice
typedef float t_mat_elem;

// Structure pour une matrice carrée
typedef struct {
    int rows;
    t_mat_elem **data;
} t_matrix;

// PROTOTYPES UNIQUEMENT (pas d'implémentation !)
t_matrix createEmptyMatrix(int n);
void freeMatrix(t_matrix *m);
t_matrix createTransitionMatrix(liste_adjacence G);
void copyMatrix(t_matrix dest, t_matrix src);
void multiplyMatrices(t_matrix A, t_matrix B, t_matrix C);
t_mat_elem diffMatrix(t_matrix M, t_matrix N);
void displayMatrix(t_matrix m);
t_matrix subMatrix(t_matrix matrix, struct t_partition *part, int compo_index);
int gcd(int *vals, int nbvals);
int getPeriod(t_matrix sub_matrix);

#endif //LAFARGE_GIRAULT_DESNOES_PROJET_C_MATRIX_H