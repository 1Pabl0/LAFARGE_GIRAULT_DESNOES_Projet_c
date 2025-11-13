//
// Created by Paul Girault on 13/11/2025.
//

#ifndef LAFARGE_GIRAULT_DESNOES_PROJET_C_MATRIX_H
#define LAFARGE_GIRAULT_DESNOES_PROJET_C_MATRIX_H

#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include "Partie_1.h" // Pour liste_adjacence
#include "Partie_2.h" // Pour t_partition

// Définition d'un type pour les éléments de la matrice
typedef float t_mat_elem;

// ===============================================
// === STRUCTURES MATRICES ===
// ===============================================


typedef struct {
    int rows;           // Nombre de lignes (égal au nombre de colonnes)
    t_mat_elem **data;  // Tableau 2D de données (matrice)
} t_matrix;


// ===============================================
// === PROTOTYPES FONCTIONS - ÉTAPE 1 (MATRICIEL) ===
// ===============================================


t_matrix createEmptyMatrix(int n);

void freeMatrix(t_matrix *m);

t_matrix createTransitionMatrix(liste_adjacence G);


void copyMatrix(t_matrix dest, t_matrix src);

void multiplyMatrices(t_matrix A, t_matrix B, t_matrix C);


t_mat_elem diffMatrix(t_matrix M, t_matrix N);


void displayMatrix(t_matrix m);



// === PROTOTYPES FONCTIONS - ÉTAPE 2 (SOUS-MATRICE) ===

t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index);


#endif // MATRIX_H


#endif //LAFARGE_GIRAULT_DESNOES_PROJET_C_MATRIX_H
