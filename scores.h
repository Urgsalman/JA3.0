#ifndef SCORES_H
#define SCORES_H

typedef struct {
    int score_joueur1;
    int score_joueur2;
} Scores;

void sauvegarder_scores(Scores scores);
Scores charger_scores();

#endif // SCORES_H