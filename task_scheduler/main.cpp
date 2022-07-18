#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define POLL_RATE_MS 1000
#define INITIAL_TASKS 5

// Errors:
// 69: bad pointer assignment

struct Task {
    void * target;
    int priority;
    long inactive_time;
};

Task * tasks;

void error (char * message) {
    printf("Error: %s\n", message);
    while (1);
}

/*void setup () {

}*/

/*void loop () {

}*/

int main () { // place code into loop() when finished
    printf("Program start - emulating Arduino loop\n");

    { // emulate Arduino setup() ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        tasks = (Task*)malloc(INITIAL_TASKS * sizeof(Task));
        if (tasks == nullptr) {
            printf("Bad pointer assignment.\n");
            return (69);
        }
    }

    while (1) {
        
    }
}