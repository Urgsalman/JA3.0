#include "scores.h"
#include <stdio.h>
#include <stdlib.h>

void sauvegarder_scores(Scores scores) {
    system("mkdir -p resources/data");

    FILE *fichier = fopen("resources/data/scores.txt", "w");
    if (fichier == NULL) {
        fprintf(stderr, "Error: Cannot open scores.txt\n");
        return;
    }
    fprintf(fichier, "%d %d", scores.score_joueur1, scores.score_joueur2);
    fclose(fichier);
}

Scores charger_scores() {
    Scores scores = {0, 0};
    FILE *fichier = fopen("resources/data/scores.txt", "r");
    if (fichier == NULL) {
        return scores;
    }
    
    if (fscanf(fichier, "%d %d", &scores.score_joueur1, &scores.score_joueur2) != 2) {
        scores.score_joueur1 = 0;
        scores.score_joueur2 = 0;
    }
    fclose(fichier);
    return scores;
}