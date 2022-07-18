#include <stdio.h>
#include <stdlib.h>
#include <ctime>

#define POLL_RATE_MS 1000
#define INITIAL_TASKS 5

// Errors:
// 69: bad pointer assignment

struct Task {
    void (*target)();
    unsigned char priority; // range of 0-255, where 255 is the lowest
    unsigned long last_active;
    unsigned long preferred_rate; // optimal rate of execution, ms
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

void dummy1 () {
    printf("I do nothing.\n");
    return;
}

int main () { // place code into loop() when finished
    

    { // emulate Arduino setup() ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        printf("Emulating Arduino setup()\n");
        tasks = (Task*)malloc(INITIAL_TASKS * sizeof(Task));
        if (tasks == nullptr) { printf("Bad pointer assignment.\n"); return (69); }

        printf("Emulating Arduino task setup\n");
        for (int i = 0; i < INITIAL_TASKS; i++) { // initialise first task list
            struct Task * temp_task;

            temp_task->target = dummy1;
            temp_task->priority = i;
            temp_task->last_active = std::clock(); // set to initial clock
            temp_task->preferred_rate = 16U;
        }
    }

    while (1) { // emulate Arduino loop() ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        printf("Emulating Arduino loop\n");
    }
}