// this is your task/process - this code will be compiled with the kernel and included as a task to run
class Task {
    private:
    // create your globally accessed variables and internal processing here

    public:
    // create your publicly accessible functions here

        // class constructor - this is called by the kernel when the program starts
        // initialise your variables here
        //Task (unsigned char * pid, unsigned char * priority, unsigned long * optimal_rate) = 0;

        // this will be called by the kernel every "frame", write your code in here
        virtual int run () = 0;
};