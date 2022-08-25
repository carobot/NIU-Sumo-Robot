#include "task.cpp"
class MotorTask : public Task {
    private:
    // create your globally accessed variables and internal processing here

    public:
    // create your publicly accessible functions here

        // class constructor - this is called by the kernel when the program starts
        // initialise your variables here
        MotorTask (unsigned char * pid, unsigned char * priority, unsigned long * optimal_rate) {
            
            // pass on some information about your code to help the kernel schedule your task accordingly

            // a unique number identifier for your specific process
            // the kernel will refer to your code using this PID, and other processes will request data with your PID
            //*pid = 0;

            // a 0-9 scale of how important your code is to the processor
            // 0 is realtime - run whenever possible - avoid using this as much as possible
            // 1 is highest priority - run at optimal frequency whenever possible - for important tasks
            // 9 is lowest priority - give it the scraps of whatever processor time is left over
            //*priority = 0;

            // frequency of clock cycles you'd like this to run at - 133,333 is 60Hz, or 60fps on our 8MHz processor
            // if 60Hz is ok, this does not need to be changed
            //*optimal_rate = 133333U;

        }

        // this will be called by the kernel every "frame", write your code in here
        int run () {

        }
};