#include "headers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Transition table.
int* transition_table[MAX_NUM_STATES][256];
int to_state_set_size[MAX_NUM_STATES][256];

void add_transition(int from, int ch, int* to, size_t to_n) {
    transition_table[from][ch] = malloc(to_n * sizeof(int));
    memcpy(transition_table[from][ch], to, to_n * sizeof(int));
    to_state_set_size[from][ch] = to_n;
}

// Returns the array of the states resulting from a transition.
int* get_states(int from, int ch, size_t* out_n) {
    *out_n = to_state_set_size[from][ch];
    return transition_table[from][ch];
}

void print_table() {
    // print out each entry in the table.

    int i;
    for (i = 0; i < MAX_NUM_STATES; i++) {
        int j;
        for (j = 0; j < 256; j++) {
            int* states = transition_table[i][j];

            if (states) {
                if (j == EPSILON) {
                    printf("%d | EPSILON | ", i);
                } else {
                    printf("%d | %c | ", i, j);
                }

                int k;
                for (k = 0; k < to_state_set_size[i][j]; k++) {
                    printf("%d,", states[k]);
                }

                printf("\n");
            }
        }
    }
}