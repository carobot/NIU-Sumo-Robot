// simple task scheduling through prioritisation on each loop

#include <stdio.h>
#include <stdlib.h>
#include <ctime>

#define CLOCK_RATE 133333 // target cycle gaps between starts
#define INITIAL_TASKS 5

// Errors:
// 69: bad pointer assignment

struct Task {
    void (*target)(int); // pointer to component interface code
    int pid;
    unsigned char priority; // range of 0-255, where 255 is the lowest and 0 is urgent - 0 will lock thread
    unsigned long last_active; // last start point of component
    unsigned long preferred_rate; // optimal rate of execution, ms
    unsigned long predicted_runtime; // an estimated time, as project components have mostly fixed runtimes
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

void run (struct Task * target) {
    target->last_active = std::clock();
    printf("Task run: PID%d\n", target->pid);
    // run task
    target->target(0); // run target task
}

int main () { // place code into setup() and loop() when finished
    

    { // emulate Arduino setup() ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        printf("Emulating Arduino setup()\n");

        // Arduino pin setups go here ~~~vvv~~~

        // various setups for robot components go here ~~~vvv~~~


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
            //temp_task->preferred_rate = 16U; // use CLOCK_RATE for preferred frequency

            int start = std::clock();
            dummy1(-1);
            temp_task->predicted_runtime = std::clock() - start;
            temp_task->last_active = start;


            tasks[i] = temp_task; temp_task = nullptr;
            printf("Task %d added to stack:\n", i+1);
        }
    }
    printf("Emulating Arduino loop\n");
    while (1) { // emulate Arduino loop() ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        struct Task next;
        long next_target_start = std::clock(); // desired start cycle by highest priority task
        long next_target_end = 0; // estimated end time, if matching priorities, decide by end time

        for (int i = 0; i < active_tasks; i++) {

        }
    }

    free(tasks);
}