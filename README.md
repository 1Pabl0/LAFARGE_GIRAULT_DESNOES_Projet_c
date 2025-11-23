# LAFARGE_GIRAULT_DESNOES_Projet_c
# README -- Projet C : Étude Complète de Graphes de Markov

## 📘 Introduction

Ce projet implémente l'intégralité des traitements liés à des **graphes
de Markov à temps discret**, conformément aux trois parties du sujet
fourni.\
Il couvre : - La construction du graphe (Partie 1)\
- La décomposition en **composantes fortement connexes** (Tarjan), la
construction du **diagramme de Hasse** et l'analyse Markovienne (Partie
2)\
- Le calcul des **distributions**, des **puissances de matrices**, des
**distributions stationnaires** et de la **périodicité** (Partie 3)

Le programme est entièrement réalisé en C.

------------------------------------------------------------------------

# PARTIE 1 --- Construction et vérification du graphe de Markov

Cette partie correspond au cahier des charges du PDF Partie 1 (pages 1 à
7).

Elle comporte : - Lecture du graphe depuis un fichier - Construction de
la **liste d'adjacence** - Vérification de la propriété de Markov -
Export du graphe au format **Mermaid**

## 🔧 Structures de données (Partie 1)

### `cellule`

Représente une arête sortante :

``` c
typedef struct cellule {
    int sommet_arrivee;
    float proba;
    struct cellule *suiv;
} cellule;
```

### `liste`

Une liste chaînée d'arêtes :

``` c
typedef struct {
    cellule *head;
} liste;
```

### `liste_adjacence`

Tableau de listes, une par sommet :

``` c
typedef struct {
    int taille;
    liste *tab;
} liste_adjacence;
```

## 🧩 Fonctions principales (Partie 1)

### `creer_cellule(int arrivee, float proba)`

Crée un maillon de liste représentant une arête.

### `creer_liste_vide()`

Initialise une liste vide.

### `ajouter_cellule(liste *l, int arrivee, float proba)`

Ajoute une arête au sommet donné.

### `readGraph(const char *filename)`

**Lit un fichier texte** et construit le graphe selon le format :

    nbSommets
    depart arrivee proba
    ...

Le rôle principal du paramètre `nbvert` correspond au nombre de sommets du graphe.

### `verifier_markov(liste_adjacence G)`

Vérifie que **la somme des probabilités sortantes vaut 1** pour chaque
sommet (tolérance ±0.01).

### `generer_fichier_mermaid(liste_adjacence G, char *filename)`

Génère un fichier `.mmd` affichable dans MermaidChart.

------------------------------------------------------------------------

# PARTIE 2 --- Algorithme de Tarjan, Partition, Hasse, Analyse Markovienne

Cette partie suit les spécifications du PDF Partie 2 (pages 1 à 9).

Elle comprend : - L'implémentation complète du **Tarjan** pour détecter
les CFC - Construction d'une **partition** - Construction du **graphe
quotient** - Création du **diagramme de Hasse** - Analyse Markovienne
: - classes transitoires / persistantes - états absorbants -
irréductibilité

## 🔧 Structures de données (Partie 2)

### `t_tarjan_vertex`

Stocke les données nécessaires au Tarjan :

``` c
typedef struct {
    int identifiant;
    int numero;
    int numero_accessible;
    int dans_pile;
} t_tarjan_vertex;
```

### `t_pile`

Pile dynamique pour Tarjan.

### `t_classe`

Une classe de la partition :

``` c
typedef struct {
    char nom[10];
    int *sommets;
    int nb_sommets;
    int cap_sommets;
} t_classe;
```

### `t_partition`

Contient toutes les classes :

``` c
typedef struct {
    t_classe *classes;
    int nb_classes;
    int cap_classes;
} t_partition;
```

### `t_link` et `t_link_array`

Représentent les arcs du graphe quotient entre classes.

------------------------------------------------------------------------

## 🧩 Fonctions Tarjan (Partie 2)

### `initialiser_tarjan_data(G)`

Crée le tableau des sommets utilisés pour Tarjan.

### `tarjan_parcours(u_id, G, data, pile, p_index, partition)`

La **fonction centrale** :\
- Numérote les sommets\
- Met à jour `numero_accessible`\
- Détecte les **racines de CFC**\
- Construit les classes

### `algorithme_tarjan(G)`

-   Crée une partition vide\
-   Initialise Tarjan\
-   Exécute les parcours\
-   Renvoie la partition complète

------------------------------------------------------------------------

## 🧩 Création du graphe quotient et du diagramme de Hasse

### `creer_tableau_appartenance(partition, taille)`

Crée le tableau `tab[s] = classe`.

### `creer_diagramme_hasse(G, partition, tab)`

Construit le graphe des classes.

### `removeTransitiveLinks()`

Supprime les liens transitifs pour obtenir le **diagramme de Hasse**
minimal.

### `afficher_diagramme_mermaid()`

Crée un fichier Mermaid `.mmd` qui représente les classes et leurs
relations.

------------------------------------------------------------------------

## 🔍 Analyse Markovienne (Partie 2)

Fonction : `analyser_graphe()`

Détermine : - Classes **transitoires** - Classes **persistantes** -
États **absorbants** - Graphe **irréductible** ou non

Le comportement correspond exactement à la page 9 du PDF Partie 2.

------------------------------------------------------------------------

# PARTIE 3 --- Distributions, Matrices, Convergence, Périodicité

Basé sur le PDF Partie 3 (pages 1 à 8).

Implémenté dans `matrix.c` / `matrix.h` + `main.c`.

## 🔢 Objectifs :

-   Construire la **matrice de transition**
-   Calculer :
    -   `M²`, `M³`, ..., `Mn`
-   Calculer les **distributions Πn**
-   Détecter la convergence vers une **distribution stationnaire**
-   Extraire les **sous-matrices** par classe
-   Déterminer la **périodicité** (bonus)

------------------------------------------------------------------------

## 🧩 Fonctions (Partie 3)

### `createEmptyMatrix(n)`

Crée une matrice remplie de zéros.

### `createTransitionMatrix(G)`

Crée la matrice 𝑀 du graphe de Markov.

### `copyMatrix(A, B)`

Copie A → B.

### `multiplyMatrices(A, B, C)`

Effectue `C = A × B`.

### `diffMatrix(A, B)`

Calcule\
\[ `\sum`{=tex}*i `\sum`{=tex}*j \|a*{ij} - b*{ij}\| \]\
Utilisé pour détecter la convergence.

------------------------------------------------------------------------

## 🧩 Distributions Πn

Une distribution est un vecteur ligne :\
\[ `\Pi`{=tex}\_n = `\Pi`{=tex}\_0 imes M\^n \]

Le fichier **main.c** implémente :\
- Lecture d'une distribution initiale\
- Calcul de `Mⁿ` successifs\
- Export CSV\
- Arrêt lorsque `diff(Mⁿ, Mⁿ⁻¹) < ε`

------------------------------------------------------------------------

# Sous-matrices et distributions limite par classe

La fonction :

``` c
t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index);
```

extrait la matrice d'une classe.\
Elle sert à chercher les **distributions stationnaires propres à chaque
classe persistante** comme demandé dans la Partie 3.

------------------------------------------------------------------------

# 🔷 Périodicité des classes (Bonus)

Intégration du code du PDF p.7 :

-   Calcul des puissances successives
-   Détection des indices où la diagonale est non nulle
-   Calcul du **PGCD** pour obtenir la période

Fonction :

``` c
int getPeriod(t_matrix sub_matrix);
```

------------------------------------------------------------------------

# 📝 Contenu du menu (main.c)

Le programme permet :

-   Charger un graphe
-   Vérifier Markov
-   Dessiner le graphe (Mermaid)
-   Calculer la matrice
-   Calculer Mⁿ
-   Exporter les distributions
-   Appliquer Tarjan
-   Générer le Hasse
-   Analyser le graphe
-   Calculer les distributions stationnaires par classe
-   Calculer la période

------------------------------------------------------------------------

# 🙌 Conclusion

Ce projet implémente **l'intégralité** de l'étude d'un graphe de Markov
: - Structure - Probabilités - Connexité - Classes - Hasse -
Stationnarité - Périodicité

Le code est modulaire, propre et entièrement conforme aux 3 parties du
sujet.
