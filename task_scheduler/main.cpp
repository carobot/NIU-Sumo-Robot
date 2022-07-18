// simple task scheduling through prioritisation on each loop

#include <stdio.h>
#include <stdlib.h>
#include <ctime>

#define POLL_RATE_MS 1000
#define INITIAL_TASKS 5

// Errors:
// 69: bad pointer assignment

struct Task {
    void (*target)(int);
    int pid;
    unsigned char priority; // range of 0-255, where 255 is the lowest
    unsigned long last_active;
    unsigned long preferred_rate; // optimal rate of execution, ms
};

Task ** tasks;
char active_tasks;

void error (char * message) {
    printf("Error: %s\n", message);
    while (1);
}

/*void setup () {

}*/

/*void loop () {

}*/

void dummy1 (int var) {
    printf("I do nothing. PID: %d\n", var);
    return;
}

int main () { // place code into setup() and loop() when finished
    

    { // emulate Arduino setup() ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        printf("Emulating Arduino setup()\n");
        tasks = (Task**)malloc(INITIAL_TASKS * sizeof(Task));
        if (tasks == nullptr) { printf("Bad pointer assignment.\n"); return (69); }

        active_tasks = 0;

        printf("Emulating Arduino task setup\n");
        for (int i = 0; i < INITIAL_TASKS; i++) { // initialise first task list
            struct Task * temp_task = (struct Task*)malloc(sizeof(Task));
            if (temp_task == nullptr) { printf("Bad pointer assignment.\n"); return (69); }

            temp_task->target = dummy1;
            temp_task->pid = i;
            temp_task->priority = 0;
            temp_task->last_active = std::clock(); // set to initial clock
            temp_task->preferred_rate = 16U;

            tasks[i] = temp_task; temp_task = nullptr;
            printf("Task %d added to stack:\n", i+1);
        }
    }
    printf("Emulating Arduino loop\n");
    while (1) { // emulate Arduino loop() ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        
    }

    free(tasks);
}