/* NFA Simulator for matching character sequences. */

#include "headers.h"
#include "transition_table.c"
#include <stdio.h>

int accepting_states_set[MAX_NUM_STATES];
size_t accepting_states_set_n = 0;

/*
    Returns an array of states that can be reached with 0 or more
    epsilon transitions from some member of the input state set.
*/
int* epsilon_closure(int* state_set, size_t state_set_n, size_t *out_closure_set_n) {
    int closure_set[MAX_NUM_STATES];
    int closure_set_membership[MAX_NUM_STATES];
    int closure_set_i = 0;

    int i;
    for (i = 0; i < MAX_NUM_STATES; i++) {
        closure_set_membership[i] = 0;
    }

    int stack[MAX_NUM_STATES];
    int stack_i = 0;

    // add all states into the closure set.
    for (i = 0; i < state_set_n; i++) {
        closure_set[closure_set_i++] = state_set[i];
        closure_set_membership[state_set[i]] = 1;
    }

    // push all the states onto the 'stack.'
    for (i = 0; i < state_set_n; i++) {
        stack[stack_i++] = state_set[i];
    }

    while (stack_i != 0) {
        int top = stack[stack_i - 1];
        stack_i--;

        size_t num_states;
        int* to_states = get_states(top, EPSILON, &num_states);

        // for every state with an epsilon transition from the 'top'
        // state, put that in the set and on the stack.
        int j;
        for (j = 0; j < num_states; j++) {
            int state = to_states[j];
            if (!closure_set_membership[state]) {
                closure_set[closure_set_i++] = state;
                closure_set_membership[state] = 1;
            }

            stack[stack_i++] = state;
        }
    }

    *out_closure_set_n = closure_set_i;
    int* closure_set_dest = malloc(closure_set_i * sizeof(int));
    memcpy(closure_set_dest, closure_set, closure_set_i * sizeof(int));

    return closure_set_dest;
}

/*
    Returns the set of states for which there is a transition on
    the given input character from states in the state set.
*/
int* move(int* state_set, size_t state_set_n, int ch, size_t* out_move_set_n) {
    int move_set[MAX_NUM_STATES];
    int move_set_membership[MAX_NUM_STATES];
    int move_set_i = 0;

    int i;
    for (i = 0; i < MAX_NUM_STATES; i++) {
        move_set_membership[i] = 0;
    }

    for (i = 0; i < state_set_n; i++) {
        int state = state_set[i];
        size_t trans_state_set_n;

        // get the states reachable with ch-transitions from the state.
        int* trans_state_set = get_states(state, ch, &trans_state_set_n);

        // add them to the set.
        int j;
        for (j = 0; j < trans_state_set_n; j++) {
            int trans_state = trans_state_set[j];

            if (!move_set_membership[trans_state]) {
                move_set[move_set_i++] = trans_state;
                move_set_membership[trans_state] = 1;
            }
        }
    }

    *out_move_set_n = move_set_i;
    int* move_set_dest = malloc(move_set_i * sizeof(int));
    memcpy(move_set_dest, move_set, move_set_i * sizeof(int));

    return move_set_dest;
}

/*
    Returns 1 if the string is accepted by the NFA, 0 otherwise.
*/
int is_accepted(char* s) {
    size_t start_states_n;
    int *start_states = epsilon_closure((int[]) {0}, 1, &start_states_n);
    int *current_states = start_states;
    size_t current_states_n = start_states_n;
    
    int str_i = 0;
    while (s[str_i] != '\0') {
        char c = s[str_i++];

        current_states = epsilon_closure(
            move(current_states, current_states_n, c, &current_states_n),
            current_states_n,
            &current_states_n
            );
    }

    int i;
    for (i = 0; i < current_states_n; i++) {
        int j;
        for (j = 0; j < accepting_states_set_n; j++) {
            // As soon as an accepting state is found among the current 
            // states, accept the string.
            if (current_states[i] == accepting_states_set[j]) {
                return 1;
            }
        }
    }

    return 0;
}

int main() {

    // NFA recognising "(a|b)*abb" 
    add_transition(0, EPSILON, (int[]) {1,7}, 2);
    add_transition(1, EPSILON, (int[]) {2,4}, 2);
    add_transition(2, 'a', (int[]) {3}, 1);
    add_transition(3, EPSILON, (int[]) {6}, 1);
    add_transition(4, 'b', (int[]) {5}, 1);
    add_transition(5, EPSILON, (int[]) {6}, 1);
    add_transition(6, EPSILON, (int[]) {1, 7}, 2);
    add_transition(7, 'a', (int[]) {8}, 1);
    add_transition(8, 'b', (int[]) {9}, 1);
    add_transition(9, 'b', (int[]) {10}, 1);

    accepting_states_set[0] = 10;
    accepting_states_set_n = 1;

    while (1) {
        char s[100];
        scanf("%s", s);
        int b = is_accepted(s);
        printf("%d \n", b);
    }


    return 0;
}
