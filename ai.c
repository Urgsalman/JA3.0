#include "ai.h"
#include <stdlib.h>
#include <time.h>

static int initialized = 0;

void init_random() {
    if (!initialized) {
        srand((unsigned int)time(NULL));
        initialized = 1;
    }
}

int ai_easy(int total_sticks) {
    init_random();
    
    // 30% chance of making a random move
    if (rand() % 100 < 30) {
        int max_take = (total_sticks >= 3) ? 3 : total_sticks;
        return (rand() % max_take) + 1;
    }
    
    // 70% chance of making a semi-intelligent move
    return ai_medium(total_sticks);
}

int ai_medium(int total_sticks) {
    init_random();
    
    // 60% chance of making the optimal move
    if (rand() % 100 < 60) {
        return ai_hard(total_sticks);
    }
    
    // 40% chance of making a reasonable but sub-optimal move
    if (total_sticks <= 3) {
        return total_sticks;
    }
    
    int remainder = total_sticks % 4;
    if (remainder == 0) {
        return (rand() % 2) + 1;  // Take 1 or 2 when we should take 3
    }
    return (rand() % 3) + 1;  // Take any valid number
}

int ai_hard(int total_sticks) {
    if (total_sticks <= 0) return 0;
    if (total_sticks <= 3) return total_sticks;
    
    int remainder = total_sticks % 4;
    
    if (remainder == 0) {
        // On a multiple of 4, take 3 to avoid leaving a multiple of 4
        return 3;
    } else {
        // Take the remainder to reach a multiple of 4
        return remainder;
    }
}
